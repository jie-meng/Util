#include "matrix.hpp"
#include "Eigen/Dense"

namespace util
{

using namespace std;
using namespace Eigen;

typedef MatrixXd UMatrix;

struct Matrix::MatrixImpl
{
    //ctor & dtor
    MatrixImpl()
    {
    }

    MatrixImpl(size_t row, size_t col, MatrixType type)
    {
        switch (type)
        {
        case Zero:
            matrix_ = UMatrix::Zero(row, col);
            break;
        case Identity:
            matrix_ = UMatrix::Identity(row, col);
            break;
        case Random:
            matrix_ = UMatrix::Random(row, col);
            break;
        case Ones:
            matrix_ = UMatrix::Ones(row, col);
            break;
        default:
            matrix_ = UMatrix::Zero(row, col);
            break;
        }
    }

    MatrixImpl(const UMatrix& matrix) :
        matrix_(matrix)
    {
    }

    //field
    UMatrix matrix_;
};

Matrix::Matrix() :
    pimpl_(new MatrixImpl())
{
}

Matrix::Matrix(size_t row, size_t col, MatrixType type) :
    pimpl_(new MatrixImpl(row, col, type))
{
}

Matrix::Matrix(const Matrix& m) :
    pimpl_(new MatrixImpl())
{
    pimpl_->matrix_ = m.pimpl_->matrix_;
}

Matrix::~Matrix()
{
}

size_t Matrix::rows() const
{
    return pimpl_->matrix_.rows();
}

size_t Matrix::cols() const
{
    return pimpl_->matrix_.cols();
}

//void Matrix::set(const std::vector<value_type>& values)
//{
//    std::vector<value_type>::iterator it;
////    for (it = values.begin(); it != values.end(); ++it)
////    {
////
////    }
//}

Matrix Matrix::inverse() const
{
    Matrix m;
    m.pimpl_->matrix_ = pimpl_->matrix_.inverse();
    return m;
}

Matrix Matrix::transpose() const
{
    Matrix m;
    m.pimpl_->matrix_ = pimpl_->matrix_.transpose();
    return m;
}

Matrix Matrix::conjugate() const
{
    Matrix m;
    m.pimpl_->matrix_ = pimpl_->matrix_.conjugate();
    return m;
}

Matrix Matrix::adjoint() const
{
    Matrix m;
    m.pimpl_->matrix_ = pimpl_->matrix_.adjoint();
    return m;
}

Matrix& Matrix::operator=(const Matrix& m)
{
    if (this == &m)
        return *this;

    pimpl_->matrix_ = m.pimpl_->matrix_;
    return *this;
}

Matrix& Matrix::operator+=(const Matrix& m)
{
    pimpl_->matrix_ += m.pimpl_->matrix_;
    return *this;
}

Matrix& Matrix::operator-=(const Matrix& m)
{
    pimpl_->matrix_ -= m.pimpl_->matrix_;
    return *this;
}

Matrix& Matrix::operator*=(const Matrix& m)
{
    pimpl_->matrix_ *= m.pimpl_->matrix_;
    return *this;
}

bool Matrix::operator==(const Matrix& m)
{
    return pimpl_->matrix_ == m.pimpl_->matrix_;
}

Matrix Matrix::operator+(const Matrix& m)
{
    Matrix n(*this);
    return n += m;
}

Matrix Matrix::operator-(const Matrix& m)
{
    Matrix n(*this);
    n -= m;
    return n;
}

Matrix Matrix::operator*(const Matrix& m)
{
    Matrix n(*this);
    n *= m;
    return n;
}

Matrix::value_type& Matrix::operator()(size_t i, size_t j)
{
    return pimpl_->matrix_(i, j);
}

std::ostream& operator<<(std::ostream& out, const Matrix& m)
{
    out << m.pimpl_->matrix_;
    return out;
}

}
