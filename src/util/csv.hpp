#ifndef UTIL_CSV_HPP
#define UTIL_CSV_HPP

#include "base.hpp"

namespace util
{

class Csv
{
public:
    Csv(char delimeter = ',', char enclosure = '"');
    explicit Csv(const std::string& file, char delimeter = ',', char enclosure = '"');
    ~Csv();
    bool read(const std::string& file);
    bool write(const std::string& file);
    bool write();
    bool empty() const;
    size_t getTotalRows() const;
    size_t getTotalCols() const;
    std::string getCellValue(size_t row, size_t col) const;
    bool setCellValue(size_t row, size_t col, const std::string& value);
private:
    struct CsvImpl;
    UtilAutoPtr<CsvImpl> pimpl_;
};

} //namespace util

#endif // UTIL_CSV_HPP
