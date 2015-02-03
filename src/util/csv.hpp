#ifndef UTIL_CSV_HPP
#define UTIL_CSV_HPP

#include "base.hpp"

namespace util
{
class Csv
{
public:
    Csv();
    explicit Csv(const std::string& file);
    ~Csv();
    bool read(const std::string& file);
    bool write(const std::string& file);
    bool write();
    size_t getTotalRows() const;
    size_t getTotalCols() const;
private:
    struct CsvImpl;
    UtilAutoPtr<CsvImpl> pimpl_;
};

} //namespace util

#endif // UTIL_CSV_HPP
