#include "filelistener.hpp"

namespace util
{

using namespace std;

FileListener::FileListener() :
    InformationListener(),
    cur_file_info_count_(0),
    max_file_info_count_(50000)
{
    //ctor
}

FileListener::~FileListener()
{
    //dtor
    ofs.close();
}

void FileListener::process(Information* pinfo)
{
    if (cur_file_info_count_ >= max_file_info_count_)
    {
        tm time = localTime();
        cur_filename_ = filename_ + strFormat(" %04d-%02d-%02d %02d-%02d-%02d.log",
                            time.tm_year + 1900,
                            time.tm_mon + 1,
                            time.tm_mday,
                            time.tm_hour,
                            time.tm_min,
                            time.tm_sec);
        cur_file_info_count_ = 0;
        ofs.close();
        ofs.open(cur_filename_.c_str());
    }

    ofs << pinfo->getFormatedInformation() + info_append_;
    ++cur_file_info_count_;
}

bool FileListener::init(const std::string& filename, size_t file_info_count, const std::string& info_append)
{
    filename_ = filename;
    max_file_info_count_ = file_info_count;
    info_append_ = info_append;

    tm time = localTime();
    cur_filename_ = filename_ + strFormat(" %04d-%02d-%02d %02d-%02d-%02d.log",
                            time.tm_year + 1900,
                            time.tm_mon + 1,
                            time.tm_mday,
                            time.tm_hour,
                            time.tm_min,
                            time.tm_sec);
    cur_file_info_count_ = 0;

    ofs.open(cur_filename_.c_str());
    return ofs.is_open();
}

void FileListener::close()
{
    ofs.close();
}

} // util
