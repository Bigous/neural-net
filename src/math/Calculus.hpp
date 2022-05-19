#pragma once

#include "Activation.hpp"
#include "Matrix2D.hpp"

// Sigmoid prime
// Matrix2D sigmoidPrime(const Matrix2D &m) { return m.multiply(1 - m); }
Matrix2D sigmoidPrime(const Matrix2D &m) {
    return m.map([](float v) { return v * (1 - v); });
}

// Softmax
Matrix2D softmax(const Matrix2D &m) {
    double total = m.reduce<double>(0.0, [](double a, float b) { return a + exp(b); });
    return m.map([&total](float a) -> float { return float(exp(a) / total); });
}

// ReLU prime
Matrix2D reluPrime(const Matrix2D &m) { return m.map(drelu); }
