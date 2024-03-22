#include "i_node.h"

namespace my_fs {
    i_node::i_node() { }

    i_node& i_node::set_id(unsigned int id) {
        serializable::set_id(id);
        return *this;
    }
    i_node& i_node::set_permissions(int permissions) {
        this->permissions = permissions;
        return *this;
    }
    i_node& i_node::set_size(int size) {
        this->size = size;
        return *this;
    }
    i_node& i_node::set_user_id(int user_id) {
        this->user_id = user_id;
        return *this;
    }
    i_node& i_node::set_group_id(int group_id) {
        this->group_id = group_id;
        return *this;
    }
    i_node& i_node::set_object_type(object_type o_type) {
        this->o_type = o_type;
        return *this;
    }
    i_node& i_node::set_last_modified_at(time_t last_modified_at) {
        this->last_modified_at = last_modified_at;
        return *this;
    }
    i_node& i_node::set_created_time(time_t created_time) {
        this->created_time = created_time;
        return *this;
    }
    i_node& i_node::set_last_access_time(time_t last_access_time) {
        this->last_access_time = last_access_time;
        return *this;
    }
    i_node& i_node::set_data_blocks(std::vector<unsigned int> data_blocks) {
        this->data_blocks = data_blocks;
        return *this;
    }

    int i_node::get_permissions() {
        return permissions;
    }

    int i_node::get_size() {
        return size;
    }

    int& i_node::get_size_link() {
        return size;
    }

    int i_node::get_user_id() {
        return user_id;
    }

    int i_node::get_group_id() {
        return group_id;
    }

    object_type i_node::get_object_type() {
        return o_type;
    }

    time_t i_node::get_last_modified_at() {
        return last_modified_at;
    }

    time_t i_node::get_created_time() {
        return created_time;
    }

    time_t i_node::get_last_access_time() {
        return last_access_time;
    }

    std::vector<unsigned int>& i_node::get_data_blocks() {
        return data_blocks;
    }

    i_node i_node::operator+=(unsigned int id) {
        if (std::find(data_blocks.begin(), data_blocks.end(), id) != data_blocks.end()) {
            data_blocks.push_back(id);
        }
        return *this;
    }

    void i_node::serialize(std::ostream& os) const {
        os.write(reinterpret_cast<const char*>(&id), sizeof(id));
        os.write(reinterpret_cast<const char*>(&permissions), sizeof(permissions));
        os.write(reinterpret_cast<const char*>(&size), sizeof(size));
        os.write(reinterpret_cast<const char*>(&user_id), sizeof(user_id));
        os.write(reinterpret_cast<const char*>(&group_id), sizeof(group_id));
        
        int type = static_cast<int>(o_type);
        os.write(reinterpret_cast<const char*>(&type), sizeof(type));
        os.write(reinterpret_cast<const char*>(&last_modified_at), sizeof(last_modified_at));
        os.write(reinterpret_cast<const char*>(&created_time), sizeof(created_time));
        os.write(reinterpret_cast<const char*>(&last_access_time), sizeof(last_access_time));
        
        size_t data_blocks_size = data_blocks.size();
        os.write(reinterpret_cast<const char*>(&data_blocks_size), sizeof(data_blocks_size));
        if (data_blocks_size > 0) {
            os.write(reinterpret_cast<const char*>(data_blocks.data()), data_blocks_size * sizeof(unsigned int));
        }
    }
    void i_node::deserialize(std::istream& is) {
        is.read(reinterpret_cast<char*>(&id), sizeof(id));
        is.read(reinterpret_cast<char*>(&permissions), sizeof(permissions));
        is.read(reinterpret_cast<char*>(&size), sizeof(size));
        is.read(reinterpret_cast<char*>(&user_id), sizeof(user_id));
        is.read(reinterpret_cast<char*>(&group_id), sizeof(group_id));
        
        int type = 0;
        is.read(reinterpret_cast<char*>(&type), sizeof(type));
        o_type = static_cast<object_type>(type);
        is.read(reinterpret_cast<char*>(&last_modified_at), sizeof(last_modified_at));
        is.read(reinterpret_cast<char*>(&created_time), sizeof(created_time));
        is.read(reinterpret_cast<char*>(&last_access_time), sizeof(last_access_time));
        
        size_t data_blocks_size = 0;
        is.read(reinterpret_cast<char*>(&data_blocks_size), sizeof(data_blocks_size));
        
        if (data_blocks_size > 0) {
            data_blocks.resize(data_blocks_size);
            is.read(reinterpret_cast<char*>(data_blocks.data()), data_blocks_size * sizeof(unsigned int));
        }        
    }
}

