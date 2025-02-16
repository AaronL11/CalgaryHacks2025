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
        }
        return {"command \"" + cmd +
                "\" not recognized, type \"help\" for a list of commands"};
    }

    inline std::vector<std::string> help(const std::vector<std::string>& args) {
        return {"This is the help command"};
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
        if (args.empty()) {
            return {};
        }
        return {};
    }

    inline std::vector<std::string> decrypt(
        const std::vector<std::string>& args) {
        if (args.empty()) {
            return {};
        }
        return {};
    }

    inline void unlock_decrypt() { decrypt_unlocked = true; }

   private:
    std::shared_ptr<FileGuy> file_guy;
    bool decrypt_unlocked = false;
};
