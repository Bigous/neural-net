#pragma once

#include <cmath>

// Activation functions

// Sigmoid
float sigmoid(float x) { return 1.0f / (1.0f + std::exp(-x)); }

// Derivative of sigmoid
float dsigmoid(float x) { return sigmoid(x) * (1.0f - sigmoid(x)); }

// ReLU
float relu(float x) { return std::max(0.0f, x); }

// Derivative of ReLU
float drelu(float x) { return x > 0.0f ? 1.0f : 0.0f; }
