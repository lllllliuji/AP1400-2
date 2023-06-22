#include "hw1.h"
#include <iomanip>
#include <ios>
#include <iostream>
#include <random>
#include <stdexcept>

namespace algebra {
Matrix zeros(size_t m, size_t n) {
    Matrix ans;
    ans.resize(m);
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            ans[i].push_back(0);
        }
    }
    return ans;
}

Matrix ones(size_t m, size_t n) {
    Matrix ans;
    ans.resize(m);
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            ans[i].push_back(1);
        }
    }
    return ans;
}

Matrix random(size_t m, size_t n, double min, double max) {
    if (min > max) {
        throw std::logic_error("min > max");
    }
    std::random_device rd;
    std::default_random_engine e(rd());
    std::uniform_real_distribution<double> u(min, max);
    Matrix ans;
    ans.resize(m);
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            ans[i].push_back(u(e));
        }
    }
    return ans;
}

void show(const Matrix &matrix) {
    for (int i = 0; i < matrix.size(); i++) {
        for (int j = 0; j < matrix[i].size(); j++) {
            std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(3) << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

Matrix multiply(const Matrix &matrix, double c) {
    auto ans = matrix;
    for (int i = 0; i < ans.size(); i++) {
        for (int j = 0; j < ans[i].size(); j++) {
            ans[i][j] *= c;
        }
    }
    return ans;
}

Matrix multiply(const Matrix &matrix1, const Matrix &matrix2) {
    if (matrix1.empty() || matrix2.empty()) {
        return {};
    }
    int row_1 = matrix1.size(), column_1 = matrix1[0].size();
    int row_2 = matrix2.size(), column_2 = matrix2[0].size();
    if (column_1 != row_2) {
        throw std::logic_error("does not match");
    }
    Matrix ans;
    ans.resize(row_1);
    for (int i = 0; i < row_1; i++) {
        for (int j = 0; j < column_2; j++) {
            double x = 0.0;
            for (int k = 0; k < column_1; k++) {
                x += matrix1[i][k] * matrix2[k][j];
            }
            ans[i].push_back(x);
        }
    }
    return ans;
}

Matrix sum(const Matrix &matrix, double c) {
    auto ans = matrix;
    for (int i = 0; i < matrix.size(); i++) {
        for (int j = 0; j < matrix[i].size(); j++) {
            ans[i][j] += c;
        }
    }
    return ans;
}

Matrix sum(const Matrix &matrix1, const Matrix &matrix2) {
    if (matrix1.empty() && matrix2.empty()) {
        return {};
    }
    if (matrix1.empty() || matrix2.empty()) {
        throw std::logic_error("does not match");
    }
    int row_1 = matrix1.size(), column_1 = matrix1[0].size();
    int row_2 = matrix2.size(), column_2 = matrix2[0].size();
    if (row_1 != row_2 || column_1 != column_2) {
        throw std::logic_error("does not match");
    }
    Matrix ans;
    ans.resize(row_1);
    for (int i = 0; i < row_1; i++) {
        for (int j = 0; j < column_1; j++) {
            ans[i].push_back(matrix1[i][j] + matrix2[i][j]);
        }
    }
    return ans;
}

Matrix transpose(const Matrix &matrix) {
    if (matrix.empty()) {
        return {};
    }
    int row = matrix.size(), column = matrix[0].size();
    Matrix ans;
    ans.resize(column);
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < column; j++) {
            ans[j].push_back(matrix[i][j]);
        }
    }
    return ans;
}

Matrix minor(const Matrix &matrix, size_t m, size_t n) {
    if (matrix.empty()) {
        return {};
    }
    int row = matrix.size(), column = matrix[0].size();
    if (m >= row && n >= column) {
        return matrix;
    }
    Matrix ans;
    for (int i = 0; i < row; i++) {
        if (i == m) continue;
        std::vector<double> tmp;
        for (int j = 0; j < column; j++) {
            if (j != n) {
                tmp.push_back(matrix[i][j]);
            }
        }
        ans.emplace_back(std::move(tmp));
    }
    return ans;
}

double determinant(const Matrix &matrix) {
    if (matrix.empty()) {
        return 1;
    }
    int row = matrix.size(), column = matrix[0].size();
    if (row != column) {
        throw std::logic_error("does not match");
    }
    if (row == 2) {
        return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
    }
    double ans = 0;
    for (int i = 0, bia = 1; i < column; i++) {
        ans += bia * matrix[0][i] * determinant(minor(matrix, 0, i));
        bia = -bia;
    }
    return ans;
}

Matrix inverse(const Matrix &matrix) {
    if (matrix.empty()) {
        return {};
    }
    int row = matrix.size(), column = matrix[0].size();
    if (row != column) {
        throw std::logic_error("wrong input");
    }
    double x = determinant(matrix);
    if (x == 0) {
        throw std::logic_error("have no inverse");
    }
    Matrix ans;
    ans.resize(row);
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < column; j++) {
            auto tmp = minor(matrix, i, j);
            double val = (((i + j) % 2 == 0) ? 1 : -1) * determinant(tmp);
            ans[j].push_back(val);
        }
    }
    ans = multiply(ans, 1 / x);
    return ans;
}

Matrix concatenate(const Matrix &matrix1, const Matrix &matrix2, int axis) {
    if (matrix1.empty() && matrix2.empty()) {
        return {};
    }
    int row_1 = matrix1.size(), column_1 = matrix1[0].size();
    int row_2 = matrix2.size(), column_2 = matrix2[0].size();
    if (axis == 0) {
        if (column_1 != column_2) {
            throw std::logic_error("does not match");
        }
        auto ans = matrix1;
        for (int i = 0; i < row_2; i++) {
            ans.emplace_back(matrix2[i]);
        }
        return ans;
    }
    if (row_1 != row_2) {
        throw std::logic_error("does not match");
    }
    auto ans = matrix1;
    for (int i = 0; i < column_2; i++) {
        for (int j = 0; j < row_2; j++) {
            ans[j].push_back(matrix2[j][i]);
        }
    }
    return ans;
}

Matrix ero_swap(const Matrix &matrix, size_t r1, size_t r2) {
    if (matrix.empty()) {
        return {};
    }
    int m = matrix.size();
    if (r1 >= m || r2 >= m) {
        throw std::logic_error("wrong parameter");
    }
    if (r1 == r2) {
        return matrix;
    }
    auto ans = matrix;
    auto tmp = std::move(matrix[r1]);
    ans[r1] = std::move(matrix[r2]);
    ans[r2] = std::move(tmp);
    return ans;
}

Matrix ero_multiply(const Matrix &matrix, size_t r, double c) {
    if (matrix.empty()) {
        return {};
    }
    int m = matrix.size();
    if (r >= m) {
        throw std::logic_error("wrong parameter");
    }
    auto ans = matrix;
    for (int i = 0; i < ans[r].size(); i++) {
        ans[r][i] *= c;
    }
    return ans;
}

Matrix ero_sum(const Matrix &matrix, size_t r1, double c, size_t r2) {
    if (matrix.empty()) {
        return {};
    }
    int m = matrix.size();
    if (r1 >= m || r2 >= m) {
        throw std::logic_error("wrong parameter");
    }
    auto ans = matrix;
    for (int i = 0; i < ans[r2].size(); i++) {
        ans[r2][i] += ans[r1][i] * c;
    }
    return ans;
}

Matrix upper_triangular(const Matrix &matrix) {
    if (matrix.empty()) {
        return {};
    }
    int row = matrix.size(), column = matrix[0].size();
    if (row != column) {
        throw std::logic_error("no square");
    }
    auto ans = matrix;
    for (int round = 0; round < column; round++) {
        int r = -1;
        double x = -1;
        for (int i = round; i < row; i++) {
            if (fabs(ans[i][round]) > x) {
                x = fabs(ans[i][round]);
                r = i;
            }
        }
        ans = ero_swap(ans, r, round);
        if (ans[round][round] == 0) continue;
        for (int j = column - 1; j >= round; j--) {
            ans[round][j] /= ans[round][round];
        }
        for (int i = round + 1; i < row; i++) {
            for (int j = column - 1; j >= round; j--) {
                ans[i][j] -= ans[i][round] / ans[round][round] * ans[round][j];
            }
        }
    }
    return ans;
}
}  // namespace algebra