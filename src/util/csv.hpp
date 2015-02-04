#ifndef UTIL_CSV_HPP
#define UTIL_CSV_HPP

#include "base.hpp"
#include <vector>

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
    void clear();
    bool empty() const;
    size_t getTotalRows() const;
    size_t getTotalCols() const;
    std::string getCellValue(size_t row, size_t col) const;
    bool setCellValue(size_t row, size_t col, const std::string& value);

    template<typename Iter>
    bool addRow(Iter first, Iter last);

    bool addRow(const std::vector<std::string>& vec);
private:
    struct CsvImpl;
    UtilAutoPtr<CsvImpl> pimpl_;
private:
    DISALLOW_COPY_AND_ASSIGN(Csv)
};

template <typename Iter>
bool Csv::addRow(Iter first, Iter last)
{
    int len = std::distance(first, last);
    if (len <= 0)
        return false;

    std::vector<std::string> vec;
    for (Iter it = first; it != last; ++it)
        vec.push_back(toString(*it));

    return addRow(vec);
}

} //namespace util

#endif // UTIL_CSV_HPP
