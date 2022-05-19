#pragma once

#include <string>

#include "../math/Matrix2D.hpp"
#include "../utils/Img.hpp"

class NeuralNetwork {
    int input;
    int hidden;
    int output;
    float learning_rate;
    Matrix2D hidden_weights;
    Matrix2D output_weights;

  public:
    NeuralNetwork() = default;
    NeuralNetwork(int input, int hidden, int output, float lr);
    float train(const Matrix2D &input_data, const Matrix2D &output_data);
    void train_batch_imgs(const std::vector<Img> &imgs, unsigned int epochs = 1);
    Matrix2D classify_img(const Img &img);
    double classify_imgs(const std::vector<Img> &imgs);
    Matrix2D classify(Matrix2D input_data);
    void save(const std::string &file_string);
    void load(const std::string &file_string);
    void save_bin(const std::string &file_string);
    void load_bin(const std::string &file_string);
    void print();
};
