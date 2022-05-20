#pragma once

#include <fstream>
#include <vector>

/**
 * @brief Helper function to read data from a binary file
 *
 * @param is input stream to read from
 * @param t Variable to read into
 */
inline void read(std::istream &is, auto &t) {
    is.read(reinterpret_cast<char *>(&t), sizeof(t));
}

/**
 * @brief Helper function to read a vector from a binary file
 * You must set the size of the vector before reading it!
 * @tparam T Type of the vector
 * @param is Input stream to read from
 * @param v Vector to read into
 */
template <typename T>
inline void read(std::istream &is, std::vector<T> &v) {
    for(auto &t : v) {
        read(is, t);
    }
}

/**
 * @brief Helper function to write data to a binary file
 *
 * @param os Output stream to write to
 * @param t Variable to write
 */
inline void write(std::ostream &os, const auto &t) {
    os.write(reinterpret_cast<const char *>(&t), sizeof(t));
}

/**
 * @brief Helper function to write a vector to a binary file
 *
 * @tparam T Type of the vector
 * @param os Output stream to write to
 * @param v Vector to write
 */
template <typename T>
inline void write(std::ostream &os, const std::vector<T> &v) {
    for (auto &t : v) {
        write(os, t);
    }
}
