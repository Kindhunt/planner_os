#include "policy.h"
using namespace std;

namespace my_fs {
	user::user() {
		username = std::string();
		files = std::vector<std::string>();
		is_super = false;
		is_loggined = false;
	}
	user::user(std::string username, std::string password) : username(username), password(password) {
		is_super = username == "root";
		is_loggined = false;
	}
	std::string user::get_username() const {
		return username;
	}
	std::string user::get_password() {
		return password;
	}
	user& user::set_id(unsigned int id) {
		this->id = id;
		return *this;
	}
	user& user::set_user(std::string username, std::string password) {
		this->username = username;
		this->password = password;
		is_super = username == "root";

		return *this;
	}
	user& user::try_login(std::string username, std::string password) {
		is_loggined = username == get_username() ||
			password == get_password();
		return *this;
	}

	bool user::get_is_loggined() {
		return is_loggined;
	}

	bool user::get_is_super() {
		return is_super;
	}

	bool user::unlogin() {
		if (is_loggined) {
			is_loggined = false;
		}
		return !is_loggined;
	}

	void user::add_file_access(std::string file) {
		files.push_back(file);
	}

	void user::remove_file_access(std::string file) {
		auto pos = std::find(files.begin(), files.end(), file);
		if (pos != std::end(files)) {
			files.erase(pos);
		}
	}

	bool user::is_file_access(std::string file) {
		return std::find(files.begin(), files.end(), file) != std::end(files);
	}

	std::vector<std::string> user::get_files() {
		return files;
	}

	void user::serialize(std::ostream& os) const {
		os.write(reinterpret_cast<const char*>(&id), sizeof(id));
		size_t len = username.size();
		os.write(reinterpret_cast<const char*>(&len), sizeof(len));
		if (len > 0) {
			os.write(username.data(), sizeof(len));
		}
		len = password.size();
		os.write(reinterpret_cast<const char*>(&len), sizeof(len));
		if (len > 0) {
			os.write(password.data(), sizeof(len));
		}
		os.write(reinterpret_cast<const char*>(&is_super), sizeof(is_super));
		os.write(reinterpret_cast<const char*>(&is_loggined), sizeof(is_loggined));
		len = files.size();
		os.write(reinterpret_cast<const char*>(&len), sizeof(len));
		if (len > 0) {
			for (auto i : files) {
				len = i.size();
				os.write(reinterpret_cast<const char*>(&len), sizeof(len));
				if (len > 0) {
					os.write(i.data(), len);
				}
			}
		}
	}

	void user::deserialize(std::istream& is) {
		username.clear();
		password.clear();
		files.clear();
		is.read(reinterpret_cast<char*>(&id), sizeof(id));
		size_t len = 0;
		is.read(reinterpret_cast<char*>(&len), sizeof(len));
		if (len > 0) {
			username.resize(len);
			is.read(username.data(), sizeof(len));
		}
		len = 0;
		is.read(reinterpret_cast<char*>(&len), sizeof(len));
		if (len > 0) {
			password.resize(len);
			is.read(password.data(), sizeof(len));
		}
		is.read(reinterpret_cast<char*>(&is_super), sizeof(is_super));
		is.read(reinterpret_cast<char*>(&is_loggined), sizeof(is_loggined));
		len = 0;
		is.read(reinterpret_cast<char*>(&len), sizeof(len));
		if (len > 0) {
			std::string str;
			size_t str_len = 0;
			for (int i = 0; i < len; i++) {
				is.read(reinterpret_cast<char*>(&str_len), sizeof(str_len));
				if (str_len > 0) {
					str.resize(str_len);
					is.read(str.data(), str_len);
					files.push_back(str);
				}				
			}
		}
	}

	group::group() {
		users_ids = std::set<int>();
	}

	group& group::set_id(unsigned int id) {
		this->id = id;
		return *this;
	}

	group& group::set_name(std::string name) {
		this->name = name;
		return *this;
	}

	std::string group::get_name() const {
		return name;
	}

	std::set<int> group::get_users_ids() {
		return users_ids;
	}

	void group::add_user(int id) {
		users_ids.insert(id);
	}

	void group::delete_user(int id) {
		auto pos = std::find(users_ids.begin(), users_ids.end(), id);
		if (pos != std::end(users_ids)) {
			users_ids.erase(pos);
		}
	}

	bool group::check_user(user _user) {
		return std::find(users_ids.begin(), users_ids.end(), _user.get_id()) != std::end(users_ids);
	}

	void group::serialize(std::ostream& os) const {
		os.write(reinterpret_cast<const char*>(&id), sizeof(id));
		size_t len_name = name.size();
		os.write(reinterpret_cast<const char*>(&len_name), sizeof(len_name));
		if (len_name > 0) {
			os.write(name.data(), sizeof(len_name));
		}
		size_t len_users = users_ids.size();
		os.write(reinterpret_cast<const char*>(&len_users), sizeof(len_users));
		if (len_users > 0) {
			std::vector<int> ids = std::vector<int>(users_ids.begin(), users_ids.end());
			os.write(reinterpret_cast<const char*>(ids.data()), sizeof(int) * len_users);
		}
	}

	void group::deserialize(std::istream& is) {
		users_ids.clear();
		name.clear();
		is.read(reinterpret_cast<char*>(&id), sizeof(id));
		size_t len = 0;
		is.read(reinterpret_cast<char*>(&len), sizeof(len));
		if (len > 0) {
			name.resize(len);
			is.read(name.data(), sizeof(len));
		}
		len = 0;
		is.read(reinterpret_cast<char*>(&len), sizeof(len));
		if (len > 0) {
			std::vector<int> ids = std::vector<int>(len);
			is.read(reinterpret_cast<char*>(ids.data()), sizeof(int) * len);
			users_ids = std::set<int>(ids.begin(), ids.end());
		}
	}
}
