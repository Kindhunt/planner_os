#include "my_system.h"
using namespace stdext;

my_system::my_system(bool init, int size) : file_system(init, size) {
	init_fs();
}

void my_system::init_fs() {

	commands = std::unordered_map<std::string, std::function<void(int, std::unordered_map<std::string, std::string>)>>();

	commands["help"] = [](int, std::unordered_map<std::string, std::string>) -> void { // вывести все команды
		std::cout << std::format("{:10}", "login") << std::format("{:10}", "-") << std::format("{:10}", "войти") << std::endl;
		std::cout << std::format("{:10}", "exit") << std::format("{:10}", "-") << std::format("{:10}", "выйти") << std::endl;
		std::cout << std::format("{:10}", "create") << std::format("{:10}", "-") << std::format("{:10}", "создать новый файл") << std::endl;
		std::cout << std::format("{:10}", "copy") << std::format("{:10}", "-") << std::format("{:10}", "копировать файл") << std::endl;
		std::cout << std::format("{:10}", "delete") << std::format("{:10}", "-") << std::format("{:10}", "удалить файл") << std::endl;
		std::cout << std::format("{:10}", "rename") << std::format("{:10}", "-") << std::format("{:10}", "переименовать файл") << std::endl;
		std::cout << std::format("{:10}", "chmod") << std::format("{:10}", "-") << std::format("{:10}", "изменить аттрибуты файла") << std::endl;
		std::cout << std::format("{:10}", "ls") << std::format("{:10}", "-") << std::format("{:10}", "показать директорию") << std::endl;
		std::cout << std::format("{:10}", "info") << std::format("{:10}", "-") << std::format("{:10}", "информация о файле") << std::endl;
		std::cout << std::format("{:10}", "open") << std::format("{:10}", "-") << std::format("{:10}", "открыть файл") << std::endl;
		std::cout << std::format("{:10}", "write") << std::format("{:10}", "-") << std::format("{:10}", "запись в файл") << std::endl;
		std::cout << std::format("{:10}", "cls") << std::format("{:10}", "-") << std::format("{:10}", "очистить экран") << std::endl;
		std::cout << std::format("{:10}", "cuser") << std::format("{:10}", "-") << std::format("{:10}", "создать юзера") << std::endl;
		std::cout << std::format("{:10}", "cgroup") << std::format("{:10}", "-") << std::format("{:10}", "создать группу") << std::endl;
		std::cout << std::format("{:10}", "duser") << std::format("{:10}", "-") << std::format("{:10}", "удалить юзера") << std::endl;
		std::cout << std::format("{:10}", "dgroup") << std::format("{:10}", "-") << std::format("{:10}", "удалить группу") << std::endl;
		std::cout << std::format("{:10}", "luser") << std::format("{:10}", "-") << std::format("{:10}", "вывести список юзеров") << std::endl;
		std::cout << std::format("{:10}", "lgroup") << std::format("{:10}", "-") << std::format("{:10}", "вывести список групп") << std::endl;
		std::cout << std::format("{:10}", "uuser") << std::format("{:10}", "-") << std::format("{:10}", "обновить юзера") << std::endl;
		std::cout << std::format("{:10}", "ugroup") << std::format("{:10}", "-") << std::format("{:10}", "обновить группу") << std::endl;
		std::cout << std::format("{:10}", "swus") << std::format("{:10}", "-") << std::format("{:10}", "сменить пользователя") << std::endl;
		};

	commands["login"] = [this](int argc, std::unordered_map<std::string, std::string> argv) -> void {
		
		if (argc % 2 == 1 && argc > 8 && argc == 0) {
			std::cout << "bad args" << std::endl;
			return;
		}
		if (show_help(argv)) {
			std::cout << "войти в систему" << std::endl;
			std::cout << std::format("{:5}", "-n") << std::format("{:5}", "-") << std::format("{:5}", "имя юзера") << std::endl;
			std::cout << std::format("{:5}", "-p") << std::format("{:5}", "-") << std::format("{:5}", "пароль") << std::endl;
			std::cout << std::endl;
			return;
		}

		std::string _str_n = argv.find("-n") != argv.end() ? argv.find("-n")->second : std::string();
		std::string _str_p = argv.find("-p") != argv.end() ? argv.find("-p")->second : std::string();

		auto _user = my_fs::user(_str_n, _str_p);
		if (file_system.try_login(_user)) {
			file_system.set_current_user(_user);
		}

		};
	commands["exit"] = [this](int argc, std::unordered_map<std::string, std::string> argv) -> void {
		if (argc % 2 == 1 && argc > 8 && argc == 0) {
			std::cout << "bad args" << std::endl;
			return;
		}
		if (show_help(argv)) {
			std::cout << "выйти из системы" << std::endl;
			std::cout << std::endl;
			return;
		}

		file_system.try_exit();

		};
	commands["cuser"] = [this](int argc, std::unordered_map<std::string, std::string> argv) -> void {
		if (argc % 2 == 1 && argc > 8 && argc == 0) {
			std::cout << "bad args" << std::endl;
			return;
		}
		if (show_help(argv)) {
			std::cout << "создать юзера" << std::endl;
			std::cout << std::format("{:5}", "-n") << std::format("{:5}", "-") << std::format("{:5}", "имя нового юзера") << std::endl;
			std::cout << std::format("{:5}", "-p") << std::format("{:5}", "-") << std::format("{:5}", "пароль") << std::endl;
			return;
		}
		
		std::string _str_n = argv.find("-n") != argv.end() ? argv.find("-n")->second : std::string();
		std::string _str_p = argv.find("-p") != argv.end() ? argv.find("-p")->second : std::string();
		file_system.create_user(my_fs::user(_str_n, _str_p));

		};
	commands["cgroup"] = [this](int argc, std::unordered_map<std::string, std::string> argv) -> void {
		if (argc % 2 == 1 && argc > 8 && argc == 0) {
			std::cout << "bad args" << std::endl;
			return;
		}
		if (show_help(argv)) {
			std::cout << "создать группу" << std::endl;
			std::cout << std::format("{:5}", "-n") << std::format("{:5}", "-") << std::format("{:5}", "имя новой группы") << std::endl;
			std::cout << std::endl;
			return;
		}
		
		std::string _str_n = argv.find("-n") != argv.end() ? argv.find("-n")->second : std::string();
		auto gr = my_fs::group();
		file_system.create_group(gr.set_name(_str_n));

		};
	commands["duser"] = [this](int argc, std::unordered_map<std::string, std::string> argv) -> void {
		if (argc % 2 == 1 && argc > 8 && argc == 0) {
			std::cout << "bad args" << std::endl;
			return;
		}
		if (show_help(argv)) {
			std::cout << "удалить юзера" << std::endl;
			std::cout << std::format("{:5}", "-n") << std::format("{:5}", "-") << std::format("{:5}", "имя юзера") << std::endl;
			std::cout << std::endl;
			return;
		}
		std::string _str_n = argv.find("-n") != argv.end() ? argv.find("-n")->second : std::string();
		file_system.delete_user(_str_n);
		};
	commands["dgroup"] = [this](int argc, std::unordered_map<std::string, std::string> argv) -> void {
		if (argc % 2 == 1 && argc > 8 && argc == 0) {
			std::cout << "bad args" << std::endl;
			return;
		}
		if (show_help(argv)) {
			std::cout << "удалить группу" << std::endl;
			std::cout << std::format("{:5}", "-n") << std::format("{:5}", "-") << std::format("{:5}", "имя группы") << std::endl;
			std::cout << std::endl;
			return;
		}
		std::string _str_n = argv.find("-n") != argv.end() ? argv.find("-n")->second : std::string();
		file_system.delete_group(_str_n);
		};
	commands["luser"] = [this](int argc, std::unordered_map<std::string, std::string> argv) -> void {
		if (argc % 2 == 1 && argc > 8 && argc == 0) {
			std::cout << "bad args" << std::endl;
			return;
		}
		if (show_help(argv)) {
			std::cout << "показать список юзеров" << std::endl;
			std::cout << std::endl;
			return;
		}
		file_system.print_users();
		};
	commands["lgroup"] = [this](int argc, std::unordered_map<std::string, std::string> argv) -> void {
		if (argc % 2 == 1 && argc > 8 && argc == 0) {
			std::cout << "bad args" << std::endl;
			return;
		}
		if (show_help(argv)) {
			std::cout << "показать список групп" << std::endl;
			std::cout << std::endl;
			return;
		}
		file_system.print_groups();
		};
	commands["uuser"] = [this](int argc, std::unordered_map<std::string, std::string> argv) -> void {
		if (argc % 2 == 1 && argc > 8 && argc == 0) {
			std::cout << "bad args" << std::endl;
			return;
		}
		if (show_help(argv)) {
			std::cout << "обновить юзера" << std::endl;
			std::cout << std::format("{:5}", "-n") << std::format("{:5}", "-") << std::format("{:5}", "имя юзера") << std::endl;
			std::cout << std::format("{:5}", "-p") << std::format("{:5}", "-") << std::format("{:5}", "пароль") << std::endl;
			std::cout << std::format("{:5}", "-o") << std::format("{:5}", "-") << std::format("{:5}", "новый пароль") << std::endl;
			std::cout << std::endl;
			return;
		}
		std::string _str_n = argv.find("-n") != argv.end() ? argv.find("-n")->second : std::string();
		std::string _str_p = argv.find("-p") != argv.end() ? argv.find("-p")->second : std::string();
		std::string _str_o = argv.find("-o") != argv.end() ? argv.find("-o")->second : std::string();

		my_fs::user _user = file_system.get_user(_str_n);
		
		if (_user.try_login(_str_n, _str_p).get_is_loggined()) {
			_user.set_user(_str_n, _str_o);
			file_system.update_user(_user);
		}
		
		};
	commands["ugroup"] = [this](int argc, std::unordered_map<std::string, std::string> argv) -> void {
		if (argc % 2 == 1 && argc > 8 && argc == 0) {
			std::cout << "bad args" << std::endl;
			return;
		}
		if (show_help(argv)) {
			std::cout << "обновить группу" << std::endl;
			std::cout << std::format("{:5}", "-n") << std::format("{:5}", "-") << std::format("{:5}", "имя группы") << std::endl;
			std::cout << std::format("{:5}", "-n") << std::format("{:5}", "-") << std::format("{:5}", "удалить юзера(ов)") << std::endl;
			std::cout << std::format("{:5}", "-n") << std::format("{:5}", "-") << std::format("{:5}", "добавить юзера(ов)") << std::endl;
			std::cout << std::endl;
			return;
		}
		std::string _str_n = argv.find("-n") != argv.end() ? argv.find("-n")->second : std::string();
		std::string _str_d = argv.find("-d") != argv.end() ? argv.find("-d")->second : std::string();
		std::string _str_a = argv.find("-a") != argv.end() ? argv.find("-a")->second : std::string();

		std::stringstream ss_d(_str_d);
		std::stringstream ss_a(_str_a);

		std::string username;

		std::vector<my_fs::user> _users_to_add = std::vector<my_fs::user>();
		std::vector<my_fs::user> _users_to_del = std::vector<my_fs::user>();
		while (std::getline(ss_d, username, '|')) {
			_users_to_del.push_back(file_system.get_user(username));
		}
		while (std::getline(ss_a, username, '|')) {
			_users_to_add.push_back(file_system.get_user(username));
		}

		my_fs::group _group = file_system.get_group(_str_n);
		for (auto _user : _users_to_del) {
			_group.delete_user(_user.get_id());
		}
		for (auto _user : _users_to_add) {
			_group.add_user(_user.get_id());
		}
		file_system.update_group(_group);

		};
	commands["swus"] = [this](int argc, std::unordered_map<std::string, std::string> argv) -> void {
		if (argc % 2 == 1 && argc > 8 && argc == 0) {
			std::cout << "bad args" << std::endl;
			return;
		}
		if (show_help(argv)) {
			std::cout << "Сменить пользователя" << std::endl;
			std::cout << std::format("{:5}", "-n") << std::format("{:5}", "-") << std::format("{:5}", "имя юзера") << std::endl;
			std::cout << std::endl;
			return;
		}
		std::string _str_n = argv.find("-n") != argv.end() ? argv.find("-n")->second : std::string();

		my_fs::user _user = file_system.get_user(_str_n);

		if (!file_system.try_login(_user)) {
			std::cout << "Ошибка" << std::endl;
		}
		};
	commands["create"] = [this](int argc, std::unordered_map<std::string, std::string> argv) -> void { // создать новый объект create -n name -f (file) -d(dir) -r 10 -p path
		if (argc % 2 == 1 && argc > 8 && argc == 0) {
			std::cout << "bad args" << std::endl;
			return;
		}
		try {
			if (show_help(argv)) {
				std::cout << "создать файл" << std::endl;
				std::cout << std::format("{:5}", "-n") << std::format("{:5}", "-") << std::format("{:5}", "имя нового файла") << std::endl;
				std::cout << std::format("{:5}", "-f") << std::format("{:5}", "-") << std::format("{:5}", "файл(директория по дефолту)") << std::endl;
				std::cout << std::format("{:5}", "-p") << std::format("{:5}", "-") << std::format("{:5}", "путь") << std::endl;
				std::cout << std::endl;
				return;
			}

			std::string _str_p = argv.find("-p") != argv.end() ? argv.find("-p")->second : std::string();
			std::unique_ptr<my_fs::path> _path = get_path_w_root(_str_p);

			std::string _name = argv.find("-n")->second;
			object_type o_type = object_type(argv.find("-f") != argv.end());

			if (_name.empty()) {
				std::cout << "bad name or path" << std::endl;
				return;
			}

			file_system.create_obj(_name, *_path, o_type);
		}
		catch (const std::bad_array_new_length& gfg) {
			std::cout << gfg.what() << std::endl;
		}
		
		};

	commands["copy"] = [this](int argc, std::unordered_map<std::string, std::string> argv) -> void { // копировать объект copy -f file_to_copy -p path
		if (argc % 2 == 1 && argc > 2 && argc == 0) {
			std::cout << std::endl;
			std::cout << "bad args" << std::endl;
			return;
		}

		if (show_help(argv)) {
			std::cout << "копировать файл в директорию" << std::endl;
			std::cout << std::format("{:5}", "-f") << std::format("{:5}", "-") << std::format("{:5}", "файл для копирования") << std::endl;
			std::cout << std::format("{:5}", "-p") << std::format("{:5}", "-") << std::format("{:5}", "путь для копирования") << std::endl;
			std::cout << std::endl;
			return;
		}
		std::string _str_p = argv.find("-f") != argv.end() ? argv.find("-f")->second : std::string();
		std::unique_ptr<my_fs::path> _file_to_copy = get_path(_str_p);

		_str_p = argv.find("-p") != argv.end() ? argv.find("-p")->second : std::string();
		std::unique_ptr<my_fs::path> _path = get_path_w_root(_str_p);

		file_system.copy_obj(*_path, *_file_to_copy);

		};

	commands["delete"] = [this](int argc, std::unordered_map<std::string, std::string> argv) -> void { // удалить объект delete -p path
		if (argc % 2 == 1 && argc > 2 && argc == 0) {
			std::cout << "bad args" << std::endl;
			return;
		}

		if (show_help(argv)) {
			std::cout << "удалить файл" << std::endl;
			std::cout << std::format("{:5}", "-f") << std::format("{:5}", "-") << std::format("{:5}", "файл для удаления") << std::endl;
			std::cout << std::endl;
			return;
		}

		std::string _str_p = argv.find("-f") != argv.end() ? argv.find("-f")->second : std::string();
		std::unique_ptr<my_fs::path> _path = get_path(_str_p);
		
		if (_path == nullptr) {
			std::cout << "bad path" << std::endl;
			return;
		}

		file_system.delete_obj (*_path);
		};

	commands["rename"] = [this](int argc, std::unordered_map<std::string, std::string> argv) -> void { // переименовать файл rename -f file -n name
		if (argc % 2 == 1 && argc > 2 && argc == 0) {
			std::cout << "bad args" << std::endl;
			return;
		}

		if (show_help(argv)) {
			std::cout << "переименовать файл" << std::endl;
			std::cout << std::format("{:5}", "-f") << std::format("{:5}", "-") << std::format("{:5}", "файл для переименования") << std::endl;
			std::cout << std::format("{:5}", "-n") << std::format("{:5}", "-") << std::format("{:5}", "новое имя") << std::endl;
			std::cout << std::endl;
			return;
		}

		std::string _name = argv.find("-n") != argv.end() ? argv.find("-n")->second : std::string();
		std::string _str_p = argv.find("-f") != argv.end() ? argv.find("-f")->second : std::string();
		std::unique_ptr<my_fs::path> _path = get_path(_str_p);

		if (_path == nullptr) {
			std::cout << "bad path" << std::endl;
			return;	
			//logger<std::string, std::string, std::string>::log("null", "args:", "path null");
		}
		
		file_system.rename_obj(_name, *_path);

		};

	commands["chmod"] = [this](int argc, std::unordered_map<std::string, std::string> argv) -> void { 
		if (argc % 2 == 1 && argc > 4 && argc == 0) {
			std::cout << "bad args" << std::endl;
			return;
		}
		
		if (show_help(argv)) {
			std::cout << "задать атрибуты" << std::endl;
			std::cout << std::format("{:5}", "-m") << std::format("{:5}", "-") << std::format("{:5}", "права на файл") << std::endl;
			std::cout << std::format("{:5}", "-u") << std::format("{:5}", "-") << std::format("{:5}", "доступ для юзера") << std::endl;
			std::cout << std::format("{:5}", "-g") << std::format("{:5}", "-") << std::format("{:5}", "доступ для группы") << std::endl;
			std::cout << std::format("{:5}", "-f") << std::format("{:5}", "-") << std::format("{:5}", "файл") << std::endl;
			std::cout << std::endl;
			return;
		}
		int _perms = argv.find("-m") != argv.end() ? stoi(argv.find("-m")->second) : -1;
		std::string _str_p = argv.find("-f") != argv.end() ? argv.find("-f")->second : std::string();
		std::unique_ptr<my_fs::path> _path = get_path(_str_p);

		if ((_perms == -1 || _path == nullptr) ||
			(_perms == -1 && _path == nullptr)) {
			std::cout << "bad path or perms" << std::endl;
			return;
		}
		file_system.change_permissions(_perms,*_path);

		};

	commands["ls"] = [this](int argc, std::unordered_map<std::string, std::string> argv) -> void { // вывести объекты директории ls -p path
		if (argc % 2 == 1 && argc > 2 && argc == 0) {
			std::cout << "bad args" << std::endl;
			return;
		}

		if (show_help(argv)) {
			std::cout << "показать вхождения директории" << std::endl;
			std::cout << std::format("{:5}", "-p") << std::format("{:5}", "-") << std::format("{:5}", "директория") << std::endl;
			std::cout << std::endl;
			return;
		}
		std::string _str_p = argv.find("-p") != argv.end() ? argv.find("-p")->second : std::string();
		std::unique_ptr<my_fs::path> _path = get_path_w_root(_str_p);

		auto objs = file_system.get_dir(*_path);

		if (objs.empty()) {
			std::cout << "empty" << std::endl;
		}

		file_system.print_metadata_table();
		for (auto obj : objs) {
			file_system.print_file_metadata(obj);
		}
		};
	
	commands["open"] = [this](int argc, std::unordered_map<std::string, std::string> argv) -> void { // открыть объект cd -p file 
		if (argc % 2 == 1 && argc > 2 && argc == 0) {
			std::cout << "bad args" << std::endl;
			return;
		}

		if (show_help(argv)) {
			std::cout << "открыть файл" << std::endl;
			std::cout << std::format("{:5}", "-p") << std::format("{:5}", "-") << std::format("{:5}", "путь к файлу") << std::endl;
			std::cout << std::endl;
			return;
		}
		my_fs::file_object _f_obj = my_fs::file_object();
		std::string _str_p = argv.find("-p") != argv.end() ? argv.find("-p")->second : std::string();

		std::unique_ptr<my_fs::path> _path = get_path(_str_p);

		if (_path == nullptr) {
			std::cout << "bad path" << std::endl;
			return;
		}

		_f_obj = file_system.open_obj(*_path);

		if ((_f_obj).get_current_inode().get_object_type() == object_type::FILE) {
			file_system.print_obj(_f_obj);
		}

		};
	
	commands["write"] = [this](int argc, std::unordered_map<std::string, std::string> argv) -> void { // открыть объект cd -p file 
		if (argc % 2 == 1 && argc > 4 && argc == 0) {
			std::cout << "bad args" << std::endl;
			return;
		}

		if (show_help(argv)) {
			std::cout << "записать данные в файл" << std::endl;
			std::cout << std::format("{:5}", "-p") << std::format("{:5}", "-") << std::format("{:5}", "путь к файлу") << std::endl;
			std::cout << std::format("{:5}", "-a") << std::format("{:5}", "-") << std::format("{:5}", "дописать в файл") << std::endl;
			std::cout << std::endl;
			return;
		}

		my_fs::file_object _f_obj = my_fs::file_object();
		std::string _str_p = argv.find("-p") != argv.end() ? argv.find("-p")->second : std::string();
		write_type _w_type = argv.find("-a") != argv.end() ? write_type::APPEND : write_type::TRUNC;

		std::unique_ptr<my_fs::path> _path = get_path(_str_p);

		if (_path == nullptr) {
			std::cout << "bad path" << std::endl;
			return;
		}

		_f_obj = file_system.open_obj(*_path);

		if ((_f_obj).get_current_inode().get_object_type() == object_type::DIR) {
			std::cout << "file is dir, can't write" << std::endl;
			return;
		}

		std::cout << "input data:";
		std::string input;
		std::getline(std::cin, input);

		auto _f_obj_path = _f_obj.get_path();
		file_system.write_obj(input, _f_obj_path, _w_type);

		};

	commands["cls"] = [](int, std::unordered_map<std::string, std::string>) -> void {
		system("cls");
		};
}
std::string my_system::get_user() {
	return file_system.get_current_user();
}
void my_system::login() {
	my_fs::user current_user = file_system.get_user(file_system.get_current_user());
	if (!current_user.get_is_loggined()) {
		std::string password;
		std::cout << "Введите пароль:";
		std::cin >> password;
	}
	current_user.try_login(current_user.get_username(), current_user.get_password());
	file_system.try_login(current_user);
}
bool my_system::is_loggined() {
	return file_system.is_loggined();
}
std::string my_system::cur_path() {
	std::string str = file_system.get_current_path().get_virtual_path();
	return str + "$:";
}

std::unique_ptr<my_fs::path> my_system::get_path(std::string& str_path) {
	std::unique_ptr<my_fs::path> _path = nullptr;
	if (!str_path.empty()) {
		_path = str_path.substr(0, 2) == ".\\" ?
			std::unique_ptr<my_fs::path>(new my_fs::path(file_system.get_current_path() + str_path.substr(2, str_path.size()))) :
			std::unique_ptr<my_fs::path>(new my_fs::path(str_path));
	}
	return _path;
}

std::unique_ptr<my_fs::path> my_system::get_path_w_root(std::string& str_path) {
	std::unique_ptr<my_fs::path> _path = get_path(str_path);
	if (_path == nullptr) {
		_path = std::unique_ptr<my_fs::path>(new my_fs::path(file_system.get_current_path()));
	}
	return _path;
}

inline bool my_system::show_help(std::unordered_map<std::string, std::string> argv) {
	return argv.find("-help") != argv.end() || argv.find("-h") != argv.end();
}

template<typename Derived, typename Base, typename Del>
std::unique_ptr<Derived, Del>
static_unique_ptr_cast(std::unique_ptr<Base, Del>&& p)
{
	auto d = static_cast<Derived*>(p.release());
	return std::unique_ptr<Derived, Del>(d, std::move(p.get_deleter()));
}