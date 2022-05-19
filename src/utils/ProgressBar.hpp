#pragma once

#include <string>
#include <chrono>

class ProgressBar {
    std::string title;
    std::string message;
    int max;
    int width;
    int value;
    int last_pct;
    std::chrono::time_point<std::chrono::high_resolution_clock> start;
    std::chrono::time_point<std::chrono::high_resolution_clock> last_update;
public:
    ProgressBar(std::string title, int max, int width = 50);
    ~ProgressBar()=default;

    void restart() {
        start = std::chrono::high_resolution_clock::now();
        value = 0;
        last_pct = 0;
    }
    void update();
    void update(int value);
    void update(int value, std::string message);
};
