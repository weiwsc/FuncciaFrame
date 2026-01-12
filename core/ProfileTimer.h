//
// Created by Wangsicong Wei on 2026-01-11.
//

#ifndef PROFILETIMER_H
#define PROFILETIMER_H


#include <chrono>

#define PROFILE_SCOPE(name) ProfileTimer timer##__LINE__(name)

class ProfileTimer {
public:
    ProfileTimer(const char* name) : m_name(name), m_start(std::chrono::high_resolution_clock::now()) {}
    ~ProfileTimer() {
        auto end = std::chrono::high_resolution_clock::now();
        double ms = std::chrono::duration<double, std::milli>(end - m_start).count();
        if (ms > 0.01) {  // only log slow stuff
            printf("%s: %.3fms\n", m_name, ms);
        }
    }
private:
    const char* m_name;
    std::chrono::high_resolution_clock::time_point m_start;
};


#endif //PROFILETIMER_H