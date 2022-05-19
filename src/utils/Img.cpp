#include "Img.hpp"

#include <fstream>
#include <iostream>
#include <ranges>

std::string getColor(float value) {
    if (value <= 0) {
        // ANSI black
        return "\033[30m";
    } else if (value <= 0.33f) {
        // ANSI light black
        return "\033[90m";
    } else if (value <= 0.66f) {
        // ANSI dark white
        return "\033[37m";
    } else {
        // ANSI white
        return "\033[97m";
    }
}

void Img::print() {
    std::cout << "Img data: " << std::endl;
    // std::cout << img_data << std::endl;
    auto &data = img_data.getData();
    for (std::size_t row = 0; row < img_data.getRows(); ++row) {
        for (std::size_t col = 0; col < img_data.getCols(); ++col) {
            const float &v = data[row * img_data.getCols() + col];
            std::cout << getColor(v) << int(v * 10) << " ";
        }
        std::cout << "\n";
    }
    // ANSI blue

    std::cout << "\033[37m"
              << "Label: "
              << "\033[94m" << label << "\033[37m" << std::endl;
}

std::vector<Img> csv_to_imgs(const std::string &file_string,
                             int number_of_imgs) {
    std::ifstream file(file_string);
    std::vector<Img> imgs;
    imgs.reserve(number_of_imgs);
    std::string line;
    int i = 0;
    while (std::getline(file, line)) {
        i++;
        if (i == 1 || line.empty()) {
            continue; // skip header
        }
        Img img;
        img.img_data = std::move(Matrix2D(28, 28));
        int j = 0;

        std::istringstream s(line);
        std::string field;
        while (getline(s, field, ',')) {
            if (j == 0) {
                img.label = std::stoi(field);
            } else {
                img.img_data[j - 1] = std::stoi(field) / 256.0f;
            }
            j++;
        }
        imgs.push_back(std::move(img));
    }
    return imgs;
}

bool save_binary_imgs(const std::string &file_string,
                      const std::vector<Img> &imgs) {
    std::ofstream file(file_string, std::ios::binary | std::ios::trunc);
    if (!file.is_open()) {
        return false;
    }
    auto size = imgs.size();
    file.write(reinterpret_cast<const char *>(&size), sizeof(size));
    for (const auto &img : imgs) {
        file.write(reinterpret_cast<const char *>(&img.label),
                   sizeof(img.label));
        img.img_data.write_bin(file);
    }
    return true;
}

std::vector<Img> load_binary_imgs(const std::string &file_string) {
    std::ifstream file(file_string, std::ios::binary);
    if (!file.is_open()) {
        return {};
    }
    std::size_t size;
    file.read(reinterpret_cast<char *>(&size), sizeof(size));
    if (size <= 0) {
        return {};
    }
    std::vector<Img> imgs;
    imgs.reserve(size);
    for (int i = 0; i < size; ++i) {
        Img img;
        file.read(reinterpret_cast<char *>(&img.label), sizeof(img.label));
        img.img_data.load_bin(file);
        imgs.push_back(std::move(img));
    }
    return imgs;
}

bool save_binary_compact_imgs(const std::string &file_string,
                              const std::vector<Img> &imgs) {
    std::ofstream file(file_string, std::ios::binary | std::ios::trunc);
    if (!file.is_open()) {
        return false;
    }
    auto size = imgs.size();
    file.write(reinterpret_cast<const char *>(&size), sizeof(size));
    for (const auto &img : imgs) {
        file.write(reinterpret_cast<const char *>(&img.label),
                   sizeof(img.label));
        img.img_data.write_bin_compact(file);
    }
    return true;
}

std::vector<Img> load_binary_compact_imgs(const std::string &file_string) {
    std::ifstream file(file_string, std::ios::binary);
    if (!file.is_open()) {
        return {};
    }
    std::size_t size;
    file.read(reinterpret_cast<char *>(&size), sizeof(size));
    if (size <= 0) {
        return {};
    }
    std::vector<Img> imgs;
    imgs.reserve(size);
    for (int i = 0; i < size; ++i) {
        Img img;
        file.read(reinterpret_cast<char *>(&img.label), sizeof(img.label));
        img.img_data.load_bin_compact(file);
        imgs.push_back(std::move(img));
    }
    return imgs;
}
