#include "ProgressBar.hpp"
#include <chrono>
#include <iostream>

ProgressBar::ProgressBar(std::string title, int max, int width)
    : title(title), max(max), width(width), value(0), last_pct(0) {
    start = std::chrono::high_resolution_clock::now();
}

void ProgressBar::update() {
    int percent = value * 100 / max;
    if (percent == last_pct) {
        if(std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::high_resolution_clock::now() - last_update).count() < 250) {
            return;
        }
    }
    std::string bar(width, ' ');

    // Calculate the progress bar
    int pos = value * width / max;
    for (int i = 0; i < width; i++) {
        if (i < pos) {
            bar.replace(i, 1, "=");
        } else if (i == pos) {
            bar.replace(i, 1, ">");
        }
    }

    // Print the progress bar
    std::clog << "\r" << title << " [";
    std::clog << bar;
    std::clog << "] ";

    // Print the percentage
    std::clog << percent << "%";

    auto durationMS =
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::high_resolution_clock::now() - start)
            .count();
    std::clog << " E: " << durationMS << "ms";

    if (percent > 0) {
        // Print estimated time remaining
        auto remainingMS = (durationMS / percent) * (100 - percent);
        std::clog << " ETA: " << remainingMS << "ms ";
    }

    // Print the message
    std::clog << message << " " << std::flush;
    last_update = std::chrono::high_resolution_clock::now();
    last_pct = percent;
}

void ProgressBar::update(int value) {
    this->value = value;
    update();
}

void ProgressBar::update(int value, std::string message) {
    this->value = value;
    this->message = message;
    update();
}
