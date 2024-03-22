#include "my_system.h"

std::unordered_map<std::string, std::string> parse_command(std::vector<std::string>* tokens) {
	std::unordered_map<std::string, std::string> argv = std::unordered_map<std::string, std::string>();

	
	bool is_single = tokens->size() == 1;

	for (int i = 0; i < tokens->size(); ++i) {
		bool is_many = tokens->size() - i > 1; // 3 - 2 = 1, 4 - 2 = 2

		auto a = is_many && (tokens->at(i).find("-") != std::string::npos) && tokens->at(i).find("-") == 0;
		auto b = is_many && (tokens->at(i + !is_single).find("-") != std::string::npos) && tokens->at(i + !is_single).find("-") == 0;
		auto c = !is_many && tokens->size() - i == 1;
		
		is_single = c || b && a;

		std::string flag = tokens->at(i);
		std::string val = tokens->at(i + !is_single);
		
		i += !is_single;

		argv[flag] = val;
	}

	return argv;
}

void main(int argc, char* argv[]) {
	setlocale(LC_ALL, "ru");
	my_system m_sys = my_system(true);
	bool init = true;

	while (true) {
		if (init) {
			std::cin.clear();
			std::cin.ignore();
			init = false;
		}
		if (!m_sys.is_loggined()){
			m_sys.login();
			std::cin.clear();
			std::cin.ignore();
		}
		std::string stroke;
		std::cout << m_sys.get_user() << '@' << m_sys.cur_path();

		std::getline(std::cin, stroke);

		std::istringstream iss(stroke); // создаем строковый поток из входной строки
		std::vector<std::string> tokens; // создаем вектор для хранения разделенных слов

		std::string token;
		auto is_first = true;
		std::string command;

		while (iss >> token) { // пока операция извлечения пробелов успешна
			if (is_first) {
				command = token;
				is_first = false;
			}
			else {
				tokens.push_back(token); // добавляем слово в вектор
			}
		}

		std::unordered_map<std::string, std::string> command_flags = parse_command(&tokens);
		m_sys.commands[command](tokens.size(), command_flags);
	}

}

