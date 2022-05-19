#include "deep_learning/NeuralNetwork.hpp"

#include <chrono> // duration && duration_cast && milliseconds && nanoseconds && high_resolution_clock
#include <iostream> // cout && endl
#include <locale>   // locale && to_string
#include <ranges>   // views::iota

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

void TestMatrixAlgos() {
    // Verifying that matrix multiplication is correct
    Matrix2D a(3, 3);
    Matrix2D b(3, 2);
    a.getData().assign({1, 2, 3, 4, 5, 6, 7, 8, 9});
    b.getData().assign({1, 2, 3, 4, 5, 6});
    Matrix2D c = a * b;
    std::cout << "Matrix multiplication: " << std::endl;
    std::cout << "a: " << std::endl;
    std::cout << a << std::endl;
    std::cout << "b: " << std::endl;
    std::cout << b << std::endl;
    std::cout << "c: " << std::endl;
    std::cout << c << std::endl;

    try {
        Matrix2D d = b * a;
        std::cout << "d: " << std::endl;
    } catch (const std::exception &e) {
        std::cout << "Exception: " << e.what() << std::endl;
    }
}

void StartTraining() {
    // TRAINING
    try {
        std::vector<Img> imgs;
        benchmark(
            [&imgs]() { imgs = csv_to_imgs("data/mnist_train.csv", 60000); },
            "1. csv_to_imgs");
        NeuralNetwork net(784, 300, 10, 0.164f);
        benchmark([&net, &imgs]() { net.train_batch_imgs(imgs, 8); },
                  "2. train_batch_imgs");
        benchmark([&net]() { net.save("data/net.txt"); }, "3. save");
    } catch (const std::exception &e) {
        std::cout << "Exception: " << e.what() << std::endl;
    }
}

void ContinueTraining(unsigned int nEpochs=1) {
    // Continue TRAINING
    try {
        std::vector<Img> imgs;
        benchmark(
            [&imgs]() {
                imgs = load_binary_compact_imgs("data/mnist_train_compact.bin");
            },
            "1. load_binary_compact_imgs");
        NeuralNetwork net;
        benchmark([&net]() { net.load_bin("data/net.net-bin"); },
                  "2. load_bin");
        benchmark([&net, &imgs, &nEpochs]() { net.train_batch_imgs(imgs, nEpochs); },
                  "3. train_batch_imgs");
        benchmark([&net]() { net.save_bin("data/net.net-bin"); },
                  "4. save_bin");
    } catch (const std::exception &e) {
        std::cout << "Exception: " << e.what() << std::endl;
    }
}

void Converting() {
    // Convert csv to binary
    try {
        std::vector<Img> imgs;
        benchmark(
            [&imgs]() {
                imgs = std::move(csv_to_imgs("data/mnist_test.csv", 10000));
            },
            "1. csv_to_imgs");

        benchmark([&imgs]() { save_binary_imgs("data/mnist_test.bin", imgs); },
                  "2. save_binary_imgs");
        benchmark(
            [&imgs]() {
                save_binary_compact_imgs("data/mnist_test_compact.bin", imgs);
            },
            "3. save_binary_compact_imgs");

        benchmark(
            [&imgs]() {
                imgs = std::move(csv_to_imgs("data/mnist_train.csv", 60000));
            },
            "4. csv_to_imgs");

        benchmark([&imgs]() { save_binary_imgs("data/mnist_train.bin", imgs); },
                  "5. save_binary_imgs");

        benchmark(
            [&imgs]() {
                save_binary_compact_imgs("data/mnist_train_compact.bin", imgs);
            },
            "6. save_binary_compact_imgs");

        benchmark(
            [&imgs]() {
                imgs = std::move(load_binary_imgs("data/mnist_test.bin"));
            },
            "7. load_binary_imgs");

        NeuralNetwork net;
        benchmark([&net]() { net.load("data/net.txt"); }, "6. load");

        auto &correct_image_prediction = imgs[0];
        auto &incorrect_image_prediction = imgs[115];

        auto c_pred = net.classify_img(correct_image_prediction);
        auto i_pred = net.classify_img(incorrect_image_prediction);

        auto show_classification = [](Img &img, auto prediction) {
            img.print();
            std::cout << "Classification: "
                      << (prediction == img.label ? "\033[32m" : "\033[31m")
                      << prediction << "\033[37m" << std::endl;
        };

        show_classification(correct_image_prediction, c_pred.argmax());
        show_classification(incorrect_image_prediction, i_pred.argmax());

        // double score;
        // benchmark([&net, &imgs, &score]() { score =
        // net.classify_imgs(imgs);
        // },
        //           "3. predict_batch_imgs");
        // std::cout << "Score: " << score << std::endl;

    } catch (const std::exception &e) {
        std::cout << "Exception: " << e.what() << std::endl;
    }
}

void Classifying() {
    // Classifying
    try {
        std::vector<Img> imgs;
        benchmark(
            [&imgs]() {
                imgs = std::move(
                    load_binary_compact_imgs("data/mnist_test_compact.bin"));
            },
            "1. load_binary_compact_imgs");

        NeuralNetwork net;
        benchmark([&net]() { net.load("data/net.txt"); }, "2. load");

        auto show_classification = [](Img &img, auto &prediction) {
            auto classification = prediction.argmax();
            img.print();
            std::cout << "Classification: "
                      << (classification == img.label ? "\033[32m" : "\033[31m")
                      << classification << "\033[37m" << std::endl
                      << "Classification Matrix:" << std::endl
                      << prediction << std::endl;
        };

        // Correct classifications
        for (int i : std::views::iota(0, 4)) {
            auto &img = imgs[i];
            Matrix2D classification = std::move(net.classify_img(img));
            show_classification(img, classification);
        }

        // Incorrect classifications
        for (auto &i : std::vector<int>({115, 217, 247, 259})) {
            auto &img = imgs[i];
            Matrix2D classification = std::move(net.classify_img(img));
            show_classification(img, classification);
        }

    } catch (const std::exception &e) {
        std::cout << "Exception: " << e.what() << std::endl;
    }
}

void ClassificationBenchmarck() {
    // Classifying benchmark
    try {
        std::vector<Img> imgs;
        benchmark(
            [&imgs]() {
                imgs = std::move(
                    load_binary_compact_imgs("data/mnist_test_compact.bin"));
            },
            "1. load_binary_compact_imgs");

        NeuralNetwork net2;
        benchmark([&net2]() { net2.load_bin("data/net.net-bin"); },
                  "2. load_bin");

        double score;
        benchmark(
            [&net2, &imgs, &score]() { score = net2.classify_imgs(imgs); },
            "3. predict_batch_imgs");
        std::cout << "Score: " << score << std::endl;

    } catch (const std::exception &e) {
        std::cout << "Exception: " << e.what() << std::endl;
    }
}

int main(int argc, char *argv[]) {
    std::clog.imbue(std::locale("en-US"));
    std::cout.imbue(std::locale("en-US"));

    // TestMatrixAlgos();

    // StartTraining();

    // ContinueTraining(4);

    // Converting();

    // Classifying();

    ClassificationBenchmarck();

    return 0;
}
