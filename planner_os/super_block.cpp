#include "super_block.h"

namespace my_fs {
    super_block::super_block(bool init) {
        if (init) {
            block_size = BLOCK_SIZE;
            filesystem_size = 0;
            inode_bitmap = std::vector<char>(MAX_INODES, '0');
            block_bitmap = std::vector<char>(MAX_BLOCKS, '0');
        }
    }
    super_block& super_block::set_block_size(int block_size) {
        this->block_size = block_size;
        return *this;
    }
    super_block& super_block::set_filesystem_size(int filesystem_size) {
        this->filesystem_size = filesystem_size;
        return *this;
    }
    super_block& super_block::set_inode_bitmap(std::vector<char> inode_bitmap) {
        this->inode_bitmap = inode_bitmap;
        return *this;
    }
    super_block& super_block::set_block_bitmap(std::vector<char> block_bitmap) {
        this->block_bitmap = block_bitmap;
        return *this;
    }

    int super_block::get_block_size() {
        return block_size;
    }
    int super_block::get_filesystem_size() {
        return filesystem_size;
    }
    std::vector<char>& super_block::get_inode_bitmap() {
        return inode_bitmap;
    }
    std::vector<char>& super_block::get_block_bitmap() {
        return block_bitmap;
    }

    void super_block::serialize(std::ostream& os) const {
        os.write(reinterpret_cast<const char*>(&block_size), sizeof(block_size));
        os.write(reinterpret_cast<const char*>(&filesystem_size), sizeof(filesystem_size));
        size_t blocks_allocated = block_bitmap.size();
        os.write(reinterpret_cast<const char*>(&blocks_allocated), sizeof(blocks_allocated));
        if (blocks_allocated > 0) {
            os.write(block_bitmap.data(), blocks_allocated);
        }
        size_t inode_count = inode_bitmap.size();
        os.write(reinterpret_cast<const char*>(&inode_count), sizeof(inode_count));
        if (inode_count > 0) {
            os.write(inode_bitmap.data(), inode_count);
        }
    }
    void super_block::deserialize(std::istream& is) {
        is.read(reinterpret_cast<char*>(&block_size), sizeof(block_size));
        is.read(reinterpret_cast<char*>(&filesystem_size), sizeof(filesystem_size));
        size_t blocks_allocated = 0;
        is.read(reinterpret_cast<char*>(&blocks_allocated), sizeof(blocks_allocated));
        if (blocks_allocated > 0) {
            block_bitmap.resize(blocks_allocated);
            is.read(block_bitmap.data(), blocks_allocated);
        }
        size_t inode_count = 0;
        is.read(reinterpret_cast<char*>(&inode_count), sizeof(inode_count));
        if (inode_count > 0) {
            inode_bitmap.resize(inode_count);
            is.read(inode_bitmap.data(), inode_count);
        }
    }

    char& super_block::operator[](block& __block) {
        return block_bitmap[__block.get_id()];
    }

    char& super_block::operator[](i_node& __inode) {
        return inode_bitmap[__inode.get_id()];
    }

    super_block& super_block::operator+=(int filesystem_size) {
        this->filesystem_size += filesystem_size;
        return *this;
    }
    super_block& super_block::operator-=(int filesystem_size) {
        this->filesystem_size -= filesystem_size;
        return *this;
    }
}