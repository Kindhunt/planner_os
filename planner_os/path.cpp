#include "path.h"

namespace my_fs {
    void path::make_path(std::string& c_path) {
        std::stringstream str_path(c_path);
        std::string segment;
        if (!vec.empty()) {
            vec.clear();
        }
        while (std::getline(str_path, segment, '\\')) {
            vec.push_back(segment);
        }
    }

    std::vector<std::string> path::get_vec() {
        return vec;
    }
    
    path::path() { }

    path::path(std::string c_path) {
        make_path(c_path);
    }

    path path::operator+(std::string arg) {
        vec.push_back(arg);
        return *this;
    }

    path path::operator+=(std::string arg) {
        vec.push_back(arg);
        return *this;
    }

    path path::operator--() {
        if (vec.size() == 0) {
            return *this;
        }
        vec.pop_back();
        return *this;
    }

    bool path::operator==(path& other) {
        auto other_vec = other.get_vec();
        bool is_same = vec.size() == other_vec.size();

        for (auto it_other = other_vec.begin(), it = vec.begin();
            it_other != other_vec.end() && it != vec.end() && is_same;
            ++it_other, ++it) {
            is_same = *it_other == *it;
        }

        return is_same;
    }

    std::string path::operator[](size_t pos) {
        return vec.at(pos);
    }

    void path::operator=(path _path) {
        vec = _path.get_vec();
    }

    void path::operator=(std::vector<std::string> _vec) {
        std::vector<std::string>::iterator it = _vec.begin();
        vec.clear();
        do {
            vec.push_back(*it);
        } while (it != _vec.end());
    }

    void path::operator=(std::string _str) {
        make_path(_str);
    }

    std::string path::get_virtual_path() {
        if (vec.empty()) {
            return std::string(1, '\0');
        }

        std::vector<std::string>::iterator it = vec.begin();
        std::string str_path = std::string();

        while (it != vec.end() - 1) {
            str_path += *it + "\\";
            ++it;
        }
        str_path += *it;
        return str_path;
    }

    std::string path::get_name_of_current_virtual_obj() {
        return vec.back();
    }
}

