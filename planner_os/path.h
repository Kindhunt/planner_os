#pragma once
#include "includes.h"
#define _CRT_SECURE_NO_WARNINGS

// нахуй переделать
namespace my_fs {
    class path
    {
    protected:
        std::vector<std::string> vec = std::vector<std::string>();
        void make_path(std::string& c_path);
    public:
        path();
        path(std::string c_path);

        std::vector<std::string> get_vec();
        path operator+=(std::string arg);
        path operator+(std::string arg);
        path operator--();
        bool operator==(path& other);
        std::string operator[](size_t pos);

        void operator=(path _path);
        void operator=(std::vector<std::string> _vec);
        void operator=(std::string _str);

        std::string get_virtual_path();
        int get_virtual_size() {
            return vec.size();
        }

        std::string get_name_of_current_virtual_obj();
    };


}

