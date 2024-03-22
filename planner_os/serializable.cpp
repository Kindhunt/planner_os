#include "serializable.h"

void my_fs::serializable::serialize(std::ostream& os) const {
	throw std::exception("undefined");
}

void my_fs::serializable::deserialize(std::istream& is) {
	throw std::exception("undefined");
}
