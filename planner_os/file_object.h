#pragma once
#pragma warning(disable : 4996)
#include "i_node.h"
#include "file_descriptor.h"
#include "path.h"

namespace my_fs {
    inline std::string time_str(time_t c_time);
	class file_object
	{
    private:
        i_node current_inode;
        file_descriptor f_description;
        path parent_path;
    
    public:
        file_object();
        file_object(i_node current_inode,
            file_descriptor f_description, path parent_path);

        file_object& set_current_inode(i_node current_inode);
        file_object& set_f_description(file_descriptor f_description);
        file_object& set_parent_path(path parent_path);

        i_node& get_current_inode();
        file_descriptor& get_f_description();
        path get_parent_path();
        path get_path();
	};
    std::ostream& operator << (std::ostream& outs, file_object& __f_obj);

    std::string to_string(file_object& __f_obj);

    inline std::string time_str(time_t c_time);

}

