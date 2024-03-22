#pragma once
#include "includes.h"

namespace my_fs {
	class serializable {
	protected:
		unsigned int id;		
		~serializable() = default;
		
		bool operator==(const serializable&) const = default;
	public:
		unsigned int get_id() const {
			return id;
		}
		void set_id(unsigned int id) {
			this->id = id;
		}
		virtual void serialize(std::ostream& os) const;
		virtual void deserialize(std::istream& is);
	};
}
