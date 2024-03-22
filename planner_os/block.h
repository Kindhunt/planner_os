#pragma once
#include "serializable.h"

namespace my_fs {	
	class block : public serializable {
	private:
		std::string data;
	public:
		char* get_data();
		std::string& get_data_vector();

		block& write_data(std::string& __data, write_type __write_type);
		bool is_full();

		void serialize(std::ostream& os) const override;
		void deserialize(std::istream& is) override;
	};
}

template<>
struct std::hash<my_fs::block> {
	std::size_t operator()(const my_fs::block& s) const noexcept {
		return std::hash<unsigned int>{}(s.get_id());
	}
};


