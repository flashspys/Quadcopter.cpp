//
// Created by Felix Wehnert on 12.03.2017.
//

#ifndef QUADROCOPTER_LOG_HPP
#define QUADROCOPTER_LOG_HPP

#include <string>

class Log {
public:
    static bool quite;

    /*
     * Default log funciton. Logs something, then \n
     */
    template<typename... Args>
    static void log(Args... args) {
        if (!Log::quite) {
            std::fprintf(stdout, args...);
            std::fprintf(stdout, "\n");
        }
    }

    /*
     * logs without \n
     */
    template<typename... Args>
    static void logString(Args... args) {
        if (!Log::quite) {
            std::fprintf(stdout, args...);
        }
    }

};


#endif //QUADROCOPTER_LOG_HPP
