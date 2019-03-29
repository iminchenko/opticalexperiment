#pragma once

#include <complex>
#include <vector>

template<class T>
class Matrix {
public:
    Matrix();
    Matrix(size_t rows, size_t columns);
    Matrix(size_t rows, size_t columns, T element);
    Matrix(Matrix<T> &&m) noexcept;
    Matrix(const Matrix<T> &m) = default;

    ~Matrix<T>() = default;

    size_t  rows() const;
    size_t columns() const;
    void setMatrix(const std::vector<std::vector<T>>& matrix);
    void removeAt(size_t index);
    void insert(size_t index);
    void resize(size_t size);

    std::vector<std::vector<T>>& getMatrix();

    std::vector<T>& operator[](size_t index);
    const std::vector<T>& operator[](size_t index) const;

    Matrix<T>& operator=(const Matrix<T> &m);

private:
    size_t _rows;
    size_t _columns;
    std::vector<std::vector<T>> _matrix;
};

template<class T>
Matrix<T>::Matrix() : 
    _rows(0), 
    _columns(0)
{}

template<class T>
Matrix<T>::Matrix(size_t rows, size_t columns):
    _rows(rows),
    _columns(columns),
    _matrix(_rows, std::vector<T>(_columns)) 
{}

template<class T>
Matrix<T>::Matrix(size_t rows, size_t columns, T element) :
    _rows(rows), 
    _columns(columns) {
    for (size_t row = 0; row < rows; ++row) {
        _matrix.push_back(std::vector<T>());
        for (size_t column = 0; column < columns; ++column)
            _matrix[row].push_back(element);
    }
}

template<class T>
Matrix<T>::Matrix(Matrix<T>&& m) noexcept :
    _rows(m.rows()),
    _columns(m.columns()),
    _matrix(m.getMatrix()) {
    m._rows = 0;
    m._columns = 0;
}

template<class T>
void Matrix<T>::setMatrix(const std::vector<std::vector<T>>& matrix) {
    _matrix = matrix;
    _rows = matrix.size();
    _rows > 0 ? _columns = matrix[0].size() : _columns = 0;
}

template<class T>
void Matrix<T>::removeAt(size_t index) {
    if (index > _rows || index > _columns)
        throw "Не верный индекс";

    _matrix.erase(_matrix.begin() + index);
}

template<class T>
void Matrix<T>::resize(size_t size) {
    if (_rows < size) {
        _matrix.reserve((size_t)size);
        for (int i = size - _rows; i > 0; i--)
            _matrix.push_back(std::vector<T>(size, 0));

        for (int i = 0; i < _rows; i++) {
            _matrix[i].resize(size);
            for (int j = 0; j < size; j++)
                _matrix[i][j] = 0;
        }

        _rows = _columns = (int)_matrix.size();
    } else if (_rows > size) {
        _matrix.resize((size_t)size);
        _rows = size;
        
        for (int i = 0; i < _rows; i++)
            _matrix[i].resize(size);
        _columns = size;
    }
}

template<class T>
std::vector<std::vector<T>>& Matrix<T>::getMatrix() {
    return _matrix;
}

template<class T>
size_t Matrix<T>::columns() const {
   return _columns;
}

template<class T>
size_t Matrix<T>::rows() const {
    return _rows;
}

template<class T>
std::vector<T> &Matrix<T>::operator[](size_t index) {
    if (index < 0 || index > _rows)
        throw "Выход за границы матрицы";

    return _matrix[index];
}

template<class T>
const std::vector<T> &Matrix<T>::operator[](size_t index) const {
    if (index < 0 || index > _rows)
        throw "Выход за границы матрицы";

    return _matrix[index];
}

template<class T>
Matrix<T> &Matrix<T>::operator=(const Matrix<T> &m) {
    if (this != &m) {
        _matrix = m._matrix;
        _rows = m._rows;
        _columns = m._columns;
    }

    return *this;
}

template<class T>
Matrix<T> operator +(const Matrix<T>& m1, const Matrix<T>& m2) {
    if (m1._rows != m2._rows && m1._columns != m2._columns)
        throw "Матрицы разного размера";

    Matrix<T> newM = Matrix<T>(m1._rows, m2._columns);
    for (int row = 0; row < m1._rows; row++)
        for (int column = 0; column < m1._columns; column++)
            newM[row][column] = m1[row][column] + m2[row][column];

    return newM;
}

template<class T>
Matrix<T> operator -(const Matrix<T>& m1, const Matrix<T>& m2) {
    if (m1._rows != m2._rows && m1._columns != m2._columns)
        throw "Матрицы разного размера";

    Matrix<T> newM = Matrix<T>(m1._rows, m2._columns);
    for (int row = 0; row < m1._rows; row++)
        for (int column = 0; column < m1._columns; column++)
            newM[row][column] = m1[row][column] - m2[row][column];

    return newM;
}

template<class T>
Matrix<T> operator *(const Matrix<T>& m1, const Matrix<T>& m2) {
    if (m1._rows != m2._rows && m1._columns != m2._columns)
        throw "Матрицы разного размера";

    Matrix<T> newM = Matrix<T>(m1._rows, m2._columns);
    for (int row = 0; row < m1._rows; row++)
        for (int column = 0; column < m1._columns; column++)
            for (int count = 0; count < m1._columns; count++)
                newM[row][column] += m1[row][count] * m2[count][column];

    return newM;
}

template<class T>
bool operator ==(const Matrix<T>& m1, const Matrix<T>& m2) {
    if (m1._rows != m2._rows && m1._columns != m2._columns)
        return false;

    for (int row = 0; row < m1._rows; row++)
        for (int column = 0; column < m1._columns; column++)
            if (m1[row][column] != m2[row][column])
                return false;
    
    return true;
}

template<class T>
bool operator !=(const Matrix<T>& m1, const Matrix<T>& m2) {
    return !(m1 == m2);
}
