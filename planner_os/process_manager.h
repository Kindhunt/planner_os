#pragma once
#include <time.h>
#include <vector>

namespace my_fs {
	class process {
	private:
		int pid;
		int user;
		int pri;
		int ni;
		char state;
		float used_cpu;
		float used_mem;
		time_t work_time;
	public:
		bool is_it_process(int id);
	};

	class process_manager {
	private:
		std::vector<process> processes;
	public:
		process_manager();
		void add_process(process proc);
		void remove_process(int pid);
	};
}

