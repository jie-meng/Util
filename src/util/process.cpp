#include "process.hpp"
#include <vector>
#include "thread.hpp"

#ifdef _PLATFORM_WINDOWS_
#include <windows.h>
#endif // _PLATFORM_WINDOWS_
#ifdef _PLATFORM_UNIX_
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#endif // _PLATFORM_LINUX_

namespace util
{

const std::string kUtilProcessPipeKill = "{Util.Process.ReadOutput.Thread.Kill.Cmd}";

#ifdef _PLATFORM_WINDOWS_

int executeProcess(const std::string& cmdline, const std::string& cur_path)
{
    PROCESS_INFORMATION pi;
    //STARTUPINFO sui;
    _STARTUPINFOA sui;

    ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
    ZeroMemory(&sui, sizeof(STARTUPINFO));

#ifdef _GUI_
    sui.wShowWindow = SW_HIDE;
    sui.dwFlags |= STARTF_USESHOWWINDOW;
#endif

    char* path = NULL;
    if ("" != cur_path)
        path = (char*)cur_path.c_str();

    if (!::CreateProcessA(NULL, const_cast<char*>(cmdline.c_str()), NULL, NULL,
                         TRUE, 0, NULL, path, &sui, &pi))
        return -1;

    ::CloseHandle(pi.hThread);

    ::WaitForSingleObject(pi.hProcess, INFINITE);
    DWORD exit_code(0);
    if(!::GetExitCodeProcess(pi.hProcess, &exit_code))
        return -1;

    ::CloseHandle(pi.hProcess);

    return exit_code;
}

bool executeProcessAsyn(const std::string& cmdline, const std::string& cur_path)
{
    PROCESS_INFORMATION pi;
    _STARTUPINFOA sui;

    ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
    ZeroMemory(&sui, sizeof(STARTUPINFO));

#ifdef _GUI_
    sui.wShowWindow = SW_HIDE;
    sui.dwFlags |= STARTF_USESHOWWINDOW;
#endif

    char* path = NULL;
    if ("" != cur_path)
        path = (char*)cur_path.c_str();

    if (!::CreateProcessA(NULL, const_cast<char*>(cmdline.c_str()), NULL, NULL,
                         TRUE, 0, NULL, path, &sui, &pi))
        return false;

    ::CloseHandle(pi.hThread);
    ::CloseHandle(pi.hProcess);

    return true;
}

struct Process::ProcessImpl
{
    ProcessImpl() :
        stdout_read_pipe_(NULL),
        stdout_write_pipe_(NULL),
        stderr_read_pipe_(NULL),
        stderr_write_pipe_(NULL),
        stdin_read_pipe_(NULL),
        stdin_write_pipe_(NULL),
        read_output_thread_(UtilBind(&ProcessImpl::readOutput, this)),
        read_error_thread_(UtilBind(&ProcessImpl::readError, this)),
        output_func_(0)
    {
        memset(&pi_, 0, sizeof(pi_));
    }

    bool create(const std::string& cmdline, const std::string& cur_path, bool input, bool output, OutputFunc output_func)
    {
        if (isRunning())
            return false;

        if (input)
        {
            if (!createPipe(stdin_read_pipe_, stdin_write_pipe_))
                return false;
        }
        if (output)
        {
            if (!createPipe(stdout_read_pipe_, stdout_write_pipe_))
                return false;

            if (!createPipe(stderr_read_pipe_, stderr_write_pipe_))
                return false;
        }

        output_func_ = output_func;

        _STARTUPINFOA sui;
        ZeroMemory(&pi_, sizeof(PROCESS_INFORMATION));
        ZeroMemory(&sui, sizeof(STARTUPINFO));
        sui.cb = sizeof(sui);

        sui.dwFlags = STARTF_USESTDHANDLES;

#ifdef _GUI_
        sui.wShowWindow = SW_HIDE;
        sui.dwFlags |= STARTF_USESHOWWINDOW;
#endif

        if (input)
            sui.hStdInput = stdin_read_pipe_;
        if (output)
        {
            sui.hStdOutput = stdout_write_pipe_;
            sui.hStdError = stderr_write_pipe_;
        }

        char* path = NULL;
        if ("" != cur_path)
            path = (char*)cur_path.c_str();

        char tmp[kBufSize] = {0};
        strcpy(tmp, cmdline.c_str());
        if (!::CreateProcessA(NULL, tmp, NULL, NULL,
                             TRUE, 0, NULL, path, &sui, &pi_))
        {
            kill();
            return false;
        }
        //close useless handle
        ::CloseHandle(pi_.hThread);
        if (input)
        {
            ::CloseHandle(stdin_read_pipe_);
            stdin_read_pipe_ = 0;
        }
        if (output)
        {
            read_output_thread_.start();
            read_error_thread_.start();
        }
        return true;
    }

    void sendQuitReadOutputCmd()
    {
        DWORD write_cnt = 0;
        ::WriteFile(stdout_write_pipe_, kUtilProcessPipeKill.c_str(), kUtilProcessPipeKill.length(), &write_cnt, NULL);
        ::WriteFile(stderr_write_pipe_, kUtilProcessPipeKill.c_str(), kUtilProcessPipeKill.length(), &write_cnt, NULL);
        //::FlushFileBuffers(stdout_write_pipe_);
    }

    void kill()
    {
        sendQuitReadOutputCmd();
        closePipe(stdout_read_pipe_, stdout_write_pipe_);
        closePipe(stderr_read_pipe_, stderr_write_pipe_);
        closePipe(stdin_read_pipe_, stdin_write_pipe_);
        closeProcess();
        read_output_thread_.join();
        read_error_thread_.join();
        output_func_ = 0;
    }

    bool input(const std::string& str)
    {
        if (stdin_write_pipe_)
        {
            DWORD write_cnt = 0;
            return ::WriteFile(stdin_write_pipe_, str.c_str(), str.length(), &write_cnt, NULL);
        }

        return false;
    }

    void readOutput()
    {
        if(!output_func_)
            return;

        DWORD read = 0;
        char buf[kBufSize];

        while (pi_.hProcess)
        {
            memset(buf, 0, sizeof(buf));
            if(!::ReadFile(stdout_read_pipe_, buf, sizeof(buf), &read, NULL))
                break;
            std::string read_str(buf);
            if (strContains(read_str, kUtilProcessPipeKill))
                break;

            output_func_(read_str);
        }
    }

    void readError()
    {
        if(!output_func_)
            return;

        DWORD read = 0;
        char buf[kBufSize];

        while (pi_.hProcess)
        {
            memset(buf, 0, sizeof(buf));
            if(!::ReadFile(stderr_read_pipe_, buf, sizeof(buf), &read, NULL))
                break;
            std::string read_str(buf);
            if (strContains(read_str, kUtilProcessPipeKill))
                break;

            output_func_(read_str);
        }
    }

    bool createPipe(HANDLE& read, HANDLE& write)
    {
        SECURITY_ATTRIBUTES sa;
        sa.bInheritHandle = TRUE;
        sa.lpSecurityDescriptor = NULL;
        sa.nLength = sizeof(SECURITY_ATTRIBUTES);
        return ::CreatePipe(&read, &write, &sa, 0);
    }

    void closePipe(HANDLE& read, HANDLE& write)
    {
        if (write)
        {
            ::CloseHandle(write);
            write = 0;
        }

        if (read)
        {
            ::CloseHandle(read);
            read = 0;
        }
    }

    void closeProcess()
    {
        if (pi_.hProcess)
        {
            ::TerminateProcess(pi_.hProcess, 0);
            ::CloseHandle(pi_.hProcess);
        }
        memset(&pi_, 0, sizeof(pi_));
    }

    bool isRunning()
    {
        if (0 == pi_.hProcess)
            return false;
        return WAIT_OBJECT_0 != WaitForSingleObject(pi_.hProcess, 0);
    }

    HANDLE stdout_read_pipe_;
    HANDLE stdout_write_pipe_;
    HANDLE stderr_read_pipe_;
    HANDLE stderr_write_pipe_;
    HANDLE stdin_read_pipe_;
    HANDLE stdin_write_pipe_;
    PROCESS_INFORMATION pi_;
    Thread read_output_thread_;
    Thread read_error_thread_;
    OutputFunc output_func_;
};

#endif // _PLATFORM_WINDOWS_

#ifdef _PLATFORM_UNIX_

int executeProcess(const std::string& cmdline, const std::string& cur_path)
{
    pid_t pid = fork();
    switch(pid)
    {
    case -1:
        return -1;
    case 0:
        {
            //extract params
            std::vector<std::string> params;
            strSplitEx(cmdline, " ", "\"", "\"", params);
            if (params.empty())
                exit(-1);

            //change dir
            if("" != cur_path)
                chdir(cur_path.c_str());

            //exec
            std::vector<int> argv(params.size()+1);
            size_t i(0);
            for (; i<params.size(); ++i)
                argv[i] = (int)params[i].c_str();
            argv[i] = 0;
            if(-1 == execvp(params[0].c_str(), (char* const*)&argv[0]))
                exit(-1);
        }
        exit(0);
    default:
        {
            int state_val(0);
            if(-1 == waitpid(pid, &state_val, 0))
            {
                return -1;
            }
            else
            {
                int ret = WEXITSTATUS(state_val);
                return (255 == ret) ? -1 : ret;
            }
        }
    }
}

bool executeProcessAsyn(const std::string& cmdline, const std::string& cur_path)
{
    pid_t pid = fork();
    switch(pid)
    {
    case -1:
        return false;
    case 0:
        {
            //extract params
            std::vector<std::string> params;
            strSplitEx(cmdline, " ", "\"", "\"", params);
            if (params.empty())
                exit(-1);

            //change dir
            if("" != cur_path)
                chdir(cur_path.c_str());

            //exec
            std::vector<int> argv(params.size()+1);
            size_t i(0);
            for (; i<params.size(); ++i)
                argv[i] = (int)params[i].c_str();
            argv[i] = 0;

            if(-1 == execvp(params[0].c_str(), (char* const*)&argv[0]))
                exit(-1);
        }
        exit(0);
    default:
        {
            msleep(500);
            return (0 != waitpid(pid, (int*)0, WNOHANG)) ? false : true;
        }
    }
}

struct Process::ProcessImpl
{
    ProcessImpl() :
        pid_(0),
        read_output_thread_(UtilBind(&ProcessImpl::readOutput, this)),
        output_func_(0)
    {
        memset(input_pipe_, 0, sizeof(input_pipe_));
        memset(output_pipe_, 0, sizeof(output_pipe_));
    }

    bool create(const std::string& cmdline, const std::string& cur_path, bool input, bool output, OutputFunc output_func)
    {
        if (isRunning())
            return false;

        output_func_ = output_func;

        if (input)
        {
            if (0 != pipe(input_pipe_))
                return false;
        }

        if (output)
        {
            if (0 != pipe(output_pipe_))
                return false;
        }

        pid_ = fork();
        switch(pid_)
        {
        case -1:
            return false;
        case 0:
            {

                if(!processFunc(cmdline, cur_path, input, output))
                    exit(-1);
            }
            exit(0);
        default:
            {
                if (0 != waitpid(pid_, (int*)0, WNOHANG))
                    return false;

                if (input)
                {
                    close(input_pipe_[0]);
                }

                if (output)
                {
                    close(output_pipe_[1]);
                    read_output_thread_.start();
                }
            }
            return true;
        }
    }

    bool processFunc(const std::string& cmdline, const std::string& cur_path, bool input, bool output)
    {
        //extract params
        std::vector<std::string> params;
        strSplitEx(cmdline, " ", "\"", "\"", params);
        if (params.empty())
            return false;

        //change dir
        if("" != cur_path)
            chdir(cur_path.c_str());

        //reset input
        if (input)
        {
            close(0);
            dup(input_pipe_[0]);
            closePipe(input_pipe_);
        }

        //reset output
        if (output)
        {
            close(1);
            dup(output_pipe_[1]);
            closePipe(output_pipe_);
        }

        //exec
        std::vector<int> argv(params.size()+1);
        size_t i(0);
        for (; i<params.size(); ++i)
            argv[i] = (int)params[i].c_str();
        argv[i] = 0;
        if(-1 == execvp(params[0].c_str(), (char* const*)&argv[0]))
            return false;

        return true;
    }

    void closePipe(int* pipe)
    {
        if (pipe[0])
        {
            close(pipe[0]);
            pipe[0] = 0;
        }
        if (pipe[1])
        {
            close(pipe[1]);
            pipe[1] = 0;
        }
    }

    void kill()
    {
        closePipe(input_pipe_);
        closePipe(output_pipe_);
        closeProcess();
        read_output_thread_.join();
        output_func_ = 0;
    }

    void closeProcess()
    {
        if (pid_)
            ::kill(pid_, SIGHUP);
        pid_ = 0;
    }

    void readOutput()
    {
        if(!output_func_)
            return;

        char buf[kBufSize] = {0};
        while(true)
        {
            memset(buf, 0, sizeof(buf));
            size_t len = ::read(output_pipe_[0], buf, sizeof(buf));
            if (len <= 0)
            {
                break;
            }

            output_func_(std::string(buf));
        }
    }

    bool input(const std::string& str)
    {
        if (input_pipe_[1])
        {
            size_t len = ::write(input_pipe_[1], str.c_str(), str.length());
            return (len >= 0) ? true : false;
        }
        else
        {
            return false;
        }
    }

    bool isRunning()
    {
        if (0 == pid_)
            return false;
        return (0 != waitpid(pid_, (int*)0, WNOHANG)) ? false : true;
    }

    pid_t pid_;
    int input_pipe_[2];
    int output_pipe_[2];
    Thread read_output_thread_;
    OutputFunc output_func_;
};

#endif // _PLATFORM_LINUX_

Process::Process() :
    impl_(new ProcessImpl())
{
}

Process::~Process()
{
    kill();
}

bool Process::create(const std::string& cmdline, const std::string& cur_path, bool input, bool output, OutputFunc output_func)
{
    return impl_->create(cmdline, cur_path, input, output, output_func);
}

void Process::kill()
{
    impl_->kill();
}

bool Process::input(const std::string& str)
{
    return impl_->input(str);
}

bool Process::isRunning()
{
    return impl_->isRunning();
}

} //namespace util
