#pragma once
#include "super_block.h"
#include "block.h"
#include "file_object.h"
#include "i_node.h"
#include "file_descriptor.h"
#include "file_object.h"
#include "path.h"
#include "my_io.h"
#include "process_manager.h"
#include "policy.h"

#define _CRT_SECURE_NO_WARNINGS

namespace my_fs {
    class S5fs
    {
    private:
        user current_user;
        group current_group;

        std::vector<std::string> users;
        std::vector<std::string> groups;

        file_manager f_manager;
        process_manager pr_manager;
        policy_manager po_manager;

        // std::function<file_object(path)> init_object_file;
        file_object current_opened_file;

    private:
        S5fs(); // V
        S5fs(bool init); // V
    public:
        S5fs(bool init, int size); // V

        //files
        void create_obj(std::string name, path& abs_path, object_type type = object_type::DIR);
        file_object open_obj(path abs_path);
        void write_obj(std::string& data, path abs_path, write_type w_type = write_type::TRUNC);
        void print_obj(file_object& file);
        void delete_obj(path& abs_path);
        void copy_obj(path& path_for_copy, path& abs_path);
        void rename_obj(std::string new_name, path abs_path);
        void mov_obj(path& path_to_mov, path& abs_path);
        void change_permissions(int new_permissions, path& abs_path);
        void print_file_metadata(file_object& file);
        std::string check_copy(path abs_path, std::string name);
        std::string get_file_metadata(path abs_path);
        std::vector<file_object> get_dir(path abs_path);
        void print_metadata_table();
        path get_current_path();

        //policy
        bool try_exit();
        bool try_login(user user_check);
        bool is_loggined(std::string username);
        bool is_loggined();

        user get_user(std::string username);
        group get_group(std::string name);
        void create_user(user user_create);
        void create_group(group group_create);
        void update_user(user user_upd);
        void update_group(group group_upd);
        void set_current_user(user current_user);
        void set_current_group(std::string name);
        void delete_user(std::string name);
        void delete_group(std::string name);
        void print_users();
        void print_groups();
        std::string get_current_user();
    };
}


