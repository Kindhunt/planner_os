#pragma once
#include "serializable.h"
#include "block.h"
#include "i_node.h"
#define _CRT_SECURE_NO_WARNINGS

namespace my_fs
{
    class super_block : public serializable
    {
    private:
        int block_size;
        int filesystem_size;
        std::vector<char> inode_bitmap;
        std::vector<char> block_bitmap;
    public:
        super_block(bool init = false);
        super_block& set_block_size(int block_size);
        super_block& set_filesystem_size(int filesystem_size);
        super_block& set_inode_bitmap(std::vector<char> inode_bitmap);
        super_block& set_block_bitmap(std::vector<char> block_bitmap);

        int get_block_size();
        int get_filesystem_size();
        std::vector<char>& get_inode_bitmap();
        std::vector<char>& get_block_bitmap();

        void serialize(std::ostream& os) const override;
        void deserialize(std::istream& is) override;

        char& operator[](block& __block);

        char& operator[](i_node& __inode);
        
        super_block& operator+=(int filesystem_size);
        super_block& operator-=(int filesystem_size);
    };
}
