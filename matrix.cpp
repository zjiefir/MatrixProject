#include "Matrix.h"
#include <iostream>
#include <cmath>
#include <thread>
#include <future> 
#include <vector> 
#include <algorithm>

Matrix::Matrix() : rows(0), cols(0), data(nullptr) {
    std::cout << "Default constructor called\n";
}

Matrix::Matrix(int r, int c) : rows(r), cols(c) {
    if (r <= 0 || c <= 0) {
        throw std::invalid_argument("Matrix dimensions must be positive");
    }
    data = new double*[rows];
    for (int i = 0; i < rows; ++i) {
        data[i] = new double[cols](); 
    }
    std::cout << "Constructor with parameters called\n";
}

Matrix::Matrix(int r, int c, double value) : rows(r), cols(c) {
    if (r <= 0 || c <= 0) {
        throw std::invalid_argument("Matrix dimensions must be positive");
    }
    data = new double*[rows];
    for (int i = 0; i < rows; ++i) {
        data[i] = new double[cols];
        for (int j = 0; j < cols; ++j) {
            data[i][j] = value;
        }
    }
    std::cout << "Constructor with parameters and value called\n";
}

Matrix::Matrix(const Matrix& other) : rows(other.rows), cols(other.cols) {
    data = new double*[rows];
    for (int i = 0; i < rows; ++i) {
        data[i] = new double[cols];
        // Copy elements
        for (int j = 0; j < cols; ++j) {
            data[i][j] = other.data[i][j];
        }
    }
    std::cout << "Copy constructor called\n";
}

Matrix::~Matrix() {
    if (data != nullptr) {
        for (int i = 0; i < rows; ++i) {
            delete[] data[i];
        }
        delete[] data; 
    }
    std::cout << "Destructor called\n";
}

Matrix& Matrix::operator=(const Matrix& other) {
    if (this != &other) { 
        if (data != nullptr) {
            for (int i = 0; i < rows; ++i) {
                delete[] data[i];
            }
            delete[] data;
        }
        rows = other.rows;
        cols = other.cols;
        data = new double*[rows];
        for (int i = 0; i < rows; ++i) {
            data[i] = new double[cols];
            // Copy elements
            for (int j = 0; j < cols; ++j) {
                data[i][j] = other.data[i][j];
            }
        }
    }
    std::cout << "Assignment operator called\n";
    return *this;
}

double& Matrix::operator()(int i, int j) {
    if (i < 0 || i >= rows || j < 0 || j >= cols) {
        throw std::out_of_range("Index out of range");
    }
    return data[i][j];
}

const double& Matrix::operator()(int i, int j) const {
    if (i < 0 || i >= rows || j < 0 || j >= cols) {
        throw std::out_of_range("Index out of range");
    }
    return data[i][j];
}

double* Matrix::operator[](int i) {
    if (i < 0 || i >= rows) {
        throw std::out_of_range("Row index out of range");
    }
    return data[i];
}

const double* Matrix::operator[](int i) const {
    if (i < 0 || i >= rows) {
        throw std::out_of_range("Row index out of range");
    }
    return data[i];
}

Matrix Matrix::operator+(const Matrix& other) const {
    if (rows != other.rows || cols != other.cols) {
        throw std::invalid_argument("Matrix dimensions must match for addition");
    }
    Matrix result(rows, cols);
    size_t num_threads = std::thread::hardware_concurrency();
    if (num_threads == 0) num_threads = 1;
    std::vector<std::future<void>> futures;
    int rows_per_thread = rows / static_cast<int>(num_threads);
    for (size_t t = 0; t < num_threads; ++t) {
        int start_row = static_cast<int>(t) * rows_per_thread;
        int end_row = (t == num_threads - 1) ? rows : start_row + rows_per_thread;
        futures.push_back(std::async(std::launch::async, [&result, this, &other, start_row, end_row]() {
            for (int i = start_row; i < end_row; ++i) {
                for (int j = 0; j < this->cols; ++j) {
                    result[i][j] = (*this)[i][j] + other[i][j];
                }
            }
        }));
    }
    for (auto& f : futures) f.wait();
    return result;
}

Matrix Matrix::operator*(const Matrix& other) const {
    if (cols != other.rows) {
        throw std::invalid_argument("Number of columns of first matrix must equal number of rows of second matrix");
    }
    Matrix result(rows, other.getCols());
    size_t num_threads = std::thread::hardware_concurrency();
    if (num_threads == 0) num_threads = 1;
    std::vector<std::future<void>> futures;
    int rows_per_thread = rows / static_cast<int>(num_threads);
    for (size_t t = 0; t < num_threads; ++t) {
        int start_row = static_cast<int>(t) * rows_per_thread;
        int end_row = (t == num_threads - 1) ? rows : start_row + rows_per_thread;
        futures.push_back(std::async(std::launch::async, [&result, this, &other, start_row, end_row]() {
            for (int i = start_row; i < end_row; ++i) {
                for (int j = 0; j < other.getCols(); ++j) {
                    result[i][j] = 0.0;
                    for (int k = 0; k < this->cols; ++k) {
                        result[i][j] += (*this)[i][k] * other[k][j];
                    }
                }
            }
        }));
    }
    for (auto& f : futures) f.wait();
    return result;
}

Matrix Matrix::operator*(double scalar) const {
    Matrix result(rows, cols);
    size_t num_threads = std::thread::hardware_concurrency();
    if (num_threads == 0) num_threads = 1;
    std::vector<std::future<void>> futures;
    int rows_per_thread = rows / static_cast<int>(num_threads);
    for (size_t t = 0; t < num_threads; ++t) {
        int start_row = static_cast<int>(t) * rows_per_thread;
        int end_row = (t == num_threads - 1) ? rows : start_row + rows_per_thread;
        futures.push_back(std::async(std::launch::async, [&result, this, scalar, start_row, end_row]() {
            for (int i = start_row; i < end_row; ++i) {
                for (int j = 0; j < this->cols; ++j) {
                    result[i][j] = (*this)[i][j] * scalar;
                }
            }
        }));
    }
    for (auto& f : futures) f.wait();
    return result;
}

Matrix operator*(double scalar, const Matrix& m) {
    return m * scalar; 
}

Matrix Matrix::transpose() const {
    Matrix result(cols, rows);
    size_t num_threads = std::thread::hardware_concurrency();
    if (num_threads == 0) num_threads = 1;
    std::vector<std::future<void>> futures;
    int cols_per_thread = cols / static_cast<int>(num_threads);
    for (size_t t = 0; t < num_threads; ++t) {
        int start_col = static_cast<int>(t) * cols_per_thread;
        int end_col = (t == num_threads - 1) ? cols : start_col + cols_per_thread;
        futures.push_back(std::async(std::launch::async, [&result, this, start_col, end_col]() {
            for (int j = start_col; j < end_col; ++j) {
                for (int i = 0; i < this->rows; ++i) {
                    result[j][i] = (*this)[i][j];
                }
            }
        }));
    }
    for (auto& f : futures) f.wait();
    return result;
}

double Matrix::determinant() const {
    if (rows != cols) {
        throw std::invalid_argument("Matrix must be square for determinant calculation");
    }
    int n = rows;
    Matrix copy = *this; 
    double det = 1.0;
    int sign_changes = 0;

    for (int p = 0; p < n; ++p) {
        int pivot = p;
        for (int i = p + 1; i < n; ++i) {
            if (std::abs(copy[i][p]) > std::abs(copy[pivot][p])) {
                pivot = i;
            }
        }
        if (pivot != p) {
            std::swap(copy.data[p], copy.data[pivot]);
            sign_changes++;
        }

        if (std::abs(copy[p][p]) < 1e-10) {
            return 0.0;
        }
        det *= copy[p][p];

        double div = copy[p][p];
        for (int j = p; j < n; ++j) {
            copy[p][j] /= div;
        }

        size_t num_threads = std::thread::hardware_concurrency();
        if (num_threads == 0) num_threads = 1;
        std::vector<std::future<void>> futures;
        int rows_below_per_thread = (n - p - 1) / static_cast<int>(num_threads);
        for (size_t t = 0; t < num_threads; ++t) {
            int start_i = p + 1 + static_cast<int>(t) * rows_below_per_thread;
            int end_i = (t == num_threads - 1) ? n : start_i + rows_below_per_thread;
            if (start_i >= n) continue;
            futures.push_back(std::async(std::launch::async, [&copy, p, start_i, end_i, n]() {
                for (int i = start_i; i < end_i; ++i) {
                    double factor = copy[i][p];
                    for (int j = p; j < n; ++j) {
                        copy[i][j] -= factor * copy[p][j];
                    }
                }
            }));
        }
        for (auto& f : futures) f.wait();
    }

    return det * (sign_changes % 2 == 0 ? 1.0 : -1.0);
}

Matrix Matrix::inverse() const {
    if (rows != cols) {
        throw std::invalid_argument("Matrix must be square for inverse calculation");
    }
    int n = rows;
    Matrix augmented(n, 2 * n); // [A | I]

    // Copy A and I
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            augmented[i][j] = (*this)[i][j];
        }
        augmented[i][i + n] = 1.0;
    }

    for (int p = 0; p < n; ++p) {
        // Pivot
        int pivot = p;
        for (int i = p + 1; i < n; ++i) {
            if (std::abs(augmented[i][p]) > std::abs(augmented[pivot][p])) {
                pivot = i;
            }
        }
        if (pivot != p) {
            std::swap(augmented.data[p], augmented.data[pivot]);
        }

        if (std::abs(augmented[p][p]) < 1e-10) {
            throw std::runtime_error("Matrix is singular, inverse does not exist");
        }

        double div = augmented[p][p];
        size_t num_threads_norm = std::thread::hardware_concurrency();
        if (num_threads_norm == 0) num_threads_norm = 1;
        std::vector<std::future<void>> futures_norm;
        int cols_per_thread = (2 * n) / static_cast<int>(num_threads_norm);
        for (size_t t = 0; t < num_threads_norm; ++t) {
            int start_j = static_cast<int>(t) * cols_per_thread;
            int end_j = (t == num_threads_norm - 1) ? 2 * n : start_j + cols_per_thread;
            futures_norm.push_back(std::async(std::launch::async, [&augmented, p, div, start_j, end_j]() {
                for (int j = start_j; j < end_j; ++j) {
                    augmented[p][j] /= div;
                }
            }));
        }
        for (auto& f : futures_norm) f.wait();

        size_t num_threads = std::thread::hardware_concurrency();
        if (num_threads == 0) num_threads = 1;
        std::vector<std::future<void>> futures;
        int rows_per_thread = n / static_cast<int>(num_threads);
        for (size_t t = 0; t < num_threads; ++t) {
            int start_i = static_cast<int>(t) * rows_per_thread;
            int end_i = (t == num_threads - 1) ? n : start_i + rows_per_thread;
            futures.push_back(std::async(std::launch::async, [&augmented, p, start_i, end_i, n]() {
                for (int i = start_i; i < end_i; ++i) {
                    if (i == p) continue;
                    double factor = augmented[i][p];
                    for (int j = 0; j < 2 * n; ++j) {
                        augmented[i][j] -= factor * augmented[p][j];
                    }
                }
            }));
        }
        for (auto& f : futures) f.wait();
    }

    Matrix inv(n, n);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            inv[i][j] = augmented[i][j + n];
        }
    }
    return inv;
}

void Matrix::print() const {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            std::cout << data[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

int Matrix::getRows() const {
    return rows;
}

int Matrix::getCols() const {
    return cols;
}