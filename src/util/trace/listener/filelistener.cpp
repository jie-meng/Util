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
        DateTime dt = DateTime::now();
        cur_filename_ = filename_ + strFormat(" %04d-%02d-%02d %02d-%02d-%02d.log",
                            dt.getYear(),
                            dt.getMonth(),
                            dt.getDay(),
                            dt.getHour(),
                            dt.getMinute(),
                            dt.getSecond());
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

    DateTime dt = DateTime::now();
    cur_filename_ = filename_ + strFormat(" %04d-%02d-%02d %02d-%02d-%02d.log",
                            dt.getYear(),
                            dt.getMonth(),
                            dt.getDay(),
                            dt.getHour(),
                            dt.getMinute(),
                            dt.getSecond());
    cur_file_info_count_ = 0;

    ofs.open(cur_filename_.c_str());
    return ofs.is_open();
}

void FileListener::close()
{
    ofs.close();
}

} // util
