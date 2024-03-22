#include "S5fs.h"

namespace my_fs {
    S5fs::S5fs(bool init) : S5fs() {
        if (init) {
            std::ofstream ofs;

            ofs.open(_sb_path, std::ofstream::out | std::ofstream::trunc);
            ofs.close();

            ofs.open(_data_path, std::ofstream::out | std::ofstream::trunc);
            ofs.close();

            ofs.open(_inode_path, std::ofstream::out | std::ofstream::trunc);
            ofs.close();

            ofs.open(_fd_path, std::ofstream::out | std::ofstream::trunc);
            ofs.close();

            ofs.open(_users_path, std::ofstream::out | std::ofstream::trunc);
            ofs.close();

            ofs.open(_groups_path, std::ofstream::out | std::ofstream::trunc);
            ofs.close();
        }
    }

    S5fs::S5fs(bool init, int size) : S5fs(init) {
        if (init) {
            user root_user = user();
            group root_group = group();

            std::string password;
            std::cout << "initing system" << std::endl << "enter new root password:";
            std::cin >> password;

            root_user.set_user("root", password);
            root_group.set_name("root");
            root_group.add_user(root_user.get_id());

            po_manager.create_user(root_user);
            po_manager.create_group(root_group);

            auto root = f_manager.init_system(root_user, root_group);

            current_user = po_manager.get_user(root_user.get_username());
            current_group = po_manager.get_group(root_group.get_name());

            current_opened_file = root;

            users.push_back("root");
            groups.push_back("root");
        }
    }

    void S5fs::create_obj(std::string name, path& abs_path, object_type type) {
        if (!current_user.is_file_access(name) && !current_user.get_is_super()) {
            std::cout << "don't have access" << std::endl;
            return;
        }

        name = check_copy(abs_path, name);

        f_manager.create_obj(name, abs_path, type, current_user.get_id(), current_group.get_id());
    }

    file_object S5fs::open_obj(path abs_path) {
        file_object f_obj = f_manager.get_file_metadata(abs_path);
        if (f_obj.get_current_inode().get_object_type() == object_type::DIR) {
            current_opened_file = f_obj;
        }
        return f_obj;
    }

    void S5fs::write_obj(std::string& data, path abs_path, write_type w_type) {
        if (!current_user.is_file_access(current_user.get_username()) && !current_user.get_is_super()) {
            std::cout << "don't have access" << std::endl;
            return;
        }
        open_obj(abs_path);
        f_manager.write_file(abs_path, data, w_type);
    }

    void S5fs::print_obj(file_object& file) {
        if (!current_user.is_file_access(current_user.get_username()) && !current_user.get_is_super()) {
            std::cout << "don't have access" << std::endl;
            return;
        }
        int eof = file.get_current_inode().get_data_blocks().empty() ? 
            file.get_f_description().get_end_offset() : 
            file.get_current_inode().get_data_blocks().front();

        std::string output;
        for (int i = file.get_f_description().get_start_offset(); i < eof; ++i) {
            auto str = f_manager.get_data_from_block(i);
            output += str;
        }
        std::cout << output << std::endl;
    }

    void S5fs::delete_obj(path& abs_path) {
        if (!current_user.is_file_access(current_user.get_username()) && !current_user.get_is_super()) {
            std::cout << "don't have access" << std::endl;
            return;
        }
        f_manager.delete_obj(abs_path);
    }

    void S5fs::copy_obj(path& path_for_copy, path& abs_path) {
        if (!current_user.is_file_access(current_user.get_username()) && !current_user.get_is_super()) {
            std::cout << "don't have access" << std::endl;
            return;
        }
        
        auto entries = get_dir(path_for_copy);
        auto pos = std::find_if(entries.begin(), entries.end(), [&](file_object f_obj) {
            return f_obj.get_path() == abs_path;
            });
        std::string name = abs_path.get_name_of_current_virtual_obj();
        if (pos != std::end(entries)) {
            name = check_copy(path_for_copy, abs_path.get_name_of_current_virtual_obj());
        }
        
        f_manager.copy_obj(path_for_copy, abs_path, name);
    }

    void S5fs::rename_obj(std::string new_name, path abs_path) {
        if (!current_user.is_file_access(current_user.get_username()) && !current_user.get_is_super()) {
            std::cout << "don't have access" << std::endl;
            return;
        }
        file_object f_obj = f_manager.get_file_metadata(abs_path);
        
        /*std::for_each(MY_BAD_CHARS.begin(), MY_BAD_CHARS.end(), [&](auto ch) {
            assert(new_name.find(ch));
            });*/
        auto _path = f_obj.get_path();
        --_path;
        f_obj.get_f_description().set_name((_path + new_name).get_virtual_path());
        f_manager.update_file(f_obj, abs_path);
    }

    void S5fs::mov_obj(path& path_to_mov, path& abs_path) {
        if (!current_user.is_file_access(current_user.get_username()) && !current_user.get_is_super()) {
            std::cout << "don't have access" << std::endl;
            return;
        }
        f_manager.copy_obj(path_to_mov, abs_path, abs_path.get_name_of_current_virtual_obj());
        f_manager.delete_obj(abs_path);
    }

    void S5fs::change_permissions(int new_permissions, path& abs_path) {
        if (!current_user.is_file_access(current_user.get_username()) && !current_user.get_is_super()) {
            std::cout << "don't have access" << std::endl;
            return;
        }
        file_object f_obj = f_manager.get_file_metadata(abs_path);
        f_obj.get_current_inode().set_permissions(new_permissions);
        f_manager.update_file(f_obj, abs_path);
    }

    void S5fs::print_file_metadata(file_object& file) {
        std::string s_file = to_string(file);
        std::stringstream ss(s_file);
        std::string temp;
        std::vector<std::string> strs = std::vector<std::string>();
        while (std::getline(ss, temp, ' ')) {
            strs.push_back(temp);
        }
        auto perm1 = strs[3][0] - '0';
        std::bitset<3> perm1_bit(perm1);
        std::string perm1_str = std::string(perm1_bit[0] ? "r" : "-") +
            std::string(perm1_bit[1] ? "w" : "-") +
            std::string(perm1_bit[2] ? "x" : "-");

        auto perm2 = strs[3][1] - '0';
        std::bitset<3> perm2_bit(perm2);
        std::string perm2_str = std::string(perm2_bit[0] ? "r" : "-") +
            std::string(perm2_bit[1] ? "w" : "-") +
            std::string(perm2_bit[2] ? "x" : "-");

        auto perm3 = strs[3][2] - '0';
        std::bitset<3> perm3_bit(perm3);
        std::string perm3_str = std::string(perm3_bit[0] ? "r" : "-") +
            std::string(perm3_bit[1] ? "w" : "-") +
            std::string(perm3_bit[2] ? "x" : "-");

        strs[3] = perm1_str + perm2_str + perm3_str;
        strs[5] = get_user(strs[5]).get_username();
        strs[6] = get_group(strs[6]).get_name();

        std::cout << std::format("{:5}{:10}{:5}{:20}{:5}{:10}{:10}{:20}{:20}{:20}\n", 
            strs[0], strs[1], strs[2], strs[3], strs[4], strs[5], strs[6], strs[7], strs[8], strs[9]);
    }

    std::string S5fs::check_copy(path abs_path, std::string name) {
        auto entries = get_dir(abs_path);
        auto fullpath = abs_path + name;
        for (auto entry : entries) {
            if (entry.get_path() == fullpath) {
                std::string to_insert = "-cpy";

                if (name.find("-cpy") != std::string::npos) {
                    to_insert = std::to_string(std::stoi(std::string(name.begin() + name.find("-cpy"), name.end())) + 1);
                }

                name += to_insert;
            }
        }
        return name;
    }

    bool S5fs::try_exit() {
        if (current_user.get_is_loggined()) {
            current_user.unlogin();
            po_manager.update_user(current_user);            
        }
        return !current_user.get_is_loggined();
    }

    bool S5fs::try_login(user user_check) {
        if (std::find_if(users.begin(), users.end(), [&](std::string username) {
            auto u = po_manager.get_user(username);
            return user_check.try_login(u.get_username(), u.get_password()).get_is_loggined();
            }) != std::end(users))
        {
            current_user = user_check;
            po_manager.update_user(user_check);

            return true;
        }
        return false;
    }

    bool S5fs::is_loggined(std::string username) {
        return std::find_if(users.begin(), users.end(), [&](std::string username) {
            auto u = po_manager.get_user(username);
            return u.get_is_loggined();
            }) != std::end(users);
    }

    bool S5fs::is_loggined() {
        return current_user.get_is_loggined();
    }

    void S5fs::create_user(user user_create) {
        users.push_back(user_create.get_username());
        po_manager.create_user(user_create);
    }

    void S5fs::create_group(group group_create) {
        groups.push_back(group_create.get_name());
        po_manager.create_group(group_create);
    }

    void S5fs::update_user(user user_upd) {
        po_manager.update_user(user_upd);
        if (user_upd.get_id() == current_user.get_id()) {
            current_user = user_upd;
        }
    }

    void S5fs::update_group(group group_upd) {
        po_manager.update_group(group_upd);
    }

    void S5fs::set_current_user(user current_user) {
        if (po_manager.get_user(current_user.get_username())
            .try_login(current_user.get_username(), 
                current_user.get_password()).get_is_loggined()) {
            this->current_user = current_user;
        }
    }

    void S5fs::set_current_group(std::string name) {
        current_group = po_manager.get_group(name);
    }

    void S5fs::delete_user(std::string name) {
        auto it = std::find(users.begin(), users.end(), name);
        if (it != std::end(users)) {
            users.erase(it);
        }
        auto _user = po_manager.get_user(name);
        po_manager.delete_user(_user.get_id());
    }

    void S5fs::delete_group(std::string name) {
        auto it = std::find(groups.begin(), groups.end(), name);
        if (it != std::end(groups)) {
            groups.erase(it);
        }
        auto _group = po_manager.get_group(name);
        po_manager.delete_group(_group.get_id());
    }

    void S5fs::print_users() {
        std::vector<std::string> user_table = {
            "ID", "N", "L", "S", "F"
        }; 
        std::cout << std::format("{:5}{:10}{:5}{:5}{:10}\n",
            user_table[0], user_table[1], user_table[2], user_table[3], user_table[4]);
        for(auto username : users) {
            user u = po_manager.get_user(username);
            std::string id = std::to_string(u.get_id());
            std::string is_super = u.get_is_super() ? "s" : "-";
            std::string is_loggined = u.get_is_loggined() ? "l" : "-";
            std::string files = [&]() -> std::string {
                std::string to_ret;
                if (u.get_files().empty()) {
                    return "-";
                }
                for (auto i : u.get_files()) {
                    to_ret.append(i).append(" ");
                };
                to_ret.erase(to_ret.end() - 1);
                return to_ret;
                }();
            std::cout << std::format("{:5}{:10}{:5}{:5}{:10}\n", id, username, is_super, is_loggined, files);
        };
    }

    void S5fs::print_groups() {
        std::vector<std::string> group_table = {
            "ID", "N", "U"
        };
        std::cout << std::format("{:5}{:10}{:10}\n", group_table[0], group_table[1], group_table[2]);
        for(auto name : groups) {
            group g = po_manager.get_group(name);
            std::stringstream ss;
            std::string id = std::to_string(g.get_id());
            std::string userz = [&]() -> std::string {
                std::string to_ret;
                if (g.get_users_ids().empty()) {
                    return "-";
                }
                for (auto i : g.get_users_ids()) {
                    to_ret.append(po_manager.get_user(i).get_username()).append(" ");
                };
                to_ret.erase(to_ret.end() - 1);
                return to_ret;
                }();
            std::cout << std::format("{:5}{:10}{:10}\n", id, name, userz);
        };
    }

    user S5fs::get_user(std::string username) {
        return po_manager.get_user(username);
    }

    group S5fs::get_group(std::string name) {
        return po_manager.get_group(name);
    }

    path S5fs::get_current_path() {
        return current_opened_file.get_path();
    }

    std::vector<file_object> S5fs::get_dir(path abs_path) {
        return f_manager.get_entries(abs_path);
    }

    std::string S5fs::get_file_metadata(path abs_path) {
        auto f_obj = f_manager.get_file_metadata(abs_path);
        return to_string(f_obj);
    }

    std::string S5fs::get_current_user() {
        return current_user.get_username();
    }

    void S5fs::print_metadata_table() {
        std::vector<std::string> metadata_table = {
            "ID", "N", "T", "P", "S", "U", "G", "TC", "TA", "TM"
        };
        std::cout << std::format("{:5}{:10}{:5}{:20}{:5}{:10}{:10}{:20}{:20}{:20}\n",
            metadata_table[0], metadata_table[1], metadata_table[2], metadata_table[3], metadata_table[4], 
            metadata_table[5], metadata_table[6], metadata_table[7], metadata_table[8], metadata_table[9]);
    }

    S5fs::S5fs() {
        f_manager = file_manager();
        current_group = group();
        current_user = user();
    }
}

