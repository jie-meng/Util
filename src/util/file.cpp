#include "file.hpp"
#include <fstream>

#ifdef _PLATFORM_WINDOWS_
#include <io.h>
#include <direct.h>
#include <Shlwapi.h>
#endif
#ifdef _PLATFORM_UNIX_
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#ifdef __APPLE__
#include <mach-o/dyld.h>
#endif
#endif

namespace util
{

using namespace std;

std::string readTextFile(const std::string& file)
{
    std::string content("");
    std::ifstream ifs(file.c_str());
    if (ifs.is_open())
    {
        std::string line("");
        while(!ifs.eof())
        {
            std::getline(ifs, line);
            content += line + "\n";
        }
        ifs.close();

        if (content.size() > 0)
        {
            std::string::iterator it = content.end();
            --it;
            if (*it == '\n')
                content.erase(it);
        }
    }
    return content;
}

bool writeTextFile(const std::string& file, const std::string& content, bool append)
{
    std::ofstream ofs;
    if (append)
        ofs.open(file.c_str(), std::ios::out | std::ios::app);
    else
        ofs.open(file.c_str());

    if (ofs.is_open())
    {
        ofs<<content;
        ofs.close();
        return true;
    }
    else
    {
        return false;
    }
}

bool createBinaryFile(const std::string& file, uint64_t file_size, char default_data)
{
    std::ofstream ofs(file.c_str(), std::ios::binary);
    if (!ofs.is_open())
        return false;

    if (file_size > 0)
    {
        char buf[1024*1024];
        memset(buf, default_data, sizeof(buf));
        uint64_t i = 0;
        while(i + sizeof(buf) < file_size)
        {
            ofs.write(buf, sizeof(buf));
            i += sizeof(buf);
        }
        ofs.write(buf, file_size - i);
        return true;
    }
    else if (0 == file_size)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool readBinaryFile(const std::string& file, char* pout_buf, size_t read_len, std::streamoff offset, std::ios::seekdir pos)
{
    if (0 == pout_buf)
        return false;

    std::ifstream ifs(file.c_str(), std::ios::binary);
    if (ifs.is_open())
    {
        ifs.seekg(offset, pos);
        ifs.read(pout_buf, read_len);
        return true;
    }
    else
    {
        return false;
    }
}

bool writeBinaryFile(const std::string& file, char* pbuf, size_t write_len, bool append)
{
    if (0 == pbuf)
        return false;

    std::ios_base::openmode openmode = std::ios::binary | std::ios::out;
    if (append)
        openmode = std::ios::binary | std::ios::out | std::ios::app;

    std::ofstream ofs(file.c_str(), openmode);
    if (ofs.is_open())
    {
        ofs.write(pbuf, write_len);
        return true;
    }
    else
    {
        return false;
    }
}

bool overwriteBinaryFile(const std::string& file, char* pbuf, size_t write_len, std::streamoff offset, std::ios::seekdir pos)
{
    if (0 == pbuf)
        return false;

    std::ofstream ofs(file.c_str(), std::ios::binary | std::ios::in | std::ios::out);
    if (ofs.is_open())
    {
        ofs.seekp(offset, pos);
        ofs.write(pbuf, write_len);
        return true;
    }
    else
    {
        return false;
    }
}

uint64_t fileSize(const std::string& file)
{
    if (!isPathFile(file))
        return (uint64_t)(-1);
    
    std::ifstream in(file.c_str(), std::ifstream::ate | std::ifstream::binary);
    return in.tellg();
}

std::string fileExtension(const std::string& file)
{
    size_t found = file.find_last_of(".");
    if (found == std::string::npos)
        return "";
    else
        return file.substr(found + 1);
}

std::string fileBaseName(const std::string& file)
{
    std::string str = splitPathname(file).second;
    size_t found = str.find_last_of(".");
    if (found == std::string::npos)
        return str;
    else
        return str.substr(0, found);
}

bool fileCopy(const std::string& src_path, const std::string& dest_path, bool fail_if_exitst)
{
    if (fail_if_exitst && isPathExists(dest_path)) return false;

    std::ifstream ifs(src_path.c_str(), std::ios::binary);
    if (!ifs.is_open()) return false;

    char buf[kBufSize];
    std::ofstream ofs(dest_path.c_str(), std::ios::binary);

    while (!ifs.eof())
    {
        ifs.read(buf, sizeof(buf));
        ofs.write(buf, ifs.gcount());
    }

    return true;
}

std::pair<std::string, std::string> splitPathname(const std::string& str)
{
    size_t found = str.find_last_of("/\\");
    if (found == std::string::npos)
        return std::make_pair(".", str);
    else
        return std::make_pair(str.substr(0, found), str.substr(found + 1));
}

#ifdef _PLATFORM_WINDOWS_
time_t systemTimeToTime_t(const SYSTEMTIME& st)
{
    tm tmptm = {st.wSecond,
                st.wMinute,
                st.wHour,
                st.wDay,
                st.wMonth - 1,
                st.wYear - 1900,
                st.wDayOfWeek,
                0,
                0};
    return ::mktime(&tmptm);
}
#endif

time_t fileTime(const std::string& file, E_FileTime ft)
{
#ifdef _PLATFORM_WINDOWS_
    WIN32_FILE_ATTRIBUTE_DATA wfad;
    if (!::GetFileAttributesExA(file.c_str(), ::GetFileExInfoStandard, &wfad))
        return 0;

    FILETIME ftlocal;
    SYSTEMTIME st;
    switch (ft)
    {
        case FtCreationTime:
            FileTimeToLocalFileTime(&wfad.ftCreationTime, &ftlocal);
            break;
        case FtLastWriteTime:
            FileTimeToLocalFileTime(&wfad.ftLastWriteTime, &ftlocal);
            break;
        case FtLastAccessTime:
            FileTimeToLocalFileTime(&wfad.ftLastAccessTime, &ftlocal);
            break;
        default:
            return 0;
    }

    FileTimeToSystemTime(&ftlocal, &st);
    return systemTimeToTime_t(st);

#endif // _PLATFORM_WINDOWS_
#ifdef _PLATFORM_UNIX_
    struct stat buf;
    if (0 != ::stat(file.c_str(), &buf))
        return 0;

    switch (ft)
    {
        case FtCreationTime:
            return buf.st_ctime;
        case FtLastWriteTime:
            return buf.st_mtime;
        case FtLastAccessTime:
            return buf.st_atime;
        default:
            return 0;
    }
#endif // _PLATFORM_UNIX_
}

std::string appPath()
{
    char buf[kMaxPathLen];
    memset(buf, 0, sizeof(buf));

#ifdef _PLATFORM_WINDOWS_
    GetModuleFileNameA(NULL, buf, sizeof(buf));
    return strReplaceAll(std::string(buf), "\\", "/");
#endif // _PLATFORM_WINDOWS_

#ifdef _PLATFORM_UNIX_
#ifdef __APPLE__
    uint32_t buf_size = kMaxPathLen;
    if (!_NSGetExecutablePath(buf, &buf_size))
        return std::string(buf);
    else
        return "";

#else
    std::string link = strFormat("/proc/%d/exe", getpid());
    readlink(link.c_str(), buf, sizeof(buf));
    return std::string(buf);
#endif

#endif // _PLATFORM_UNIX_
}

std::string currentPath()
{
    size_t i = 1;
    while (true)
    {
        Memory<char> m(i * kMaxPathLen);
        char* p = ::getcwd(m.buf(), m.size());
        if (p)
        {
            std::string cwd = std::string(p);
            return strReplaceAll(cwd, "\\", "/");
        }
        else
        {
            ++i;
        }
    }
}

bool setCurrentPath(const std::string& dir)
{
    return 0 == ::chdir(dir.c_str());
}

std::string relativePathToAbsolutePath(const std::string& path)
{
    std::string path_format = strReplaceAll(path, "\\", "/");

    //linux absolute path
    if (strStartWith(path_format, "/"))
        return path_format;

    //windows absolute path
    if (path_format.length() >= 2 && path_format[1] == ':')
        return path_format;

    if (strStartWith(path_format, "./"))
        path_format = strRight(path_format, path_format.length() - 2);

    return currentPath() + "/" + path_format;
}

bool isPathExists(const std::string& path)
{
#ifdef _PLATFORM_WINDOWS_
    return ::PathFileExistsA(path.c_str());
#endif
#ifdef _PLATFORM_UNIX_
    return 0 == ::access(path.c_str(), F_OK);
#endif
}

bool isPathDir(const std::string& path)
{
#ifdef _PLATFORM_WINDOWS_
    return ::PathIsDirectoryA(path.c_str());
#endif
#ifdef _PLATFORM_UNIX_
    struct stat info;
    stat(path.c_str(), &info);
    if(S_ISDIR(info.st_mode))
        return true;
    else
        return false;
#endif
}

bool isPathFile(const std::string& path)
{
    if (isPathExists(path) && !isPathDir(path))
        return true;
    else
        return false;
}

bool isPathEmpty(const std::string& path)
{
    if (!isPathDir(path))
        return false;

#ifdef _PLATFORM_WINDOWS_
    return ::PathIsDirectoryEmptyA(path.c_str());
#endif
#ifdef _PLATFORM_UNIX_
    if (!pathRemove(path))
        return false;

    mkDir(path);
    return true;
#endif
}

void pathRemoveAll(const std::string& path)
{
    if (!isPathExists(path))
        return;

    if (isPathFile(path))
    {
        ::remove(path.c_str());
        return;
    }

    if (isPathDir(path))
    {
#ifdef _PLATFORM_WINDOWS_
        std::string cmd = std::string("rd /s /q \"") + path + "\"";
#endif
#ifdef _PLATFORM_UNIX_
        std::string cmd = std::string("rm -rf \"") + path + "\"";
#endif
        ::system(cmd.c_str());
    }
}

bool pathRemove(const std::string& path)
{
    if (!isPathExists(path))
        return false;

    if (isPathFile(path))
    {
        return 0 == ::remove(path.c_str());
    }

    if (isPathDir(path))
        return 0 == ::rmdir(path.c_str());

    return false;
}

bool pathRename(const std::string& src_path, const std::string& dest_path)
{
    return 0 == rename(src_path.c_str(), dest_path.c_str());
}

bool mkDir(const std::string& path)
{
#ifdef _PLATFORM_WINDOWS_
        return 0 == ::mkdir(path.c_str());
#endif
#ifdef _PLATFORM_UNIX_
        return 0 == ::mkdir(path.c_str(), S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
#endif
}

template <class Coll>
size_t listFiles(const std::string& path, Coll& out_coll, PathFilter* pfilter)
{
    if (isPathFile(path))
        return out_coll.size();

#ifdef _PLATFORM_WINDOWS_

    //WIN32_FIND_DATA FindFileData;
    _WIN32_FIND_DATAA FindFileData;
    HANDLE hListFile = ::FindFirstFileA((path + "/*").c_str(), &FindFileData);
    if(hListFile == INVALID_HANDLE_VALUE)
    {
        return out_coll.size();
    }
    else
    {
        do
        {
            std::string name = std::string(FindFileData.cFileName);
            if ("." == name || ".." == name)
                continue;

            std::string find_file = path + "/" + name;
            if (!pfilter)
            {
                out_coll.push_back(find_file);
            }
            else
            {
                if (pfilter->filter(find_file))
                    out_coll.push_back(find_file);
            }

        } while(0 != ::FindNextFileA(hListFile, &FindFileData));
    }
    ::FindClose(hListFile);
#endif
#ifdef _PLATFORM_UNIX_
    DIR* pdir = 0;
    struct dirent* pdirent;
    if((pdir = opendir(path.c_str())) == 0)
        return out_coll.size();

    while((pdirent = readdir(pdir)) != 0)
    {
        std::string name = std::string(pdirent->d_name);
        if ("." == name || ".." == name)
            continue;

        std::string find_file = path + "/" + name;
        if (!pfilter)
        {
            out_coll.push_back(find_file);
        }
        else
        {
            if (pfilter->filter(find_file))
                out_coll.push_back(find_file);
        }
    }
    ::closedir(pdir);
#endif

    return out_coll.size();
}

size_t listFiles(const std::string& path, std::vector<std::string>& out_vec, PathFilter* pfilter)
{ return listFiles< std::vector<std::string> >(path, out_vec, pfilter); }

size_t listFiles(const std::string& path, std::list<std::string>& out_list, PathFilter* pfilter)
{ return listFiles< std::list<std::string> >(path, out_list, pfilter); }

///-------- desprecated -------//
void findFilesInDir(const std::string& dir, std::vector<std::string>& out_vec, const std::string& ext_filter)
{
    FileFilter ff(ext_filter);
    listFiles(dir, out_vec, &ff);
}

void findFilesInDir(const std::string& dir, std::list<std::string>& out_list, const std::string& ext_filter)
{
    FileFilter ff(ext_filter);
    listFiles(dir, out_list, &ff);
}

void findPathInDir(const std::string& dir, std::vector<std::string>& out_vec)
{
    DirFilter df;
    listFiles(dir, out_vec, &df);
}

void findPathInDir(const std::string& dir, std::list<std::string>& out_list)
{
    DirFilter df;
    listFiles(dir, out_list, &df);
}

void findFilesInDirRecursively(const std::string& dir, std::vector<std::string>& out_vec, const std::string& ext_filter)
{
    FileFilterRecursive< vector<string> > ffr(out_vec, ext_filter);
    listFiles(dir, out_vec, &ffr);
}

void findFilesInDirRecursively(const std::string& dir, std::list<std::string>& out_list, const std::string& ext_filter)
{
    FileFilterRecursive< list<string> > ffr(out_list, ext_filter);
    listFiles(dir, out_list, &ffr);
}
///---------------------------//

} // namespace util
