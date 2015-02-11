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

    size_t rows() const;
    size_t cols() const;
//    void set(const std::vector<value_type>& values);

    value_type determinant() const;
//    bool invertible() const;
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

}

#endif // UTIL_MATRIX_H
