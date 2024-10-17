#pragma once

#include <chrono>
#include <utility>

#define MEASURE_TIME(operation)                                                             \
    do {                                                                                    \
        auto start = std::chrono::high_resolution_clock::now();                             \
        operation;                                                                          \
        auto end = std::chrono::high_resolution_clock::now();                               \
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start); \
        duration;                                                                           \
    } while (0)

#define MEASURE_TIME_WITH_RESULT(operation)                                                 \
    ({                                                                                      \
        auto start = std::chrono::high_resolution_clock::now();                             \
        auto result = (operation);                                                          \
        auto end = std::chrono::high_resolution_clock::now();                               \
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start); \
        std::make_pair(result, duration);                                                   \
    })
