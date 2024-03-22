#include "file_object.h"

namespace my_fs {
    file_object::file_object() {
        f_description = file_descriptor();
        current_inode = i_node();
    }

    file_object::file_object(i_node current_inode,
        file_descriptor f_description, path parent_path) {
        this->current_inode = current_inode;
        this->f_description = f_description;
        this->parent_path = parent_path;
    }

    file_object& file_object::set_current_inode(i_node current_inode) {
        this->current_inode = current_inode;
        return *this;
    }
    file_object& file_object::set_f_description(file_descriptor f_description) {
        this->f_description = f_description;
        return *this;
    }
    file_object& file_object::set_parent_path(path parent_path) {
        this->parent_path = parent_path;
        return *this;
    }

    i_node& file_object::get_current_inode() {
        return current_inode;
    }
    file_descriptor& file_object::get_f_description() {
        return f_description;
    }
    path file_object::get_parent_path() {
        return parent_path;
    }

    path file_object::get_path() {
        return path(f_description.get_name());
    }

    std::ostream& operator << (std::ostream& outs, file_object& __f_obj)
    {
        std::string type = __f_obj.get_current_inode().get_object_type() == object_type::DIR ? "d" : "-";
        return outs << __f_obj.get_current_inode().get_id() << ' ' <<
            path(__f_obj.get_f_description().get_name()).get_name_of_current_virtual_obj() << ' ' <<
            type << ' ' <<
            __f_obj.get_current_inode().get_permissions() << ' ' <<
            __f_obj.get_current_inode().get_size() << ' ' <<
            __f_obj.get_current_inode().get_user_id() << ' ' <<
            __f_obj.get_current_inode().get_group_id() << ' ' <<
            time_str(__f_obj.get_current_inode().get_created_time()) << ' ' <<
            time_str(__f_obj.get_current_inode().get_last_access_time()) << ' ' <<
            time_str(__f_obj.get_current_inode().get_last_modified_at());
    }

    std::string to_string(file_object& __f_obj) {
        std::ostringstream oss;
        oss << __f_obj;
        return oss.str();
    }

    inline std::string time_str(time_t c_time) {
        std::ostringstream oss;
        std::tm* timeinfo = std::localtime(&c_time);
        oss << std::put_time(timeinfo, "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }
}