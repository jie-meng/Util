#ifndef UTIL_MATRIX_H
#define UTIL_MATRIX_H

#include "base.hpp"
#include <vector>

namespace util
{

class Matrix
{
public:
    typedef double value_type;

    enum MatrixType
    {
        Zero,
        Identity,
        Random,
        Ones
    };

    friend std::ostream& operator<<(std::ostream& out, const Matrix& m);

    Matrix();
    Matrix(size_t row, size_t col, MatrixType type = Zero);
    Matrix(const Matrix& m);
    ~Matrix();

    bool set(const std::vector<value_type>& values);

    template<typename Iter>
    bool set(Iter first, Iter last);

    size_t rows() const;
    size_t cols() const;

    inline bool empty() const { return rows() == 0 && cols() == 0; }
    inline bool square() const { return rows() == cols() && rows() > 0; }
    inline size_t elements() const { return rows() * cols(); }

    size_t rank() const;
    value_type determinant() const;
    bool invertable() const;
    Matrix inverse() const;
    Matrix transpose()const;
    Matrix conjugate() const;
    Matrix adjoint() const;

    Matrix& operator=(const Matrix& m);
    Matrix& operator+=(const Matrix& m);
    Matrix& operator-=(const Matrix& m);
    Matrix& operator*=(const Matrix& m);

    Matrix operator+(const Matrix& m);
    Matrix operator-(const Matrix& m);
    Matrix operator*(const Matrix& m);
    bool operator==(const Matrix& m);
    value_type& operator()(size_t i, size_t j);
private:
    struct MatrixImpl;
    UtilAutoPtr<MatrixImpl> pimpl_;
};

template <typename Iter>
bool Matrix::set(Iter first, Iter last)
{
    int len = std::distance(first, last);
    if (len <= 0)
        return false;

    std::vector<Matrix::value_type> vec;
    for (Iter it = first; it != last; ++it)
        vec.push_back(*it);

    return set(vec);
}

}

#endif // UTIL_MATRIX_H
