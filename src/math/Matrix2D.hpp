#pragma once

#include <cassert>    // assert
#include <chrono>     // high_resolution_clock
#include <cmath>      // sqrt
#include <cstddef>    // size_t
#include <functional> // function && bind
#include <new>        // placement new && bad_alloc
#include <random>     // uniform_real_distribution && default_random_engine
#include <ranges>     // ranges::copy && ranges::transform
#include <sstream>    // stringstream
#include <string>     // string
#include <utility>    // move && as_const
#include <vector>     // vector

class Matrix2D {
    std::vector<float> m;
    std::size_t cols;
    std::size_t rows;

  public:
    // Constructors
    Matrix2D() = default;

    Matrix2D(std::size_t cols, std::size_t rows) {
        this->cols = cols;
        this->rows = rows;
        m.resize(rows * cols);
    }

    // copy constructor
    Matrix2D(const Matrix2D &other) {
        this->cols = other.cols;
        this->rows = other.rows;
        m.resize(other.m.size());
        for (std::size_t i = 0; i < m.size(); i++)
            m[i] = other.m[i];
    }

    // move constructor
    Matrix2D(Matrix2D &&other) noexcept {
        this->cols = other.cols;
        this->rows = other.rows;
        m = std::move(other.m);
    }

    // Constructs from a stream
    Matrix2D(std::istream &is) { is >> *this; }

    // Constructs from a string
    Matrix2D(const std::string &str) {
        std::istringstream iss(str);
        iss >> *this;
    }

    // Destructor
    ~Matrix2D() = default;

    // copy assignment
    Matrix2D &operator=(const Matrix2D &other) {
        if (this != &other) {
            this->cols = other.cols;
            this->rows = other.rows;
            m.resize(other.m.size());
            for (std::size_t i = 0; i < m.size(); i++)
                m[i] = other.m[i];
        }
        return *this;
    }

    // move assignment
    Matrix2D &operator=(Matrix2D &&other) noexcept {
        if (this != &other) {
            this->cols = other.cols;
            this->rows = other.rows;
            m = std::move(other.m);
        }
        return *this;
    }

    // index operator
    float &operator()(std::size_t row, std::size_t col) {
        assert(col < cols && row < rows);
        return m[row * cols + col];
    }

    // const index operator
    const float &operator()(std::size_t row, std::size_t col) const {
        assert(col < cols && row < rows);
        return m[row * cols + col];
    }

    // getter
    std::size_t getCols() const { return cols; }

    // getter
    std::size_t getRows() const { return rows; }

    // getter
    std::vector<float> &getData() { return m; }

    // getter
    float &operator[](std::size_t i) {
        assert(i < cols * rows);
        return m[i];
    }

    // Add Scalar
    Matrix2D operator+(float scalar) const {
        Matrix2D result(cols, rows);
        for (std::size_t i = 0; i < m.size(); i++)
            result.m[i] = m[i] + scalar;
        return result;
    }

    // Add scalar into this
    Matrix2D &operator+=(float scalar) {
        for (std::size_t i = 0; i < cols * rows; ++i) {
            m[i] += scalar;
        }
        return *this;
    }

    // Scalar addition
    friend Matrix2D operator+(float scalar, const Matrix2D &m) {
        return m + scalar;
    }

    // Subtract Scalar
    Matrix2D operator-(float scalar) const {
        Matrix2D result(cols, rows);
        for (std::size_t i = 0; i < cols * rows; ++i) {
            result.m[i] = m[i] - scalar;
        }
        return result;
    }

    // Subtract scalar into this
    Matrix2D &operator-=(float scalar) {
        for (std::size_t i = 0; i < cols * rows; ++i) {
            m[i] -= scalar;
        }
        return *this;
    }

    // Scalar subtraction
    friend Matrix2D operator-(float scalar, const Matrix2D &m) {
        return (m * -1.0f) += scalar;
    }

    // Multiply Scalar
    Matrix2D operator*(float scalar) const {
        Matrix2D result(cols, rows);
        for (std::size_t i = 0; i < cols * rows; ++i) {
            result.m[i] = m[i] * scalar;
        }
        return result;
    }

    // Multiply scalar into this
    Matrix2D &operator*=(float scalar) {
        for (std::size_t i = 0; i < cols * rows; ++i) {
            m[i] *= scalar;
        }
        return *this;
    }

    // Scalar multiplication
    friend Matrix2D operator*(float scalar, const Matrix2D &m) {
        return m * scalar;
    }

    // Divide Scalar
    Matrix2D operator/(float scalar) const {
        Matrix2D result(cols, rows);
        for (std::size_t i = 0; i < cols * rows; ++i) {
            result.m[i] = m[i] / scalar;
        }
        return result;
    }

    // Divide scalar into this
    Matrix2D &operator/=(float scalar) {
        for (std::size_t i = 0; i < cols * rows; ++i) {
            m[i] /= scalar;
        }
        return *this;
    }

    // Scalar division
    friend Matrix2D operator/(float scalar, const Matrix2D &m) {
        return (m * (1.0f / scalar));
    }

    // Add Matrix
    Matrix2D operator+(const Matrix2D &other) const {
        assert(cols == other.cols && rows == other.rows);
        Matrix2D result(cols, rows);
        for (std::size_t i = 0; i < cols * rows; ++i) {
            result.m[i] = m[i] + other.m[i];
        }
        return result;
    }

    // Add matrix into this
    Matrix2D &operator+=(const Matrix2D &other) {
        assert(cols == other.cols && rows == other.rows);
        for (std::size_t i = 0; i < cols * rows; ++i) {
            m[i] += other.m[i];
        }
        return *this;
    }

    // Subtract Matrix
    Matrix2D operator-(const Matrix2D &other) const {
        assert(cols == other.cols && rows == other.rows);
        Matrix2D result(cols, rows);
        for (std::size_t i = 0; i < cols * rows; ++i) {
            result.m[i] = m[i] - other.m[i];
        }
        return result;
    }

    // Subtract matrix into this
    Matrix2D &operator-=(const Matrix2D &other) {
        assert(cols == other.cols && rows == other.rows);
        for (std::size_t i = 0; i < cols * rows; ++i) {
            m[i] -= other.m[i];
        }
        return *this;
    }

    // Dot product the matrix with another matrix
    Matrix2D operator*(const Matrix2D &other) const {
        assert(rows == other.cols);
        Matrix2D result(cols, other.rows);
        for (std::size_t i = 0; i < cols; ++i) {
            for (std::size_t j = 0; j < other.rows; ++j) {
                float sum = 0.0f;
                for (std::size_t k = 0; k < rows; ++k) {
                    sum += m[i * rows + k] * other.m[k * other.rows + j];
                }
                result.m[i * other.rows + j] = sum;
            }
        }
        return result;
    }

    // Dot product the matrix into this
    Matrix2D &operator*=(const Matrix2D &other) {
        assert(rows == other.cols);
        for (std::size_t i = 0; i < cols; ++i) {
            for (std::size_t j = 0; j < other.rows; ++j) {
                float sum = 0.0f;
                for (std::size_t k = 0; k < rows; ++k) {
                    sum += m[i * rows + k] * other.m[k * other.rows + j];
                }
                m[i * other.rows + j] = sum;
            }
        }
        return *this;
    }

    // Multiply Matrix
    Matrix2D multiply(const Matrix2D &other) const {
        assert(cols == other.cols && rows == other.rows);
        Matrix2D result(cols, rows);
        for (std::size_t i = 0; i < cols * rows; ++i) {
            result.m[i] = m[i] * other.m[i];
        }
        return result;
    }

    // Apply a function to each element of the new temporary matrix
    Matrix2D map(std::function<float(float)> f) const {
        Matrix2D result(cols, rows);
        for (std::size_t i = 0; i < cols * rows; ++i) {
            result.m[i] = f(m[i]);
        }
        return result;
    }

    // Apply a function to each element of this matrix
    Matrix2D &apply(std::function<float(float)> f) {
        for (std::size_t i = 0; i < cols * rows; ++i) {
            m[i] = f(m[i]);
        }
        return *this;
    }

    // Reduce the matrix to a single value
    template <typename T>
    T reduce(T start, std::function<T(T, float)> f) const {
        T result = start;
        for (const float &e : m) {
            result = f(result, e);
        }
        return result;
    }

    // Transpose the matrix
    Matrix2D transpose() const {
        Matrix2D result(rows, cols);
        for (std::size_t i = 0; i < cols; ++i) {
            for (std::size_t j = 0; j < rows; ++j) {
                result.m[j * result.rows + i] = m[i * rows + j];
            }
        }
        return result;
    }

    // Return the determinant of the matrix
    float determinant() const {
        assert(cols == rows);
        float result = 0.0f;
        for (std::size_t i = 0; i < cols; ++i) {
            result += m[i * rows + i] * cofactor(i, i);
        }
        return result;
    }

    // Return the cofactor of the matrix at the given row and column
    float cofactor(std::size_t row, std::size_t col) const {
        assert(cols == rows);
        Matrix2D minor(cols - 1, rows - 1);
        std::size_t minorRow = 0;
        std::size_t minorCol = 0;
        for (std::size_t i = 0; i < cols; ++i) {
            if (i == row) {
                continue;
            }
            for (std::size_t j = 0; j < rows; ++j) {
                if (j == col) {
                    continue;
                }
                minor.m[minorRow * (minor.cols) + minorCol] = m[i * rows + j];
                ++minorCol;
            }
            ++minorRow;
            minorCol = 0;
        }
        return (row + col) % 2 == 0 ? minor.determinant()
                                    : -minor.determinant();
    }

    // Return the inverse of the matrix
    Matrix2D inverse() const {
        assert(cols == rows);
        Matrix2D result(cols, rows);
        float det = determinant();
        if (det == 0.0f) {
            return result;
        }
        for (std::size_t i = 0; i < cols; ++i) {
            for (std::size_t j = 0; j < rows; ++j) {
                result.m[i * rows + j] = cofactor(j, i) / det;
            }
        }
        return result.transpose();
    }

    // Fill the matrix with a value
    void fill(float value) {
        for (std::size_t i = 0; i < cols * rows; ++i) {
            m[i] = value;
        }
    }

    // Writes the matrix to a stream
    friend std::ostream &operator<<(std::ostream &os, const Matrix2D &m) {
        os << m.cols << " " << m.rows << "\n";
        for (std::size_t row = 0; row < m.rows; ++row) {
            for (std::size_t col = 0; col < m.cols; ++col) {
                os << m.m[row * m.cols + col] << " ";
            }
            os << "\n";
        }
        return os;
    }

    // Return the matrix as a string
    std::string toString() const {
        std::stringstream ss;
        ss << *this;
        return ss.str();
    }

    // Fill the matrix from a stream
    friend std::istream &operator>>(std::istream &is, Matrix2D &m) {
        is >> m.cols >> m.rows;
        m.m.resize(m.cols * m.rows);
        for (std::size_t row = 0; row < m.rows; ++row) {
            for (std::size_t col = 0; col < m.cols; ++col) {
                is >> m.m[row * m.cols + col];
            }
        }
        return is;
    }

    // Randomize the matrix with values between -1 and 1
    void randomize() {
        static std::default_random_engine generator(
            unsigned int(std::chrono::high_resolution_clock::now()
                             .time_since_epoch()
                             .count() &
                         0xFFFFFFFF));
        static std::uniform_real_distribution<float> distribution(-1.0f, 1.0f);
        static auto random_generator = std::bind(distribution, generator);
        this->apply([](float v) { return random_generator(); });
    }

    // Randomize the matrix with values between -1 / sqrt(n) and 1 / sqrt(n)
    void randomize(int n) {
        std::default_random_engine generator(
            unsigned int(std::chrono::high_resolution_clock::now()
                             .time_since_epoch()
                             .count() &
                         0xFFFFFFFF));
        std::uniform_real_distribution<float> distribution(
            -1.0f / float(std::sqrt(n)), 1.0f / float(std::sqrt(n)));
        auto random_generator = std::bind(distribution, generator);
        this->apply(
            [&random_generator](float v) { return random_generator(); });
    }

    // Randomize the matrix with values between min and max
    void randomize(float min, float max) {
        std::default_random_engine generator(
            unsigned int(std::chrono::high_resolution_clock::now()
                             .time_since_epoch()
                             .count() &
                         0xFFFFFFFF));
        std::uniform_real_distribution<float> distribution(min, max);
        auto random_generator = std::bind(distribution, generator);
        this->apply(
            [&random_generator](float v) { return random_generator(); });
    }

    std::size_t argmax() {
        // Expects a Mx1 matrix
        float max_score = 0;
        std::size_t max_idx = 0;
        for (std::size_t i = 0; i < m.size(); i++) {
            if (m[i] > max_score) {
                max_score = m[i];
                max_idx = i;
            }
        }
        return max_idx;
    }

    // Flatten the matrix into a new matrix, axis = 0 for Column Matrix, axis =
    // 1 for Row Matrix
    Matrix2D flatten(int axis) const {
        assert(axis == 0 || axis == 1);
        if (axis == 1) {
            Matrix2D result(1, cols * rows);
            for (std::size_t i = 0; i < cols; ++i) {
                for (std::size_t j = 0; j < rows; ++j) {
                    result.m[j * result.rows + i] = m[i * rows + j];
                }
            }
            return result;
        } else {
            Matrix2D result(rows * cols, 1);
            for (std::size_t i = 0; i < cols * rows; ++i) {
                result.m[i] = m[i];
            }
            return result;
        }
    }

    // Write binary to a stream
    void write_bin(std::ostream &of) const {
        of.write((char *)&cols, sizeof(cols));
        of.write((char *)&rows, sizeof(rows));
        of.write((char *)&m[0], sizeof(float) * m.size());
    }

    // load from binary stream
    void load_bin(std::istream &is) {
        is.read((char *)&cols, sizeof(cols));
        is.read((char *)&rows, sizeof(rows));
        m.resize(cols * rows);
        float f;
        for(int i = 0; i < cols * rows; i++) {
            is.read((char *)&f, sizeof(float));
            m[i] = f;
        }
    }

    // Write binary compact to a file
    void write_bin_compact(std::ostream &of) const {
        of.write((char *)&cols, sizeof(cols));
        of.write((char *)&rows, sizeof(rows));
        for(const float &v : m) {
            unsigned char c = (unsigned char)(v * 255.0f);
            of.write((char *)&c, sizeof(c));
        }
    }

    // load from binary stream
    void load_bin_compact(std::istream &is) {
        is.read((char *)&cols, sizeof(cols));
        is.read((char *)&rows, sizeof(rows));
        m.resize(cols * rows);
        for(float &v : m) {
            unsigned char c;
            is.read((char *)&c, sizeof(c));
            v = (float)c / 255.0f;
        }
    }
};
