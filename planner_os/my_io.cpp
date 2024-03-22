#include "my_io.h"
#include <cassert>

bool my_fs::my_io::write(std::ostream& os, serializable& __serializable_obj) {
	bool eof = os.eof();
	__serializable_obj.serialize(os);
	return !os.fail();
}

bool my_fs::my_io::read(std::istream& is, serializable& __serializable_obj) {
	__serializable_obj.deserialize(is);
	return !is.eof();
}

template<typename t>
t my_fs::my_io::get_item(const char* __file) {
	t __obj = t();
	
	std::ifstream is(__file, std::ios::binary | std::ios::in);
	read(is, __obj);
	is.close();
	
	return __obj;
}

template<typename t>
void my_fs::my_io::add_item(const char* __file, t& __serializable_obj) {
	std::vector<t> items = get_items<t>(__file);
	__serializable_obj.set_id(items.size());
	std::ofstream os(__file, std::ios::binary | std::ios::app);
	
	write(os, __serializable_obj);

	os.close();
}

template<typename t>
void my_fs::my_io::update_item(const char* __file, t& __serializable_obj) {
	t __obj;
	std::string __temp = "temp.bin";
	auto id = __serializable_obj.get_id();
	
	std::ifstream is(__file, std::ios::binary | std::ios::in);
	std::ofstream os(__temp, std::ios::binary | std::ios::out);

	int curr = 0;
	while (read(is, __obj)) {
		if (curr != id) {
			write(os, __obj);
		} else {
			write(os, __serializable_obj);
		}
		++curr;
	}
	
	os.close();
	is.close();

	remove(__file);
	rename(__temp.c_str(), __file);
}

template<typename t>
void my_fs::my_io::insert_item(const char* __file, unsigned int id, t& __serializable_obj)
{
	t __obj;

	std::string __temp = "temp.bin";

	std::ifstream is(__file, std::ios::binary | std::ios::in);
	std::ofstream os(__temp, std::ios::binary | std::ios::out);

	int curr = 0;
	while (read(is, __obj)) {
		if (curr != id) {
			write(os, __obj);
		}
		else {
			write(os, __serializable_obj);
		}
		curr++;
	}

	os.close();
	is.close();

	remove(__file);
	rename(__temp.c_str(), __file);
}

template<typename t>
void my_fs::my_io::erase_item(const char* __file, std::function<bool(t)> __bool_fun) {
	t __obj;
	std::string __temp = "temp.bin";

	std::ifstream is(__file, std::ios::binary | std::ios::in);
	std::ofstream os(__temp, std::ios::binary | std::ios::out);

	int curr = 0;
	while (read(is, __obj)) {
		if (!__bool_fun(__obj)) {
			__obj.set_id(curr);
			write(os, __obj);
			curr++;
		}
	}

	os.close();
	is.close();

	remove(__file);
	rename(__temp.c_str(), __file);
}

template<typename t>
t my_fs::my_io::find_item(const char* __file, std::function<bool(t)> __bool_fun) {
	t __obj = t();

	std::ifstream is(__file, std::ios::binary | std::ios::in);

	while (read(is, __obj)) {
		if (__bool_fun(__obj)) {
			is.close();

			return __obj;
		}
	}

	is.close();

	return __obj;
}

template<typename t, typename u>
std::vector<t> my_fs::my_io::get_items(const char* __file, u& __other_obj, std::function<bool(t)> __bool_fun) {
	t __t_obj = t();
	std::vector<t> to_ret = std::vector<t>();

	std::ifstream is(__file, std::ios::binary | std::ios::in);

	while (read(is, __t_obj)) {
		if (__bool_fun(__t_obj)) {
			to_ret.push_back(__t_obj);
		}
	}

	is.close();

	return to_ret;
}

template<typename t>
std::vector<t> my_fs::my_io::get_items(const char* __file) {
	t __obj = t();
	std::vector<t> to_ret = std::vector<t>();

	std::ifstream is(__file, std::ios::binary | std::ios::in);

	while (read(is, __obj)) {
		to_ret.push_back(__obj);
	}

	is.close();

	return to_ret;
}

void my_fs::file_manager::create_obj(std::string __name, path& __abs_path,
	object_type __o_type, int __user_id, int __group_id) {
	file_object __file_obj = file_object();
	auto __sb = get_super_block();

	__file_obj.set_parent_path(__abs_path);
	__file_obj.get_current_inode()
		.set_last_access_time(time(nullptr))
		.set_last_modified_at(time(nullptr))
		.set_created_time(time(nullptr))
		.set_group_id(__group_id)
		.set_user_id(__user_id)
		.set_size(0)
		.set_permissions(DEFAULT_FILE_PERMS)
		.set_object_type(__o_type);
	__file_obj.set_f_description(__file_obj.get_f_description()
		.set_name((__abs_path + __name).get_virtual_path()));
	__file_obj.get_current_inode().get_data_blocks() = std::vector<unsigned int>();

	allocate(__sb, __file_obj);
}

std::string my_fs::file_manager::get_data_from_block(int id) {
	block _block = io.find_item<block>(_data_path.c_str(), [&](block b) {
		return b.get_id() == id;
		});
	return _block.get_data_vector();
}


my_fs::file_object my_fs::file_manager::init_system(user __current_user, group __current_group) {
	super_block __sb = super_block(true);
	io.add_item(_sb_path.c_str(), __sb);

	file_object __root = file_object();
	__root.get_current_inode()
		.set_last_access_time(time(nullptr))
		.set_last_modified_at(time(nullptr))
		.set_created_time(time(nullptr))
		.set_group_id(__current_user.get_id())
		.set_user_id(__current_group.get_id())
		.set_size(0)
		.set_permissions(0000)
		.set_object_type(object_type::DIR);
	__root.set_f_description(__root.get_f_description()
		.set_name(std::string("root")));
	allocate(__sb, __root);

	__current_user.add_file_access(__root.get_path().get_virtual_path());

	io.update_item(_users_path.c_str(), __current_user);
	io.update_item(_groups_path.c_str(), __current_group);

	return __root;
}

void my_fs::file_manager::delete_obj(path& __abs_path) {
	auto __file_obj = get_file_metadata(__abs_path);
	__file_obj.get_current_inode().get_object_type() == object_type::DIR ?
		delete_dir(__file_obj) : delete_file(__file_obj);	
}

void my_fs::file_manager::delete_file(file_object& __file_obj) {
	
	auto blocks = __file_obj.get_current_inode().get_data_blocks();
	auto eof = __file_obj.get_current_inode().get_data_blocks().empty() ?
		*(__file_obj.get_current_inode().get_data_blocks().begin()) :
		__file_obj.get_f_description().get_end_offset();
	auto __sb = get_super_block();

	delete_blocks(__file_obj.get_f_description().get_start_offset(), eof, __sb);

	__sb[__file_obj.get_current_inode()] = '0';
	
	io.erase_item<i_node>(_inode_path.c_str(), [&](i_node i) {
		return i.get_id() == __file_obj.get_current_inode().get_id();
		});
	io.erase_item<file_descriptor>(_fd_path.c_str(), [&](file_descriptor fd) {
		return fd.get_id() == __file_obj.get_f_description().get_id();
		});
}

void my_fs::file_manager::delete_dir(file_object& __file_obj) {
	std::vector<file_descriptor> __dir_entries = io.get_items<file_descriptor, file_object>(_fd_path.c_str(), __file_obj,
		[&](file_descriptor __fd) -> bool {
			return __fd.get_name().starts_with(__file_obj.get_f_description().get_name())
				&& __fd.get_name() != __file_obj.get_f_description().get_name()
				&& std::ranges::count(__file_obj.get_f_description().get_name(), '\\') + 1 == std::ranges::count(__fd.get_name(), '\\');
		});

	for (auto _fd : __dir_entries) {
		auto _path = path(_fd.get_name());
		delete_obj(_path);
	}
	auto __sb = get_super_block();

	delete_blocks(__file_obj.get_f_description().get_start_offset(), __file_obj.get_f_description().get_end_offset(), __sb);

	__sb[__file_obj.get_current_inode()] = '0';

	io.update_item<super_block>(_sb_path.c_str(), __sb);
	io.erase_item<i_node>(_inode_path.c_str(), [&](i_node i) {
		return i.get_id() == __file_obj.get_current_inode().get_id();
		});
	io.erase_item<file_descriptor>(_fd_path.c_str(), [&](file_descriptor fd) {
		return fd.get_id() == __file_obj.get_f_description().get_id();
		});
}

void my_fs::file_manager::copy_file(file_object& __file_obj) {

}

void my_fs::file_manager::copy_dir(file_object& __file_obj)
{
}

my_fs::file_object my_fs::file_manager::get_file_metadata(path __abs_path) {
	file_object __file_obj = file_object();

	auto __fd = io.find_item<file_descriptor>(_fd_path.c_str(), [&](file_descriptor fd) {
		return fd.get_name() == __abs_path.get_virtual_path();
		});
	auto __inode = io.find_item<i_node>(_inode_path.c_str(), [&](i_node inode) {
		return inode.get_id() == __fd.get_id();
		});
	
	__file_obj.set_f_description(__fd)
		.set_parent_path(--__abs_path)
		.set_current_inode(__inode);
	
	return __file_obj;
}

std::vector<my_fs::file_object> my_fs::file_manager::get_entries(path __abs_path) {
	std::vector<file_object> to_ret = std::vector<file_object>();
	file_object __file_obj = get_file_metadata(__abs_path);
	 std::vector<file_descriptor> __dir_entries = io.get_items<file_descriptor, file_object>(_fd_path.c_str(), __file_obj,
		[&](file_descriptor __fd) -> bool {
			path p = path(__fd.get_name());
			if (__file_obj.get_path().get_virtual_size() >= p.get_virtual_size() || 
				p.get_virtual_size() > __file_obj.get_path().get_virtual_size() + 1) {
				return false;
			}
			for (int i = 0; i < __file_obj.get_path().get_virtual_size(); i++) {
				if (__file_obj.get_path()[i] != p[i]) 
					return false;
			}
			return true;
			/*return __fd.get_name().starts_with(__file_obj.get_f_description().get_name())
				&& __fd.get_name() != __file_obj.get_f_description().get_name()
				&& std::ranges::count(__file_obj.get_f_description().get_name(), '\\') + 1 == std::ranges::count(__fd.get_name(), '\\');*/
		});
	 for (auto _f_obj : __dir_entries) {
		auto _path = path(_f_obj.get_name());
		to_ret.push_back(get_file_metadata(_path));
	}
	return to_ret;
}

void my_fs::file_manager::write_file(path& __abs_path,
	std::string& __data, write_type __write_type) {
	auto __file_obj = get_file_metadata(__abs_path);
	
	auto data_it = __data.begin();
	std::string __temp;
	auto curr = 0;
	for (auto count_blocks = (__data.size() / BLOCK_SIZE) + 1;
		__file_obj.get_current_inode().get_data_blocks().size() > 0 && count_blocks > 0; --count_blocks, ++curr) {
		auto __block = io.find_item<block>(_data_path.c_str(), [&](block b) {
			return !b.is_full() 
				&& __file_obj.get_f_description().get_start_offset() <= b.get_id() 
				&& __file_obj.get_f_description().get_end_offset() >= b.get_id();
			});
		auto pos = std::find(__file_obj.get_current_inode().get_data_blocks().begin(),
			__file_obj.get_current_inode().get_data_blocks().end(), __block.get_id());
		if (pos != __file_obj.get_current_inode().get_data_blocks().end()) {
			__file_obj.get_current_inode().get_data_blocks().erase(pos);
		}
		
		__temp = std::string(data_it, data_it + __data.size() % (BLOCK_SIZE - __block.get_data_vector().size()));
		__file_obj.get_current_inode().get_size_link() += __temp.size();
		
		__block.write_data(__temp, __write_type);
		io.update_item<block>(_data_path.c_str(), __block);

		__data = __temp;
		data_it = __data.begin();
	}
	
	__file_obj.get_current_inode()
		.set_last_access_time(time(nullptr))
		.set_last_modified_at(time(nullptr));

	auto __sb = get_super_block();
	__sb -= __file_obj.get_current_inode().get_size();
	
	io.update_item<super_block>(_sb_path.c_str(), __sb);
	io.update_item<i_node>(_inode_path.c_str(), __file_obj.get_current_inode());
}

void my_fs::file_manager::update_file(file_object& __file_obj, path& __abs_path) {
	file_object __file_obj_to_upd = get_file_metadata(__abs_path);
	
	i_node inode = __file_obj.get_current_inode();
	file_descriptor fd = __file_obj.get_f_description();
	
	//__file_obj_to_upd.get_current_inode()
	//	.set_group_id(inode.get_group_id())
	//	.set_permissions(inode.get_permissions())
	//	.set_user_id(inode.get_user_id())
	//	.set_last_modified_at(inode.get_last_modified_at())
	//	.set_last_access_time(inode.get_last_access_time());

	io.update_item<i_node>(_inode_path.c_str(), inode);

	if (__file_obj_to_upd.get_current_inode().get_object_type() == object_type::DIR) {
		std::vector<file_descriptor> __dir_entries = io.get_items<file_descriptor, file_object>(_fd_path.c_str(), __file_obj_to_upd,
			[&](file_descriptor __fd) -> bool {
				return __fd.get_name().starts_with(__file_obj_to_upd.get_f_description().get_name())
					&& __fd.get_name() != __file_obj_to_upd.get_f_description().get_name();
			});
		path _p = path(__file_obj.get_f_description().get_name());
		for (auto item : __dir_entries) {
			item.get_name_link() = std::regex_replace(item.get_name(), std::regex(__file_obj_to_upd.get_path().get_name_of_current_virtual_obj()),
				_p.get_name_of_current_virtual_obj());
			io.update_item<file_descriptor>(_fd_path.c_str(), item);
		}
	}
	io.update_item<file_descriptor>(_fd_path.c_str(), fd);
}

void my_fs::file_manager::copy_obj(path __path_for_copy, path __abs_path, std::string __new_name) {
	path __path = __path_for_copy;
	auto __sb = get_super_block();

	file_object __file_obj = get_file_metadata(__abs_path);
	
	user __user = io.find_item<user>(_users_path.c_str(), [&](user u) {
		return u.get_id() == __file_obj.get_current_inode().get_user_id();
		});
	group __group = io.find_item<group>(_groups_path.c_str(), [&](group g) {
		return g.get_id() == __file_obj.get_current_inode().get_group_id();
		});
	create_obj(__new_name, __path_for_copy, __file_obj.get_current_inode().get_object_type(), 
		__user.get_id(), __group.get_id());
	__path += __new_name;
	
	file_object __cpy_obj = get_file_metadata(__path);

	if (__cpy_obj.get_current_inode().get_object_type() == object_type::DIR) {
		auto __entries = get_entries(__abs_path);
		for (auto __entry : __entries) {
			copy_obj(__path, __entry.get_path(), __entry.get_path().get_name_of_current_virtual_obj());
		}
	}
	else {
		auto eof = !__file_obj.get_current_inode().get_data_blocks().empty() ?
			*(__file_obj.get_current_inode().get_data_blocks().begin()) :
			__file_obj.get_f_description().get_end_offset();

		for (auto curr = __file_obj.get_f_description().get_start_offset(); curr < eof; curr++) {
			auto __block = io.find_item<block>(_data_path.c_str(), [&](block b) {
				return b.get_id() == curr;
				});
			write_file(__path, __block.get_data_vector(), write_type::APPEND);
		}
	}	
}

void my_fs::file_manager::delete_blocks(int __start, int __end, super_block& __sb) {
	for (auto current_offset = __start;
		current_offset < __end; current_offset++) {
		auto __block = io.find_item<block>(_data_path.c_str(), [&](block b) {
			return b.get_id() == current_offset;
			});

		if (__block.get_data_vector().size() > 0) {
			__sb += __block.get_data_vector().size();
			io.erase_item<block>(_data_path.c_str(), [&](block b) {
				return __block.get_id() == b.get_id();
				});
			__sb[__block] = '0';
		}
	}
}

void my_fs::file_manager::allocate(super_block& __sb, file_object& __file_obj) {
	path __parent_path = __file_obj.get_parent_path();
	i_node __inode = __file_obj.get_current_inode();

	__file_obj.get_current_inode().set_data_blocks(
		__file_obj.get_current_inode().get_object_type() == object_type::DIR ?
		allocate_dir(__sb, MAX_COUNT_OF_OBJ) :
		allocate_file(__inode, __parent_path, MAX_CONTENT_LENGTH));

	__inode.set_id(std::distance(__sb.get_inode_bitmap().begin(), std::find(__sb.get_inode_bitmap().begin(), __sb.get_inode_bitmap().end(), '0')));
	__sb[__inode] = '1';
	__file_obj.get_f_description().set_id(__inode.get_id())
		.set_start_offset(__file_obj.get_current_inode().get_data_blocks().front())
		.set_end_offset(__file_obj.get_current_inode().get_data_blocks().back());

	io.update_item<super_block>(_sb_path.c_str(), __sb);
	io.add_item<i_node>(_inode_path.c_str(), __file_obj.get_current_inode());
	io.add_item<file_descriptor>(_fd_path.c_str(), __file_obj.get_f_description());
}

std::vector<unsigned int> my_fs::file_manager::allocate_dir(super_block& __super_block, size_t __count) {
	std::vector<unsigned int> to_ret = std::vector<unsigned int>();
	int index = 0;
	for (std::vector<char>::iterator it = __super_block.get_block_bitmap().begin();
		it != __super_block.get_block_bitmap().end() && __count > 0; ++it, ++index) {
		if (*it == '0') {
			*it = '1';
			to_ret.push_back(index);
			block b = block();
			io.add_item<block>(_data_path.c_str(), b);
			--__count;
		}
	}

	return to_ret;
}

std::vector<unsigned int> my_fs::file_manager::allocate_file(i_node& __inode, path __parent_path, size_t __count) {
	std::vector<unsigned int> to_ret = std::vector<unsigned int>();
	file_object __parent_object = get_file_metadata(__parent_path);
	for (;__count > 0;__count--) {
		to_ret.push_back(__parent_object.get_current_inode().get_data_blocks().front());
		__parent_object.get_current_inode().get_data_blocks().erase(
			__parent_object.get_current_inode().get_data_blocks().begin()
		);
	}
	__parent_object.get_f_description().set_start_offset(
		__parent_object.get_current_inode().get_data_blocks().front()
	).set_end_offset(__parent_object.get_current_inode().get_data_blocks().back());

	io.update_item<i_node>(_inode_path.c_str(), __parent_object.get_current_inode());
	io.update_item<file_descriptor>(_fd_path.c_str(), __parent_object.get_f_description());
	return to_ret;
}

inline my_fs::super_block my_fs::file_manager::get_super_block() {
	return io.get_item<super_block>(_sb_path.c_str());
}

void my_fs::policy_manager::create_user(user __user) {
	io.add_item(_users_path.c_str(), __user);
}

void my_fs::policy_manager::create_group(group __group) {
	io.add_item(_groups_path.c_str(), __group);
}

void my_fs::policy_manager::delete_user(int __user_id) {
	io.erase_item<user>(_users_path.c_str(), [&](user u) {
		return u.get_id() == __user_id;
		});
}

void my_fs::policy_manager::delete_group(int __group_id) {
	io.erase_item<group>(_groups_path.c_str(), [&](group g) {
		return g.get_id() == __group_id;
		});
}

my_fs::user my_fs::policy_manager::get_user(int id) {
	return io.find_item<user>(_users_path.c_str(), [&](user _user) -> bool {
		return _user.get_id() == id;
		});
}

my_fs::user my_fs::policy_manager::get_user(std::string __username) {
	return io.find_item<user>(_users_path.c_str(), [&](user _user) -> bool {
		return _user.get_username() == __username;
		});
}

my_fs::group my_fs::policy_manager::get_group(std::string __name) {
	return io.find_item<group>(_groups_path.c_str(), [&](group _group) -> bool {
		return _group.get_name() == __name;
		});
}

void my_fs::policy_manager::update_user(user __user_upd) {
	io.update_item<user>(_users_path.c_str(), __user_upd);
}

void my_fs::policy_manager::update_group(group __group_upd) {
	io.update_item<group>(_groups_path.c_str(), __group_upd);
}
