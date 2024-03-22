#include "block.h"

namespace my_fs {

	/// BLOCK
	char* block::get_data() {
		return data.data();
	}

	std::string& block::get_data_vector() {
		return data;
	}

	block& block::write_data(std::string& __data, write_type __write_type) {
		if (__write_type == write_type::TRUNC) {
			data.clear();
		}
		data = __data;
		return *this;
	}

	bool block::is_full() {
		return data.size() == BLOCK_SIZE;
	}

	void block::serialize(std::ostream& os) const {
		os.write(reinterpret_cast<const char*>(&id), sizeof(id));
		size_t data_sz = data.size();
		os.write(reinterpret_cast<const char*>(&data_sz), sizeof(data_sz));
		if (data_sz > 0) {
			os.write(data.data(), data_sz);
		}
	}

	void block::deserialize(std::istream& is) {
		is.read(reinterpret_cast<char*>(&id), sizeof(id));
		size_t data_sz = 0;
		is.read(reinterpret_cast<char*>(&data_sz), sizeof(data_sz));
		if (data_sz > 0) {
			data.resize(data_sz);
			is.read(data.data(), data_sz);
		}
	}
}
