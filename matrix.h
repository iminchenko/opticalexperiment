#ifndef MATRIX_H
#define MATRIX_H

#include <complex>
#include <vector>
#include "deviceconfigs/wave.h"

template<class T>
class Matrix
{
public:
    Matrix<T>() : rows_(0), columns_(0) {}
    Matrix<T>(int rows, int columns, T element);
    Matrix<T>(Matrix<T>&& m);
    Matrix<T>(const Matrix<T>& m);

    ~Matrix<T>() { }

    int getRows() const;
    int getColumns() const;
    void setMatrix(const std::vector<std::vector<T>>& m);
    void removeAt(int index);
    void insert(int index);
    void resize(int size);

    std::vector<std::vector<T>>& getMatrix();

    std::vector<T>& operator [](int index)
    {
        if (index < 0 || index > rows_)
            throw "Выход за границы матрицы";

        return matrix_[index];
    }

    /* Добавить реализацию */
    Matrix<T>& operator =(const Matrix<T>& m)
    {
        if (this != &m)
        {
            matrix_ = m.matrix_;
            rows_ = m.rows_;
            columns_ = m.columns_;
        }

        return *this;
    }

private:
    std::vector<std::vector<T>> matrix_;
    int rows_, columns_;
};

template<class T>
Matrix<T>::Matrix(int rows, int columns, T element)
        : rows_(rows), columns_(columns)
{
    for (int row = 0; row < rows; row++)
    {
        matrix_.push_back(std::vector<T>());
        for (int column = 0; column < columns; column++)
            matrix_[row].push_back(element);
    }
}

template<class T>
Matrix<T>::Matrix(Matrix<T>&& m)
    :matrix_(std::move(m.getMatrix()))
{
    m.rows_ = 0;
    m.columns_ = 0;
}

template<class T>
void Matrix<T>::setMatrix(const std::vector<std::vector<T>>& matrix)
{
    matrix_ = matrix;
    rows_ = matrix.size();
    rows_ > 0 ? columns_ = matrix[0].size() : columns_ = 0;
}

template<class T>
void Matrix<T>::removeAt(int index)
{
    if (index > rows_ || index > columns_)
        throw "Не верный индекс";

    matrix_.removeAt(index);
    for (int i = 0; i < matrix_.size(); i++)
        matrix_[i].removeAt(index);
}

/* Добавить реализацию */
template<class T>
void Matrix<T>::insert(int index)
{

}

template<class T>
void Matrix<T>::resize(int size)
{
    if (rows_ < size)
    {
        matrix_.reserve(size);
        for (int i = size - rows_; i > 0; i--)
            matrix_.push_back(std::vector<T>(size, 0));

        for (int i = 0; i < rows_; i++)
        {
            matrix_[i].resize(size);
            for (int j = 0; j < size; j++)
                matrix_[i][j] = 0;
        }

        rows_ = columns_ = matrix_.size();
    }
    else if (rows_ > size)
    {
        matrix_.resize(size);
        rows_ = size;
        for (int i = 0; i < rows_; i++)
            matrix_[i].resize(size);
        columns_ = size;
    }
}

template<class T>
std::vector<std::vector<T>>& Matrix<T>::getMatrix()
{
    return matrix_;
}

template<class T>
int Matrix<T>::getColumns() const
{
   return columns_;
}

template<class T>
int Matrix<T>::getRows() const
{
    return rows_;
}

template<class T>
Wave operator *(const Matrix<T>& m, const Wave& w)
{
    /* В волне всегда два компонента*/
    if (m.columns != 2)
        throw "Количество столбцов матрицы не равно 2";

    Wave newW;

    newW.setEx(m[0][0]*w.getEx() + m[0][1]*w.getEy());
    newW.getEy() = m[1][0]*w.getEx() + m[1][1]*w.getEy();

    return newW;
}

/* Добавить реализацию*/
template<class T>
std::vector<Wave> operator *(const Matrix<T>& m, const std::vector<Wave>& ws)
{
    if (m.columns != ws.size()*2)
        throw "Умножение не возможно";
}

template<class T>
Matrix<T> operator +(const Matrix<T>& m1, const Matrix<T>& m2)
{
    if (m1.rows != m2.rows && m1.columns != m2.columns)
        throw "Матрицы разного размера";

    Matrix<T> newM = Matrix<T>(m1.rows, m2.columns);
    for (int row = 0; row < m1.rows; row++)
        for (int column = 0; column < m1.columns; column++)
            newM[row][column] = m1[row][column] + m2[row][column];

    return newM;
}

template<class T>
Matrix<T> operator -(const Matrix<T>& m1, const Matrix<T>& m2)
{
    if (m1.rows != m2.rows && m1.columns != m2.columns)
        throw "Матрицы разного размера";

    Matrix<T> newM = Matrix<T>(m1.rows, m2.columns);
    for (int row = 0; row < m1.rows; row++)
        for (int column = 0; column < m1.columns; column++)
            newM[row][column] = m1[row][column] - m2[row][column];

    return newM;
}

template<class T>
Matrix<T> operator *(const Matrix<T>& m1, const Matrix<T>& m2)
{
    if (m1.rows != m2.rows && m1.columns != m2.columns)
        throw "Матрицы разного размера";

    Matrix<T> newM = Matrix<T>(m1.rows, m2.columns);
    for (int row = 0; row < m1.rows; row++)
        for (int column = 0; column < m1.columns; column++)
            for (int count = 0; count < m1.columns; count++)
                newM[row][column] += m1[row][count] * m2[count][column];

    return newM;
}

template<class T>
bool operator ==(const Matrix<T>& m1, const Matrix<T>& m2)
{
    if (m1.rows != m2.rows && m1.columns != m2.columns)
        return false;

    for (int row = 0; row < m1.rows; row++)
        for (int column = 0; column < m1.columns; column++)
            if (m1[row][column] != m2[row][column])
                return false;
    return true;
}

template<class T>
Matrix<T> operator !=(const Matrix<T>& m1, const Matrix<T>& m2)
{
    return !(m1 == m2);
}

#endif // MATRIX_H
