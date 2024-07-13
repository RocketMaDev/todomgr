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
#include <string>
#include <utility>
#include <vector>

#include "modi18n.h"
#include "modview.hpp"
#include "modfile.h"

using namespace ftxui;
using namespace std;

ScreenInteractive *screen;
static TodoInfo *info;
static void manageComponents(void);

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

    *GetLanguage() = info->lang;
    auto main_screen = ScreenInteractive::Fullscreen();
    screen = &main_screen;
    screen->TrackMouse(false); // Disable mouse

    manageComponents();
    puts("Releasing...");
    ReleaseTodoInfo(info);
    return 0;
}

static void manageComponents(void) {
    auto btnYes = Button(GETTEXT(YES), screen->ExitLoopClosure());
    auto btnNo = Button(GETTEXT(NO), [&] { state &= ~EXIT_DISPLAY; });
    auto exitComponent = Container::Horizontal({
        btnYes,
        btnNo
    });

    vector<string> langStrings = {GETTEXT(ENGLISH_STR), GETTEXT(CHINESE_STR)};
    vector<string> sortTypeStrings = {GETTEXT(SMART_SORT), GETTEXT(NAME_SORT), GETTEXT(PRIORITY_SORT),
            GETTEXT(START_TIME_SORT), GETTEXT(DEADLINE_SORT)};
    int langSelected = info->lang, sortTypeSelected = info->sortType;
    auto langToggle = Toggle(&langStrings, &langSelected);
    auto sortTypeToggle = Toggle(&sortTypeStrings, &sortTypeSelected);
    auto btnApply = Button(GETTEXT(OK), [&] {
        info->lang = langSelected;
        *GetLanguage() = langSelected;
        info->sortType = (enum SortType)sortTypeSelected;
        state &= ~SETTING_DISPLAY;
    });
    auto settingsComponent = Container::Vertical({
        langToggle,
        sortTypeToggle,
        btnApply
    });

    auto doneButton = Button(GETTEXT(OK), [&] { 
        state &= ~SAVE_DONE_DISPLAY; 
    });

    auto mainComponent = make_shared<MainView>(info, screen);
    auto detailComponent = make_shared<DetailView>(info);
    auto tagComponent = make_shared<TagView>(info);

    auto ultimateContainer = Container::Vertical({
        mainComponent,
        settingsComponent,
        exitComponent,
        detailComponent,
        tagComponent
    });
    puts("Starting FTXUI instance...");
    screen->Loop(Renderer(ultimateContainer, [&] {
        if (state & EXIT_DISPLAY) {
            exitComponent->TakeFocus();
            return dbox({
                mainComponent->Render(),
                window(text(GETTEXT(CONFIRM_EXIT)), hbox({
                    btnYes->Render(),
                    separator(),
                    btnNo->Render()
                })) | center
            });
        }
        if (state & SETTING_DISPLAY) {
            settingsComponent->TakeFocus();
            return dbox({
                mainComponent->Render(),
                window(text(GETTEXT(SETTINGS_STR)), vbox({
                    hbox({
                        vbox({
                            text(GETTEXT(LANGUAGE_STR)),
                            text(GETTEXT(SORT_TYPE_STR))
                        }),
                        vbox({
                            langToggle->Render(),
                            sortTypeToggle->Render()
                        })
                    }),
                    btnApply->Render() | notflex
                })) | center
            });
        }
        if (state & DETAIL_VIEW_DISPLAY) {
            if (!(state & DETAIL_SWITCH_STATE)) {
                detailComponent->reset(mainComponent->getTodoItem());
                state |= DETAIL_SWITCH_STATE;
            }
            detailComponent->TakeFocus();
            return dbox({
                mainComponent->Render(),
                detailComponent->Render()
            });
        }
        if (state & TAG_VIEW_DISPLAY) {
            tagComponent->TakeFocus();
            return dbox({
                mainComponent->Render(),
                tagComponent->Render()
            });
        }
        if (state & SAVE_DONE_DISPLAY) {
            doneButton->TakeFocus();
            return dbox({
                mainComponent->Render(),
                window(text(GETTEXT(SAVE_DONE)), doneButton->Render() | color(Color::Green)) | center
            });
        }
        if (state & DETAIL_SWITCH_STATE)
            state &= ~DETAIL_SWITCH_STATE;
        mainComponent->TakeFocus();
        return mainComponent->Render();
    }));
}
