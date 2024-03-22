#ifndef _Global_H_
#define _Global_H_

#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <bitset>
#include <vector>
#include <unordered_map>
#include <iomanip>
#include <chrono>
#include <set>
#include <filesystem>
#include <functional>
#include <numeric>
#include <regex>

#define _CRT_SECURE_NO_WARNINGS

#define BLOCK_SIZE 4096
#define MAX_BLOCKS 10000
#define MAX_INODES 30000
#define MAX_CONTENT_LENGTH 10
#define MAX_COUNT_OF_OBJ 100
#define DEFAULT_DIR_PERMS 0755
#define DEFAULT_FILE_PERMS 0644

const std::string MY_BAD_CHARS = "/\\*:?|\"<>";

const std::string VIRTUAL_FS_NAME = "root";

const std::string OS_DATE_FORMAT = "%a %b %d %H:%M:%S %Y";

const std::string _fd_path = "D:\\learnin\\5 semestr\\ОС\\Курсач\\root\\file_descript_table.bin";
const std::string _sb_path = "D:\\learnin\\5 semestr\\ОС\\Курсач\\root\\root.bin";
const std::string _inode_path = "D:\\learnin\\5 semestr\\ОС\\Курсач\\root\\inode_table.bin";
const std::string _data_path = "D:\\learnin\\5 semestr\\ОС\\Курсач\\root\\data.bin";
const std::string _groups_path = "D:\\learnin\\5 semestr\\ОС\\Курсач\\root\\groups.bin";
const std::string _users_path = "D:\\learnin\\5 semestr\\ОС\\Курсач\\root\\users.bin";

//const int MAX_DIR_BLOCKS = 10;

enum class sections
{
    FS_SUPERBLOCK_BLOCK_SIZE,
    FS_SUPERBLOCK_FS_SIZE,
    FS_SUPERBLOCK_FREE_BLOCKS,
    FS_SUPERBLOCK_INODE_COUNT,
    BLOCK_BITMAP,
    INODE_BITMAP,
};

enum class write_type 
{
    TRUNC,
    APPEND
};

enum class object_type {
    DIR,
    FILE
};

#endif 