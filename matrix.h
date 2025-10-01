#ifndef MATRIX_H
#define MATRIX_H

#include <stdexcept>

class Matrix {
private:
    int rows;    // Количество строк
    int cols;    // Количество столбцов
    double** data; // Указатель на двумерный массив

public:
    // Конструктор по умолчанию
    Matrix();

    // Конструктор с параметрами (инициализация нулями)
    Matrix(int r, int c);

    // Конструктор с параметрами и значением для всех элементов
    Matrix(int r, int c, double value);

    // Конструктор копирования
    Matrix(const Matrix& other);

    // Деструктор
    ~Matrix();

    // Оператор присваивания
    Matrix& operator=(const Matrix& other);

    // Оператор доступа к элементу (чтение/запись) через ()
    double& operator()(int i, int j);

    // Оператор доступа к элементу (только чтение) через ()
    const double& operator()(int i, int j) const;

    // Оператор доступа к строке (чтение/запись) через []
    double* operator[](int i);

    // Оператор доступа к строке (только чтение) через []
    const double* operator[](int i) const;

    // Оператор сложения
    Matrix operator+(const Matrix& other) const;

    // Оператор умножения на другую матрицу
    Matrix operator*(const Matrix& other) const;

    // Оператор умножения на скаляр (матрица * число)
    Matrix operator*(double scalar) const;

    // Свободная функция для умножения скаляра слева (число * матрица)
    friend Matrix operator*(double scalar, const Matrix& m);

    // Метод транспонирования матрицы
    Matrix transpose() const;

    // Метод вычисления обратной матрицы
    Matrix inverse() const;

    // Метод вычисления детерминанта
    double determinant() const;

    // Метод для вывода матрицы
    void print() const;

    // Получение размеров матрицы
    int getRows() const;
    int getCols() const;
};

#endif // MATRIX_H