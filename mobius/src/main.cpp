#include <thread>
#include <raylib.h>

#include "client.h"

int main(int argc, char* argv[]) {

    auto file_guy = std::make_shared<FileGuy>(std::vector<FileUnit>{});

    file_guy->add_dir("cum1", "~");
    file_guy->add_dir("cum2", "~");
    file_guy->add_dir("cum3", "~");
    file_guy->add_text("cum4",
                       {
                           "among us",
                           "skibidi toilet",
                           "sigma",
                       },
                       "cum3");

    InitWindow(640, 480, "mobius");

    auto shell = std::make_shared<Shell>(file_guy);
    auto terminal = std::make_shared<Terminal>(shell);

    const Font cp437 = LoadFont("./data/fonts/PerfectDOSVGA437.ttf");

    std::thread listener(client);

    while (!WindowShouldClose() || errno == 0) {
        const float delta = GetFrameTime();

        terminal->update(delta);

        BeginDrawing();
        ClearBackground(BLACK);
        terminal->draw(cp437);
        EndDrawing();
    }

    listener.join();
    CloseWindow();

    return 0;
}
