#ifndef __MODVIEW_H__
#define __MODVIEW_H__

#include "modfile.h"
#include "modi18n.h"
#include "modinter.h"
#include <ctime>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

#define TIME_FMT_SHORT "%m-%d"
#define TIME_FMT_LONG "%Y-%m-%d"
extern int state;
#define EXIT_DISPLAY 0x1
#define DETAIL_VIEW_DISPLAY 0x2
#define TAG_VIEW_DISPLAY 0x4
#define SAVE_DONE_DISPLAY 0x8
#define SETTING_DISPLAY 0x10

class MainView : public ftxui::ComponentBase {
  private:
    TodoInfo *info;
    int selected;
    int length;
    ftxui::ScreenInteractive *screen;
  public:
    MainView(TodoInfo *g_info, ftxui::ScreenInteractive *g_screen): info(g_info), 
        screen(g_screen) {}
    ftxui::Element Render(void) override;
    bool OnEvent(ftxui::Event event) override;
};

class DetailView : public ftxui::ComponentBase {
  private:
    TodoInfo *info;
    std::string name;
    ftxui::Component nameInput;
    std::string subtask;
    ftxui::Component subtaskInput;
    int priority;
    std::vector<std::string> priorityStrings;
    ftxui::Component priorityToggle;
    std::string start;
    ftxui::Component startInput;
    std::string ddl;
    ftxui::Component ddlInput;
    std::string desc;
    ftxui::Component descInput;
    ftxui::Component confirmButton;
    std::vector<ftxui::Component> checkboxes;
    ftxui::Component checkboxContainer;
    bool *checkStates;
    ftxui::Component container; // overall container
    TodoItem *item;
  public:
    DetailView(TodoInfo *g_info): info(g_info), checkStates(nullptr), 
            nameInput(ftxui::Input(name)),
            subtaskInput(ftxui::Input(subtask)),
            startInput(ftxui::Input(start)),
            ddlInput(ftxui::Input(ddl)),
            descInput(ftxui::Input(desc)) {
        priorityStrings = {
            GETTEXT(CRITIAL_PRIORITY),
            GETTEXT(IMPORTANT_PRIORITY),
            GETTEXT(NORMAL_PRIORITY),
            GETTEXT(ORDINARY_PRIORITY)
        };
        priorityToggle = ftxui::Toggle(&priorityStrings, &priority);
        confirmButton = ftxui::Button(GETTEXT(OK), [&] {
            struct tm tmp;
            time_t startTime;
            time_t deadline;
            if (start.empty())
                startTime = 0;
            else {
                std::stringstream startStream(start);
                startStream >> std::get_time(&tmp, TIME_FMT_LONG);
                startTime = mktime(&tmp);
            }
            if (ddl.empty())
                deadline = 0;
            else {
                std::stringstream ddlStream(ddl);
                ddlStream >> std::get_time(&tmp, TIME_FMT_LONG);
                deadline = mktime(&tmp);
            };

            state &= ~DETAIL_VIEW_DISPLAY;
            std::vector<int> tagList;
            for (int i = 0; i < info->tagCount; i++)
                if (checkStates[i])
                    tagList.push_back(i);

            std::vector<std::string> subtaskList;
            std::string token;
            std::stringstream stream(subtask);
            while (std::getline(stream, token, '|'))
                subtaskList.push_back(token);
            const char **subtasks = new const char *[subtaskList.size()];
            for (int i = 0; i < subtaskList.size(); i++)
                subtasks[i] = subtaskList[i].c_str();

            if (!item)
                AddTodoItem(info, name.c_str(), subtasks, subtaskList.size(),
                        tagList.data(), tagList.size(), priority, startTime, deadline, desc.c_str());
            else
                ModifyTodoItem(info, item - info->items, name.c_str(), subtasks, subtaskList.size(),
                        tagList.data(), tagList.size(), priority, startTime, deadline, desc.c_str());
        });
    }
    void reset(TodoItem *itemIn);
    ftxui::Element Render(void) override;
    bool OnEvent(ftxui::Event event) override;
};
#endif
