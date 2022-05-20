#include <fstream>
#include <iostream>
#include <format>
#include <string>

#include "../math/Activation.hpp"
#include "../math/Calculus.hpp"
#include "../utils/ProgressBar.hpp"
#include "NeuralNetwork.hpp"

NeuralNetwork::NeuralNetwork(int input, int hidden, int output, float lr) {
    this->input = input;
    this->hidden = hidden;
    this->output = output;
    this->learning_rate = lr;
    Matrix2D hidden_layer(hidden, input);
    Matrix2D output_layer(output, hidden);
    hidden_layer.randomize(hidden);
    output_layer.randomize(output);
    this->hidden_weights = std::move(hidden_layer);
    this->output_weights = std::move(output_layer);
}

float NeuralNetwork::train(const Matrix2D &input_data,
                          const Matrix2D &output_data) {
    // Feed forward
    Matrix2D hidden_outputs = hidden_weights * input_data;
    hidden_outputs.apply(sigmoid);
    Matrix2D final_outputs = output_weights * hidden_outputs;
    final_outputs.apply(sigmoid);

    // Find errors
    Matrix2D output_errors = output_data - final_outputs;
    Matrix2D hidden_errors = output_weights.transpose() * output_errors;

    // Backpropogate
    // output_weights = add(
    //		 output_weights,
    //     scale(
    // 			  net->lr,
    //			  dot(
    // 		 			multiply(
    // 						output_errors,
    //				  	sigmoidPrime(final_outputs)
    //					),
    //					transpose(hidden_outputs)
    // 				)
    //		 )
    // )
    Matrix2D sigmoid_primed_mat = sigmoidPrime(final_outputs);
    Matrix2D multiplied_mat = output_errors.multiply(sigmoid_primed_mat);
    Matrix2D transposed_mat = hidden_outputs.transpose();
    Matrix2D dot_mat = multiplied_mat * transposed_mat;
    Matrix2D scaled_mat = learning_rate * dot_mat;
    Matrix2D added_mat = output_weights + scaled_mat;

    output_weights = added_mat;

    // hidden_weights = add(
    // 	 net->hidden_weights,
    // 	 scale (
    //			net->learning_rate
    //    	dot (
    //				multiply(
    //					hidden_errors,
    //					sigmoidPrime(hidden_outputs)
    //				)
    //				transpose(inputs)
    //      )
    // 	 )
    // )
    // Reusing variables after freeing memory
    sigmoid_primed_mat = sigmoidPrime(hidden_outputs);
    multiplied_mat = hidden_errors.multiply(sigmoid_primed_mat);
    transposed_mat = input_data.transpose();
    dot_mat = multiplied_mat * transposed_mat;
    scaled_mat = learning_rate * dot_mat;
    added_mat = hidden_weights + scaled_mat;

    hidden_weights = added_mat;

    float cost = output_errors.reduce<float>(0.0, [] (float a, float b) { return a + b * b; });

    return cost;
}

void NeuralNetwork::train_batch_imgs(const std::vector<Img> &imgs,
                                     unsigned int epochs) {
    for (unsigned int e = 1; e <= epochs; e++) {
        int i = 0;
        double avg_cost = 0;
        ProgressBar progress(std::format("Epoch {}/{}", e, epochs), int(imgs.size()));
        progress.update(i);
        for (const Img &cur_img : imgs) {
            auto img_data =
                cur_img.img_data.flatten(0); // 0 = flatten to column vector
            Matrix2D output(10, 1);
            output.fill(0.0f);
            output[cur_img.label] = 1.0f; // Setting the result
            float cost = train(img_data, output);
            avg_cost += cost;
            i++;
            progress.update(i, std::format("Cost: {}", cost));
        }
        avg_cost /= imgs.size();
        std::clog << " Avg Cost: " << avg_cost << std::endl;
    }
}

Matrix2D NeuralNetwork::classify_img(const Img &img) {
    Matrix2D img_data = img.img_data.flatten(0);
    return classify(img_data);
}

double NeuralNetwork::classify_imgs(const std::vector<Img> &imgs) {
    int n_correct = 0;
    for(const Img &cur_img : imgs) {
        Matrix2D prediction = classify_img(cur_img);
        if (prediction.argmax() == cur_img.label) {
            n_correct++;
        }
    }
    return 1.0 * n_correct / imgs.size();
}

Matrix2D NeuralNetwork::classify(Matrix2D input_data) {
    // Matrix2D outputs = (hidden_weights * input_data).apply(sigmoid); //
    // hidden outputs *= output_weights; outputs.apply(sigmoid); return
    // softmax(outputs);
    Matrix2D hidden_outputs = hidden_weights * input_data;
    hidden_outputs.apply(sigmoid);
    Matrix2D final_outputs = output_weights * hidden_outputs;
    final_outputs.apply(sigmoid);
    return softmax(final_outputs);
}

void NeuralNetwork::save(const std::string &file_string) {
    std::ofstream file(file_string);
    file << input << "\n"
         << hidden << "\n"
         << output << "\n"
         << learning_rate << "\n"
         << hidden_weights << output_weights;
    file.close();
    std::cout << "Successfully written to '" << file_string << "'" << std::endl;
}

void NeuralNetwork::save_bin(const std::string &file_string) {
    std::ofstream file(file_string, std::ios::binary | std::ios::trunc | std::ios::out);
    write(file, input);
    write(file, hidden);
    write(file, output);
    write(file, learning_rate);
    hidden_weights.write_bin(file);
    output_weights.write_bin(file);
    file.close();
    std::cout << "Successfully written to '" << file_string << "'" << std::endl;
}

void NeuralNetwork::load(const std::string &file_string) {
    std::ifstream file(file_string);
    file >> input >> hidden >> output >> learning_rate >> hidden_weights >>
        output_weights;
    file.close();
    std::cout << "Successfully loaded from '" << file_string << "'"
              << std::endl;
}

void NeuralNetwork::load_bin(const std::string &file_string) {
    std::ifstream file(file_string, std::ios::binary | std::ios::in);
    read(file, input);
    read(file, hidden);
    read(file, output);
    read(file, learning_rate);
    hidden_weights.load_bin(file);
    output_weights.load_bin(file);
    file.close();
    std::cout << "Successfully loaded from '" << file_string << "'"
              << std::endl;
}

void NeuralNetwork::print() {
    std::cout << "Inputs: " << input << std::endl
              << "Hidden: " << hidden << std::endl
              << "Output: " << output << std::endl
              << "Learning Rate: " << learning_rate << std::endl
              << "Hidden Weights: " << hidden_weights << std::endl
              << "Output Weights: " << output_weights << std::endl;
}
