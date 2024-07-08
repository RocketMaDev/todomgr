#include <clocale>
#include <cstring>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/color.hpp>
#include <ftxui/screen/screen.hpp>
#include "i18n.h"

using namespace ftxui;

ScreenInteractive *screen;

int main(void) {
    setlocale(LC_ALL, "");
    const char *orig = setlocale(LC_ALL, NULL);
    if (strstr(orig, "zh_CN") || strstr(orig, "Simplified")) 
        zh = 1;
    else
        zh = 0;
    setlocale(LC_ALL, "zh_CN.UTF-8");
    auto main_screen = ScreenInteractive::Fullscreen();
    screen = &main_screen;
    screen->TrackMouse(false); // Disable mouse

    
}

static void initScreen(void) {
    auto loading = text(GETTEXT(INIT_PROMPT)) | center;
    auto box = border(loading) | Decorator(color(Color::Green));
}

static void exitScreen(void) {
    auto loading = text(GETTEXT(EXIT_PROMPT)) | center;
    auto box = border(loading) | Decorator(color(Color::Green));
}

static void onEsc(void) {
    auto win = window(text(GETTEXT(CONFIRM_EXIT)), hbox({
                Button(GETTEXT(YES), [&] {
                        screen->ExitLoopClosure();
                })->Render(),
                separator(),
                Button(GETTEXT(NO), []{})->Render()
        })) | color(Color::Green);
}

static void onModify(void) {}
static void onAdd(void) {}
static void mainView(void) {}
