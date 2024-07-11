#include <cerrno>
#include <clocale>
#include <cstdio>
#include <cstring>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/color.hpp>
#include <ftxui/screen/screen.hpp>
#include <memory>
#include <utility>

#include "modi18n.h"
#include "modview.hpp"
#include "modfile.h"

using namespace ftxui;

ScreenInteractive *screen;
static TodoInfo *info;

int main(void) {
    setlocale(LC_ALL, "");
    const char *orig = setlocale(LC_ALL, NULL);
    if (strstr(orig, "zh_CN") || strstr(orig, "Simplified")) 
        *GetLanguage() = 1;
    else
        *GetLanguage() = 0;
    setlocale(LC_ALL, "zh_CN.UTF-8");

    puts("Loading...");
    info = InitTodoInfo();
    int err = ReadTodoFile(info, "tododb");
    if (err == ENOENT)
        puts("No data! Create one with save.");
    else if (err == EPERM)
        puts("No permission to read!");
    else if (err != 0) 
        perror("Error when reading db");

    auto main_screen = ScreenInteractive::Fullscreen();
    screen = &main_screen;
    screen->TrackMouse(false); // Disable mouse

    MainView view(info);
    auto renderer = Renderer([&]{return view.Render();});
    screen->Loop(renderer);
    ReleaseTodoInfo(info);
    return 0;
}
