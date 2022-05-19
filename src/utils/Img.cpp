#include "Img.hpp"

#include <fstream>
#include <iostream>

void Img::print() {
    std::cout << "Img data: " << std::endl;
    std::cout << img_data << std::endl;
    std::cout << "Label: " << label << std::endl;
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
        if(i == 1 || line.empty()) {
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
