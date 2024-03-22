#pragma once
#include "S5fs.h"
#include <conio.h>
#define _CRT_SECURE_NO_WARNINGS

class my_system
{
private:
	my_fs::S5fs file_system;
	inline bool show_help(std::unordered_map<std::string, std::string> argv);
public:
	std::unordered_map<std::string, std::function<void(int, std::unordered_map<std::string, std::string> map)>> commands;
	std::unique_ptr<my_fs::path> get_path_w_root(std::string& str_path);
	std::unique_ptr<my_fs::path> get_path(std::string& str_path);
	my_system(bool init = true, int size = 0);
	std::string cur_path();
	void init_fs();
	std::string get_user();
	void login();
	bool is_loggined();
};

