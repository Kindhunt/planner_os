#include "process_manager.h"

namespace my_fs {
	process_manager::process_manager() {
		processes = std::vector<process>();
	}

	void process_manager::add_process(process proc) {
		processes.push_back(proc);
	}

	void process_manager::remove_process(int pid) {
		auto pos = std::find_if(processes.begin(), processes.end(), [&](process proc) {
			return proc.is_it_process(pid);
			});
		if (pos != std::end(processes)) {
			processes.erase(pos);
		}
	}

	bool process::is_it_process(int id) {
		return pid == id;
	}
}
