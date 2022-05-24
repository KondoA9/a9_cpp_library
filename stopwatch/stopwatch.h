#pragma once

#include <chrono>

namespace a9 {
    class stopwatch {
        std::chrono::system_clock::time_point begin_, end_;

    public:
        void start() {
            begin_ = std::chrono::system_clock::now();
        }

        void stop() {
            end_ = std::chrono::system_clock::now();
        }

        double elapsed_sec() const {
            return static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(end_ - begin_).count())
                   * 0.001;
        }
    };
}
