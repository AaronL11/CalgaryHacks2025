#pragma once
#include <vector>

enum class FileType {
    Directory,
    Text,
};

namespace file_content {
struct Directory {
    FileType type = FileType::Directory;
};
struct Text {
    FileType type = FileType::Text;
    int i;
};
}  // namespace file_content

union FileContent {
    FileType type;
    file_content::Directory directory;
    file_content::Text text;
};

struct FileUnit {
    std::string name = {};
    FileContent content = {};
    int parent = -1;
};

class FileGuy {
   public:
    FileGuy(const std::vector<FileUnit>& files) : file_list(files) {}

    inline std::vector<int> list_current() const {
        std::vector<int> ls;
        for (int i = 0; i < file_list.size(); ++i) {
            if (is_parent(current, i)) {
                ls.push_back(i);
            }
        }
        return ls;
    }

    inline const FileUnit& at(const int i) const { return file_list[i]; }
    inline void add_file(const FileUnit& f) { file_list.push_back(f); }

    inline const int from_path(const std::string& path, const int vc) const {
        if (has_path(path, vc)) {
            if (path == "~") {
                return -1;
            } else if (path == ".") {
                return vc;
            } else if (path == "..") {
                return file_list[vc].parent;
            } else if (path.substr(0, 2) == "~/") {
                return from_path(path.substr(path.find('/') + 1), -1);
            } else if (path.substr(0, 2) == "./") {
                return from_path(path.substr(path.find('/') + 1), vc);
            } else if (path.substr(0, 3) == "../") {
                return from_path(path.substr(path.find('/') + 1),
                                 file_list[vc].parent);
            } else if (path.find('/') != -1) {
                const std::string name = path.substr(0, path.find('/'));
                return from_path(path.substr(path.find('/') + 1),
                                 find_file(name, vc));
            } else {
                return find_file(path, vc);
            }
        }
        abort();
    }

    inline std::string current_dir() const { return dir(current); }

    inline std::string dir(const int i) const {
        if (i == -1) {
            return "~";
        }
        return dir(at(i).parent) + "/" + at(i).name;
    }

    inline void change_dir_name(const std::string& name) {
        const int i = find_file(name, current);
        if (i < 0) {
            abort();
        }
        current = i;
    }

    inline bool has_path(const std::string& path, const int vc) const {
        if (path.empty()) {
            return false;
        } else if (path == "~") {
            return true;
        } else if (path == ".") {
            return true;
        } else if (path == "..") {
            return vc != -1;
        } else if (path.substr(0, 2) == "~/") {
            return has_path(path.substr(path.find('/') + 1), -1);
        } else if (path.substr(0, 2) == "./") {
            return has_path(path.substr(path.find('/') + 1), vc);
        } else if (path.substr(0, 3) == "../") {
            if (vc < 0) {
                return false;
            }
            return has_path(path.substr(path.find('/') + 1),
                            file_list[vc].parent);
        } else if (path.find('/') != -1) {
            const std::string name = path.substr(0, path.find('/'));
            const int i = find_file(name, vc);
            if (i >= 0 && is_dir(i)) {
                return has_path(path.substr(path.find('/') + 1), i);
            }
            return false;
        } else {
            return find_file(path, vc) >= 0;
        }
    }

    inline void change_dir_path(const std::string& path) {
        if (has_path(path, current)) {
            const int i = from_path(path, current);
            if (is_dir(i)) {
                current = i;
            }
            return;
        }
        abort();
    }

    inline int find_file(const std::string& name, const int vc,
                         const bool global = false) const {
        for (int i = 0; i < file_list.size(); ++i) {
            if (file_list[i].name == name) {
                if (global) {
                    return i;
                } else if (is_parent(vc, i)) {
                    return i;
                }
            }
        }
        return -1;
    }

    inline bool is_dir(const int i) const {
        if (i == -1) {
            return true;
        }
        return file_list[i].content.type == FileType::Directory;
    }

    inline bool is_text(const int i) const {
        return file_list[i].content.type == FileType::Text;
    }

    inline const std::vector<std::string>& get_file_text(const int i) const {
        if (is_text(i)) {
            return text_content[file_list[i].content.text.i];
        }
        abort();
    }

    inline bool is_parent(const int parent, const int child) const {
        return file_list[child].parent == parent;
    }

    inline int get_current() const { return current; }

    inline int add_text_content(const std::vector<std::string>& content) {
        int rid = text_content.size();
        text_content.push_back(content);
        return rid;
    }

    inline void add_dir(const std::string& name, const std::string& path) {
        int i = from_path(path, -1);
        if (!is_dir(i)) {
            abort();
        }
        file_list.push_back({
            .name = name,
            .content = {.directory = {}},
            .parent = i,
        });
    }

    inline void add_text(const std::string& name,
                         const std::vector<std::string>& content,
                         const std::string& path) {
        int i = from_path(path, -1);
        if (!is_dir(i)) {
            abort();
        }
        file_list.push_back({
            .name = name,
            .content = {.text = {.i = add_text_content(content)}},
            .parent = i,
        });
    }

   private:
    int current = -1;
    std::vector<FileUnit> file_list;
    std::vector<std::vector<std::string>> text_content;
};
