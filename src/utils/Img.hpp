#pragma once

#include "../math/Matrix2D.hpp"

struct Img {
    int label;
    Matrix2D img_data;

    void print();
};

std::vector<Img> csv_to_imgs(const std::string &file_string,
                             int number_of_imgs);

bool save_binary_imgs(const std::string &file_string,
                      const std::vector<Img> &imgs);

std::vector<Img> load_binary_imgs(const std::string &file_string);

bool save_binary_compact_imgs(const std::string &file_string,
                              const std::vector<Img> &imgs);

std::vector<Img> load_binary_compact_imgs(const std::string &file_string);
