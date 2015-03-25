#ifndef UTIL_TRACE_LISTENER_FILELISTENER_HPP
#define UTIL_TRACE_LISTENER_FILELISTENER_HPP

#include <fstream>
#include "base.hpp"
#include "../information.hpp"

namespace util
{

class FileListener : public InformationListener
{
public:
    FileListener();
    virtual ~FileListener();
    virtual void process(Information* pinfo);
    bool init(const std::string& filename, size_t file_info_count, const std::string& info_append = "");
    void close();
private:
    std::string filename_;
    std::string cur_filename_;
    size_t cur_file_info_count_;
    size_t max_file_info_count_;
    std::ofstream ofs;
    std::string info_append_;
private:
    DISALLOW_COPY_AND_ASSIGN(FileListener)
};

} // util

#endif // UTIL_TRACE_LISTENER_FILELISTENER_HPP
