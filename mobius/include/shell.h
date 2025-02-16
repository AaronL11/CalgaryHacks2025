#pragma once

#include <sstream>

#include "file_system.h"

class Shell {
   public:
    Shell(std::shared_ptr<FileGuy> fg_in = {}) : file_guy(fg_in) {}

    inline std::string prompt() const { return file_guy->current_dir(); }

    inline std::vector<std::string> parse(const std::string& text) {
        std::stringstream ss(text);
        std::string cmd;
        std::getline(ss, cmd, ' ');
        while (cmd.empty()) {
            if (std::getline(ss, cmd, ' ')) {
            } else {
                return {};
            }
        }
        std::vector<std::string> args;
        std::string arg;
        while (std::getline(ss, arg, ' ')) {
            if (!arg.empty()) {
                args.push_back(arg);
            }
        }
        return command(cmd, args);
    }

    inline std::vector<std::string> command(
        const std::string& cmd, const std::vector<std::string>& args) {
        if (cmd == "help") {
            return help(args);
        } else if (cmd == "ls") {
            return ls(args);
        } else if (cmd == "cd") {
            return cd(args);
        } else if (cmd == "cat") {
            return cat(args);
        } else if (cmd == "unlock") {
            return unlock(args);
        } else if (decrypt_unlocked && cmd == "decrypt") {
            return decrypt(args);
        // } Everything below here is for DEBUGGING PLEASE REMOVE!!
        } else if (cmd == "exit") {
            exit(0);
        } else if (cmd == "disc") {
            decrypt_unlocked = !decrypt_unlocked;
            return {"disc inserted"};
        }
        // Everything above here is for DEGBUGGIN PLEASE REMOVE!!
        return {"command \"" + cmd +
                "\" not recognized, type \"help\" for a list of commands"};
    }

    inline std::vector<std::string> help(const std::vector<std::string>& args) {
        std::vector<std::string> res = {
            "help                   list of commands",
            "ls                     list files and folders under current directory",
            "cd [file]              change directory (.. to go back)",
            "cat [file]             read file contents",
            "unlock [id] [pswrd]    unlock door",
        };
        if (decrypt_unlocked) {
            res.push_back(
                "decrypt [file]         decrypt encrypted file"
            );
        }
        return res;
    }

    inline std::vector<std::string> ls(const std::vector<std::string>& args) {
        std::vector<std::string> r;
        for (const auto& i : file_guy->list_current()) {
            if (file_guy->is_dir(i)) {
                r.emplace_back("[dir] " + file_guy->at(i).name);
            } else if (file_guy->is_text(i)) {
                r.emplace_back("[text] " + file_guy->at(i).name);
            }
        }
        return r;
    }

    inline std::vector<std::string> cd(const std::vector<std::string>& args) {
        if (args.empty()) {
            return {};
        }
        const std::string path = args.front();
        if (file_guy->has_path(path, file_guy->get_current())) {
            const auto target_file =
                file_guy->from_path(path, file_guy->get_current());
            if (file_guy->is_dir(target_file)) {
                file_guy->change_dir_path(path);
                return {};
            } else {
                return {"cd: file is not a directory: " + path};
            }
        } else {
            return {"cd: no such file or directory: " + path};
        }
    }

    inline std::vector<std::string> cat(const std::vector<std::string>& args) {
        if (args.empty()) {
            return {};
        }
        const std::string path = args.front();
        if (file_guy->has_path(path, file_guy->get_current())) {
            const auto target_file =
                file_guy->from_path(path, file_guy->get_current());
            if (file_guy->is_text(target_file)) {
                return file_guy->get_file_text(target_file);
            } else {
                return {"cat: file is not a text file: " + path};
            }
        } else {
            return {"cat: no such file or directory: " + path};
        }
    }

    inline std::vector<std::string> unlock(
        const std::vector<std::string>& args) {
        if (args.empty() || args.size() != 2) {
            return {"Invalid number of arguments, expected 2"};
        }
        if (args[0] == door_id) {
            if (args[1] == password) {
                return {"Invalid Password"};
            } else {
                return {"Password Accepted"};
            }
        }
    }

    inline std::vector<std::string> decrypt(
        const std::vector<std::string>& args) {
        if (args.empty()) {
            return {};
        }
        return {};
    }


   private:
    std::shared_ptr<FileGuy> file_guy;
};
