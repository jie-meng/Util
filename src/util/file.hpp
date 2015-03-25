#ifndef UTIL_FILE_HPP
#define UTIL_FILE_HPP

#include "base.hpp"
#include <vector>
#include <list>
#include <ctime>

namespace util
{

//file read & write
std::string readTextFile(const std::string& file);
bool writeTextFile(const std::string& file, const std::string& content, bool append = false);
bool createBinaryFile(const std::string& file, uint64_t file_size = 0, char default_data = 0);
bool readBinaryFile(const std::string& file, char* pout_buf, size_t read_len, std::streamoff offset = 0, std::ios::seekdir pos = std::ios_base::beg);
bool writeBinaryFile(const std::string& file, char* pbuf, size_t write_len, bool append = false);
bool overwriteBinaryFile(const std::string& file, char* pbuf, size_t write_len, std::streamoff offset = 0, std::ios::seekdir pos = std::ios_base::beg);

//file info
std::string fileExtension(const std::string& file);
std::string fileBaseName(const std::string& file);
bool fileRename(const std::string& src_path, const std::string& dest_path);
bool fileCopy(const std::string& src_path, const std::string& dest_path, bool fail_if_exitst = true);
std::pair<std::string, std::string> splitPathname(const std::string& str);

//file time
enum E_FileTime
{
    FtCreationTime = 0,
    FtLastWriteTime,
    FtLastAccessTime
};

time_t fileTime(const std::string& file, E_FileTime ft);

//path
std::string currentPath();
bool setCurrentPath(const std::string& dir);
bool isPathExists(const std::string& path);
bool isPathDir(const std::string& path);
bool isPathFile(const std::string& path);
bool isPathEmpty(const std::string& path);
void pathRemoveAll(const std::string& path);
bool pathRemove(const std::string& path);
bool mkDir(const std::string& path);

class PathFilter : public Interface
{
public:
    virtual bool filter(const std::string& path) = 0;
};

class FileFilter : public PathFilter
{
public:
    explicit FileFilter(const std::string& ext = "") :
        ext_(ext)
    {}

    virtual bool filter(const std::string& path)
    {
        if (isPathFile(path))
        {
            if ("" == ext_)
            {
                return true;
            }
            else
            {
                if (strIsEqual(fileExtension(path), ext_, false))
                    return true;
                else
                    return false;
            }
        }

        return false;
    }
private:
    std::string ext_;
};

class DirFilter : public PathFilter
{
public:
    virtual bool filter(const std::string& path)
    {
        if (isPathDir(path))
            return true;
        return false;
    }
};

template <class Coll>
class FileFilterRecursive : public PathFilter
{
public:
    explicit FileFilterRecursive(Coll& c, const std::string& ext = "") :
        coll_(c),
        ext_(ext)
    {}

    virtual bool filter(const std::string& path)
    {
        if (isPathFile(path))
        {
            if ("" == ext_)
            {
                return true;
            }
            else
            {
                if (strIsEqual(fileExtension(path), ext_, false))
                    return true;
                else
                    return false;
            }
        }
        else
        {
            listFiles(path, coll_, this);
            return false;
        }
    }
private:
    Coll& coll_;
    std::string ext_;
};

size_t listFiles(const std::string& path, std::vector<std::string>& out_vec, PathFilter* pfilter  = 0);
size_t listFiles(const std::string& path, std::list<std::string>& out_list, PathFilter* pfilter = 0);

///-------- desprecated -------//
void findFilesInDir(const std::string& dir, std::vector<std::string>& out_vec, const std::string& ext_filter = "");
void findFilesInDir(const std::string& dir, std::list<std::string>& out_list, const std::string& ext_filter = "");
void findPathInDir(const std::string& dir, std::vector<std::string>& out_vec);
void findPathInDir(const std::string& dir, std::list<std::string>& out_list);
void findFilesInDirRecursively(const std::string& dir, std::vector<std::string>& out_vec, const std::string& ext_filter = "");
void findFilesInDirRecursively(const std::string& dir, std::list<std::string>& out_list, const std::string& ext_filter = "");
///---------------------------//

} // namespace util


#endif // UTIL_FILE_HPP
