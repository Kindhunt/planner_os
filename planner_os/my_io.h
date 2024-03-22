#pragma once
#include "serializable.h"
#include "file_object.h"
#include "super_block.h"
#include "block.h"
#include "policy.h"

namespace my_fs {
	class my_io	{
	private:
		bool write(std::ostream& os, serializable& __serializable_obj);
		bool read(std::istream& is, serializable& __serializable_obj);
	public:
		template<typename t = serializable>
		t get_item(const char* __file);
		template<typename t = serializable>
		void add_item(const char* __file, t& __serializable_obj);
		template<typename t = serializable>
		void update_item(const char* __file, t& __serializable_obj);
		template<typename t>
		void insert_item(const char* __file, unsigned int id, t& __serializable_obj);
		template<typename t = serializable>
		void erase_item(const char* __file, std::function<bool(t)> __bool_fun);
		//template<typename t = serializable>
		//t find_item(const char* __file, size_t __control_sum);
		template<typename t = serializable>
		t find_item(const char* __file, std::function<bool(t)> __bool_fun);
		template<typename t = serializable, typename u = serializable>
		std::vector<t> get_items(const char* __file, u& __other_obj, std::function<bool(t)> __bool_fun);
		template<typename t = serializable>
		std::vector<t> get_items(const char* __file);
		//std::vector<file_descriptor>& get_dir_entries(file_object& __f_obj);
		//std::vector<group>& get_groups(user __user);
	};

	class file_manager {
	private:
		my_io io = my_io();

		std::vector<unsigned int> allocate_dir(super_block& __super_block, size_t __count);
		std::vector<unsigned int> allocate_file(i_node& __inode,
			path __parent_path, size_t __count);
		void allocate(super_block& __sb, file_object& __file_obj);
		void delete_file(file_object& __file_obj);
		void delete_dir(file_object& __file_obj);
		void copy_file(file_object& __file_obj);
		void copy_dir(file_object& __file_obj);
		/*std::string from_bin(std::string __str);
		std::string to_bin(std::string __bin_str);*/
	public:
		file_object init_system(user __current_user, 
			group __current_group);
		inline super_block get_super_block();
		inline file_object get_file_metadata(path __abs_path);
		std::vector<file_object> get_entries(path __abs_path);
		void create_obj(std::string __name,
			path& __abs_path, object_type __o_type, 
			int __user_id, int __group_id);
		std::string get_data_from_block(int id);

		void delete_obj(path& __abs_path);
		void write_file(path& __abs_path,
			std::string& __data, write_type __write_type);
		void update_file(file_object& __file_obj,
			path& __abs_path);

		/*void update_groups(user __user_upd);*/

		void copy_obj(path __path_for_copy, path __abs_path, std::string __new_name);
		void delete_blocks(int __start, int __end, super_block& __sb);
		//void rename_obj(std::string __new_name, path& __abs_path);
	};	

	class policy_manager {
	private:
		my_io io = my_io();
	public:
		user get_user(int id);
		user get_user(std::string __username);
		group get_group(std::string __name);
		void create_user(user __user);
		void create_group(group __group);
		void update_user(user __user_upd);
		void update_group(group __group_upd);
		void delete_user(int __user_id);
		void delete_group(int __group_id);

	};
}

