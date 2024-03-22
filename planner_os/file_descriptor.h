#pragma once
#include "serializable.h"

namespace my_fs {
    class file_descriptor : public serializable
    {
    private:
        std::string name;
        unsigned int start_offset; // смещение в стартовом блоке
        unsigned int end_offset; // смещение в конечном блоке
    public:
        file_descriptor& set_id(unsigned int id);
        file_descriptor& set_name(std::string name);
        file_descriptor& set_start_offset(unsigned int start_offset);
        file_descriptor& set_end_offset(unsigned int end_offset);

        std::string get_name() const;
        std::string& get_name_link();
        unsigned int get_start_offset();
        unsigned int get_end_offset();

        void serialize(std::ostream& os) const override;
        void deserialize(std::istream& is) override;
    };
}


template<>
struct std::hash<my_fs::file_descriptor> {
    std::size_t operator()(const my_fs::file_descriptor& s) const noexcept {
        return std::hash<std::string>{}(s.get_name());
    }
};
