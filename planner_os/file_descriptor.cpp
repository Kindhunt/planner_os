#include "file_descriptor.h"

namespace my_fs {
    file_descriptor& file_descriptor::set_id(unsigned int id) {
        serializable::set_id(id);
        return *this;
    }

    file_descriptor& file_descriptor::set_name(std::string name) {
        this->name = name;
        return *this;
    }

    file_descriptor& file_descriptor::set_start_offset(unsigned int start_offset) {
        this->start_offset = start_offset;
        return *this;
    }

    file_descriptor& file_descriptor::set_end_offset(unsigned int end_offset) {
        this->end_offset = end_offset;
        return *this;
    }

    std::string& file_descriptor::get_name_link() {
        return name;
    }

    std::string file_descriptor::get_name() const {
        return name;
    }

    unsigned int file_descriptor::get_start_offset() {
        return start_offset;
    }

    unsigned int file_descriptor::get_end_offset() {
        return end_offset;
    }
    void file_descriptor::serialize(std::ostream& os) const {
        os.write(reinterpret_cast<const char*>(&id), sizeof(id));
        const auto len = name.size();
        os.write(reinterpret_cast<const char*>(&len), sizeof(len));
        if (len > 0) {
            os.write(name.data(), len);
        }
        os.write(reinterpret_cast<const char*>(&start_offset), sizeof(start_offset));
        os.write(reinterpret_cast<const char*>(&end_offset), sizeof(end_offset));
    }
    void file_descriptor::deserialize(std::istream& is) {
        is.read(reinterpret_cast<char*>(&id), sizeof(id));
        size_t len = 0;
        is.read(reinterpret_cast<char*>(&len), sizeof(len));
        if (len > 0) {
            name.resize(len);
            is.read(name.data(), len);
        }
        is.read(reinterpret_cast<char*>(&start_offset), sizeof(start_offset));
        is.read(reinterpret_cast<char*>(&end_offset), sizeof(end_offset));
    }
}
