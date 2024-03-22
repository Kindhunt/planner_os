#pragma once
#include "super_block.h"
#define _CRT_SECURE_NO_WARNINGS
//
//template<typename T, typename... Args>
//class logger {
//private:
//    static std::unordered_map<std::string, std::function<std::string()>> log_map;
//
//public:
//
//    static void add_type(std::string name, std::function<std::string()>& fun) {
//        log_map[name] = fun;
//    }
//    static void log(const T& arg, const Args&... args) {
//        std::ofstream ofs(MY_LOGS + "logs.txt", std::ios_base::app);
//        char buffer[26];
//        time_t log_time = time(0);
//        ctime_s(buffer, sizeof(buffer), &log_time);
//        ofs << "[" << std::string(buffer) << "]" << log_map[arg]();
//        std::cout << "[" << std::string(buffer) << "]" << log_map[arg]();
//
//        printArgs(ofs, args...);
//        printArgs(std::cout, args...);
//
//        ofs << std::endl;
//        std::cout << std::endl;
//    }
//private:
//    // Рекурсивная функция для вывода остальных аргументов
//    template<typename Arg, typename... Rest>
//    static void printArgs(std::ostream& os, const Arg& arg, const Rest&... rest) {
//        // Выводим текущий аргумент
//        os << ' ' << arg;
//
//        // Рекурсивно вызываем printArgs для остальных аргументов
//        printArgs(os, rest...);
//    }
//
//    // Базовый случай для завершения рекурсии
//    static void printArgs(std::ostream& os) {}
//
//};
//
