#include <clocale>
#include <cstring>
#include <ftxui/component/component.hpp> // for Renderer, Button, Input, etc.
#include <ftxui/component/screen_interactive.hpp> // for ScreenInteractive
#include <string> // for std::string
#include "i18n.h"

using namespace ftxui;

int main() {
    setlocale(LC_ALL, "");
    const char *orig = setlocale(LC_ALL, NULL);
    if (strstr(orig, "zh_CN") || strstr(orig, "Simplified")) 
        zh = 1;
    else
        zh = 0;
    setlocale(LC_ALL, "zh_CN.UTF-8");
    auto screen = ScreenInteractive::Fullscreen();
    screen.TrackMouse(false);


    auto quit_button = Button(GETTEXT(EXIT), screen.ExitLoopClosure());

    auto renderer = Renderer(quit_button, [&] {
            return vbox({
                    text(GETTEXT(HELLO_FTXUI)) | center,
                    quit_button->Render() | center,
                    }) | vcenter;
            });

    screen.Loop(renderer);
    return 0;
}


