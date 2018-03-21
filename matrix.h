#ifndef MATRIX_H
#define MATRIX_H

#include <complex>
#include <vector>
#include "deviceconfigs/wave.h"

template<class T>
class Matrix
{
public:
    Matrix() : _rows(0), _columns(0) {}
    Matrix(int rows, int columns);
    Matrix(int rows, int columns, T element);
    Matrix(Matrix<T>&& m);
    Matrix(const Matrix<T>& m) = default;

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
        if (index < 0 || index > _rows)
            throw "Выход за границы матрицы";

        return matrix_[index];
    }

    Matrix<T>& operator =(const Matrix<T>& m)
    {
        if (this != &m)
        {
            matrix_ = m.matrix_;
            _rows = m._rows;
            _columns = m._columns;
        }

        return *this;
    }

private:
    std::vector<std::vector<T>> matrix_;
    int _rows, _columns;
};

template<class T>
Matrix<T>::Matrix(int rows, int columns)
        : _rows(rows), _columns(columns),
          matrix_(rows, std::vector<std::complex<double>>(columns)) {}

template<class T>
Matrix<T>::Matrix(int rows, int columns, T element)
        : _rows(rows), _columns(columns)
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
    m._rows = 0;
    m._columns = 0;
}

template<class T>
void Matrix<T>::setMatrix(const std::vector<std::vector<T>>& matrix)
{
    matrix_ = matrix;
    _rows = matrix.size();
    _rows > 0 ? _columns = matrix[0].size() : _columns = 0;
}

template<class T>
void Matrix<T>::removeAt(int index)
{
    if (index > _rows || index > _columns)
        throw "Не верный индекс";

    matrix_.erase(matrix_.begin() + index);
}

/* Добавить реализацию */
template<class T>
void Matrix<T>::insert(int index)
{

}

template<class T>
void Matrix<T>::resize(int size)
{
    if (_rows < size)
    {
        matrix_.reserve(size);
        for (int i = size - _rows; i > 0; i--)
            matrix_.push_back(std::vector<T>(size, 0));

        for (int i = 0; i < _rows; i++)
        {
            matrix_[i].resize(size);
            for (int j = 0; j < size; j++)
                matrix_[i][j] = 0;
        }

        _rows = _columns = matrix_.size();
    }
    else if (_rows > size)
    {
        matrix_.resize(size);
        _rows = size;
        for (int i = 0; i < _rows; i++)
            matrix_[i].resize(size);
        _columns = size;
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
   return _columns;
}

template<class T>
int Matrix<T>::getRows() const
{
    return _rows;
}

template<class T>
Wave operator *(const Matrix<T>& m, const Wave& w)
{
    /* В волне всегда два компонента*/
    if (m.getColumns() != 2)
        throw "Количество столбцов матрицы не равно 2";

    Wave newW;

    newW.setEx(m[0][0]*w.getEx() + m[0][1]*w.getEy());
    newW.setEy(m[1][0]*w.getEx() + m[1][1]*w.getEy());

    return newW;
}

/* Добавить реализацию*/
template<class T>
std::vector<Wave> operator *(const Matrix<T>& m, const std::vector<Wave>& ws)
{
    if (m.columns != ws.size()*2)
        throw "Умножение не возможно";
    
    std::vector<Wave> newWs(ws.size());
    for (int i = 0; i < m.getColumns(); ++i)
        for (int j = 0; j < ws.size(); ++j) {
            
        }
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
