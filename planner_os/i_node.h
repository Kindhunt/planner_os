#pragma once
#include <functional>
#include "serializable.h"

namespace my_fs {
    class i_node : public serializable
    {
    private:
        int permissions;
        int size;

        int user_id;
        int group_id;

        object_type o_type;

        time_t last_modified_at;
        time_t created_time;
        time_t last_access_time;

        std::vector<unsigned int> data_blocks;
    public:
        i_node();

        i_node& set_id(unsigned int id);
        i_node& set_permissions(int permissions);
        i_node& set_size(int size);
        i_node& set_user_id(int user_id);
        i_node& set_group_id(int group_id);
        i_node& set_object_type(object_type o_type);
        i_node& set_last_modified_at(time_t last_modified_at);
        i_node& set_created_time(time_t created_time);
        i_node& set_last_access_time(time_t last_access_time);
        i_node& set_data_blocks(std::vector<unsigned int> data_blocks);

        int get_permissions();
        int get_size();
        int& get_size_link();
        int get_user_id();
        int get_group_id();
        object_type get_object_type();
        time_t get_last_modified_at();
        time_t get_created_time();
        time_t get_last_access_time();
        std::vector<unsigned int>& get_data_blocks();

        i_node operator+=(unsigned int id);

        void serialize(std::ostream& os) const override;
        void deserialize(std::istream& is) override;
    };
}

template<>
struct std::hash<my_fs::i_node> {
    std::size_t operator()(const my_fs::i_node& s) const noexcept {
        return std::hash<unsigned int>{}(s.get_id());
    }
};

