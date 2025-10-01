#include "Matrix.h"
#include <iostream>
#include <iomanip>

int main() {
    std::cout << std::fixed << std::setprecision(2);
    try {
        std::cout << "=== Test Constructors ===\n";
        Matrix empty;
        std::cout << "Empty matrix (size: " << empty.getRows() << "x" << empty.getCols() << "):\n";
        empty.print();
        Matrix m1(2, 3);
        for (int i = 0; i < m1.getRows(); ++i) {
            for (int j = 0; j < m1.getCols(); ++j) {
                m1[i][j] = i * 3.0 + j + 1.0;
            }
        }
        std::cout << "m1 (2x3):\n";
        m1.print();
        // Copy
        Matrix m_copy = m1;
        std::cout << "Copy m_copy (2x3):\n";
        m_copy.print();
        // 2. Access to elements
        std::cout << "\n=== Test Access to Elements ===\n";
        std::cout << "m1[0][1] = " << m1[0][1] << std::endl;
        m1[1][2] = 7.5;
        std::cout << "After changing m1[1][2] = 7.5:\n";
        m1.print();
        // 3. Addition
        std::cout << "\n=== Test Addition ===\n";
        Matrix m2(2, 3, 1.0);
        std::cout << "m2 (all 1.0):\n";
        m2.print();
        Matrix sum = m1 + m2;
        std::cout << "m1 + m2:\n";
        sum.print();
        // 4. Multiplication by scalar
        std::cout << "\n=== Test Multiplication by Scalar ===\n";
        Matrix scaled = m1 * 2.5;
        std::cout << "m1 * 2.5:\n";
        scaled.print();
        Matrix scaled_left = 2.5 * m1;
        std::cout << "2.5 * m1:\n";
        scaled_left.print();
        // 5. Matrix multiplication
        std::cout << "\n=== Test Matrix Multiplication ===\n";
        Matrix m3(3, 2);
        for (int i = 0; i < m3.getRows(); ++i) {
            for (int j = 0; j < m3.getCols(); ++j) {
                m3[i][j] = i * 2.0 + j + 1.0;
            }
        }
        std::cout << "m3 (3x2):\n";
        m3.print();
        Matrix prod = m1 * m3;
        std::cout << "m1 * m3 (2x2):\n";
        prod.print();
        // 6. Transposition
        std::cout << "\n=== Test Transposition ===\n";
        Matrix trans = m1.transpose();
        std::cout << "Transposed m1 (3x2):\n";
        trans.print();
        // 7. Determinant
        std::cout << "\n=== Test Determinant ===\n";
        Matrix sq(2, 2);
        sq[0][0] = 1.0; sq[0][1] = 2.0;
        sq[1][0] = 3.0; sq[1][1] = 4.0;
        std::cout << "Square 2x2:\n";
        sq.print();
        std::cout << "Determinant: " << sq.determinant() << std::endl;
        // 8. Inverse matrix
        std::cout << "\n=== Test Inverse Matrix ===\n";
        Matrix inv = sq.inverse();
        std::cout << "Inverse 2x2:\n";
        inv.print();
        // Check: A * inv(A) == I
        Matrix identity = sq * inv;
        std::cout << "sq * inv (identity matrix):\n";
        identity.print();
        // Degenerate test
        Matrix degenerate(2, 2);
        degenerate[0][0] = 1.0; degenerate[0][1] = 2.0;
        degenerate[1][0] = 2.0; degenerate[1][1] = 4.0;
        std::cout << "Determinant of degenerate: " << degenerate.determinant() << std::endl; // 0
        // 9. Exception test
        std::cout << "\n=== Test Exceptions ===\n";
        try {
            Matrix bad_sum(2, 2);
            Matrix bad_other(3, 2);
            Matrix bad_result = bad_sum + bad_other;
        } catch (const std::exception& e) {
            std::cout << "Addition exception: " << e.what() << std::endl;
        }
        try {
            Matrix bad_mult(2, 2);
            Matrix bad_other_mult(3, 3);
            Matrix bad_prod = bad_mult * bad_other_mult;
        } catch (const std::exception& e) {
            std::cout << "Multiplication exception: " << e.what() << std::endl;
        }
        try {
            Matrix bad_det(2, 3);
            double bad_d = bad_det.determinant();
        } catch (const std::exception& e) {
            std::cout << "Determinant exception: " << e.what() << std::endl;
        }
        try {
            Matrix bad_inv(2, 3);
            Matrix bad_i = bad_inv.inverse();
        } catch (const std::exception& e) {
            std::cout << "Inverse exception: " << e.what() << std::endl;
        }
        try {
            Matrix bad_access;
            bad_access[0][0] = 1.0;
        } catch (const std::exception& e) {
            std::cout << "Access exception: " << e.what() << std::endl;
        }
        std::cout << "\nAll tests completed successfully!\n";
    } catch (const std::exception& e) {
        std::cerr << "General error: " << e.what() << std::endl;
        return 1;
    }
    std::cin.get(); 
    return 0;
}