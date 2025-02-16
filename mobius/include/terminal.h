#pragma once

#include <raylib.h>

#include "shell.h"

inline bool check_terminal_key(const int key) {
    return IsKeyPressed(key) || IsKeyPressedRepeat(key);
}

class Terminal {
   public:
    Terminal(std::shared_ptr<Shell> shell_in = {}) : shell(shell_in) {
        print(shell->prompt());
    }

    inline void update(const float delta) {
        int key = GetCharPressed();

        while (key > 0) {
            if ((key >= 32) && (key <= 125)) {
                line.insert(position, 1, key);
                ++position;
            }

            key = GetCharPressed();  // Check next character in the queue
        }

        if (position > 0 && check_terminal_key(KEY_LEFT)) {
            --position;
        } else if (position < line.size() && check_terminal_key(KEY_RIGHT)) {
            ++position;
        } else if (check_terminal_key(KEY_ENTER)) {
            print("> " + line);
            for (const auto& rep : shell->parse(line)) {
                print(rep);
            }
            print(shell->prompt());
            line = {};
            position = 0;
        } else if (position > 0 && check_terminal_key(KEY_BACKSPACE)) {
            line.erase(position - 1, 1);
            --position;
        }
    }

    inline void draw(const Font& font) const {
        for (int i = 0; i < buffer.size(); ++i) {
            DrawTextEx(font, buffer[i].c_str(), {0, static_cast<float>(i * 16)},
                       16, 0, GREEN);
        }
        DrawTextEx(font, ("> " + line).c_str(),
                   {0, static_cast<float>(buffer.size() * 16)}, 16, 0, GREEN);
        const float x =
            MeasureTextEx(font, ("> " + line).substr(0, position + 2).c_str(),
                          16, 0)
                .x;
        DrawLineEx({x, static_cast<float>(buffer.size() * 16)},
                   {x, static_cast<float>(buffer.size() * 16 + 16)}, 2, GREEN);
    }

    inline void print(const std::string& text) {
        buffer.push_back(text);
        while ((buffer.size() + 1) * 16 > 480) {
            buffer.erase(buffer.begin());
        }
    }

   private:
    int position = 0;
    std::vector<std::string> buffer;
    std::string line;

    std::shared_ptr<Shell> shell;
};
