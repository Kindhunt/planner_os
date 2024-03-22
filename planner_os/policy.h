#pragma once
#include <string>
#include <vector>
#include "serializable.h"
#include <set>

namespace my_fs {
	class user : public serializable {
	private:
		std::string username;
		std::string password;
		std::vector<std::string> files;
		bool is_super;
		bool is_loggined;
	public:
		user();
		user(std::string username, std::string password);

		std::string get_username() const;
		std::string get_password();

		user& set_id(unsigned int id);
		user& set_user(std::string username, std::string password);

		user& try_login(std::string username, std::string password);
		bool get_is_loggined();
		bool get_is_super();
		bool unlogin();

		void add_file_access(std::string file);
		void remove_file_access(std::string file);
		bool is_file_access(std::string file);
		std::vector<std::string> get_files();

		void serialize(std::ostream& os) const override;
		void deserialize(std::istream& is) override;
	};

	class group : public serializable {
	private:
		std::string name;
		std::set<int> users_ids;
	public:
		group();

		group& set_id(unsigned int id);
		group& set_name(std::string name);
		std::string get_name() const;
		std::set<int> get_users_ids();

		void add_user(int id);
		void delete_user(int id);
		bool check_user(user _user);

		void serialize(std::ostream& os) const override;
		void deserialize(std::istream& is) override;
	};
}
template<>
struct std::hash<my_fs::user> {
	std::size_t operator()(const my_fs::user& s) const noexcept {
		return std::hash<std::string>{}(s.get_username());
	}
};

template<>
struct std::hash<my_fs::group> {
	std::size_t operator()(const my_fs::group& s) const noexcept {
		return std::hash<std::string>{}(s.get_name());
	}
};

