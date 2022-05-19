#include "deep_learning/NeuralNetwork.hpp"

#include <chrono>
#include <iomanip>
#include <iostream>

void benchmark(std::function<void()> func,
               const std::string &name = "Anonymous") {
    auto start = std::chrono::high_resolution_clock::now();
    func();
    auto end = std::chrono::high_resolution_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    std::clog << name << " - time taken: " << duration.count() << " ns"
              << std::endl;
}

int main(int argc, char *argv[]) {
    // // Verifying that matrix multiplication is correct
    // Matrix2D a(3, 3);
    // Matrix2D b(3, 2);
    // a.getData().assign({1, 2, 3, 4, 5, 6, 7, 8, 9});
    // b.getData().assign({1, 2, 3, 4, 5, 6});
    // Matrix2D c = a * b;
    // std::cout << "Matrix multiplication: " << std::endl;
    // std::cout << "a: " << std::endl;
    // std::cout << a << std::endl;
    // std::cout << "b: " << std::endl;
    // std::cout << b << std::endl;
    // std::cout << "c: " << std::endl;
    // std::cout << c << std::endl;

    // try {
    //     Matrix2D d = b * a;
    //     std::cout << "d: " << std::endl;
    // } catch (const std::exception &e) {
    //     std::cout << "Exception: " << e.what() << std::endl;
    // }

    // // TRAINING
    // try {
    //     std::vector<Img> imgs;
    //     benchmark(
    //         [&imgs]() { imgs = csv_to_imgs("data/mnist_train.csv", 60000); },
    //         "1. csv_to_imgs");
    //     NeuralNetwork net(784, 300, 10, 0.164f);
    //     benchmark([&net, &imgs]() { net.train_batch_imgs(imgs, 8); },
    //               "2. train_batch_imgs");
    //     benchmark([&net]() { net.save("data/net.txt"); }, "3. save");
    // } catch (const std::exception &e) {
    //     std::cout << "Exception: " << e.what() << std::endl;
    // }

    // // Continue TRAINING
    // try {
    //     std::vector<Img> imgs;
    //     benchmark(
    //         [&imgs]() { imgs = csv_to_imgs("data/mnist_train.csv", 60000); },
    //         "1. csv_to_imgs");
    //     NeuralNetwork net;
    //     benchmark([&net]() { net.load("data/net.txt"); }, "2. load");
    //     benchmark([&net, &imgs]() { net.train_batch_imgs(imgs, 1); },
    //               "3. train_batch_imgs");
    //     benchmark([&net]() { net.save("data/net.txt"); }, "4. save");
    // } catch (const std::exception &e) {
    //     std::cout << "Exception: " << e.what() << std::endl;
    // }

    // Classifying
    try {
        std::vector<Img> imgs;
        benchmark(
            [&imgs]() { imgs = csv_to_imgs("data/mnist_test.csv", 10000); },
            "1. csv_to_imgs");

        NeuralNetwork net;
        benchmark([&net]() { net.load("data/net.txt"); }, "2. load");

        auto pred = net.classify_img(imgs[0]);

        std::cout << std::setprecision(4);
        imgs[0].print();
        std::cout << "Prediction: " << pred.argmax() << std::endl << pred <<
        std::endl;

        double score;
        benchmark(
            [&net, &imgs, &score]() { score = net.classify_imgs(imgs);
            }, "3. predict_batch_imgs");

        std::cout << "Score: " << score << std::endl;
    } catch (const std::exception &e) {
        std::cout << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
