#ifndef __MODVIEW_H__
#define __MODVIEW_H__

#include "modfile.h"
#include "modi18n.h"
#include "modinter.h"
#include <ctime>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <iomanip>
#include <list>
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
#define DETAIL_SWITCH_STATE 0x20

class MainView : public ftxui::ComponentBase {
  private:
    TodoInfo *info;
    int selected;
    int length;
    ftxui::ScreenInteractive *screen;
    TodoItem *itemHandle;
  public:
    MainView(TodoInfo *g_info, ftxui::ScreenInteractive *g_screen): info(g_info), 
        screen(g_screen), selected(0) {}
    ftxui::Element Render(void) override;
    bool OnEvent(ftxui::Event event) override;
    TodoItem *getTodoItem(void) { return itemHandle; }
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
    DetailView(TodoInfo *g_info);
    void reset(TodoItem *itemIn);
    ftxui::Element Render(void) override;
    bool OnEvent(ftxui::Event event) override;
};

class TagView : public ftxui::ComponentBase {
  private:
    std::string newTag;
    ftxui::Component newInput;
    std::list<std::string> tags;
    TodoInfo *info;
    int selected;
  public:
    TagView(TodoInfo *g_info);
    ftxui::Element Render(void) override;
    bool OnEvent(ftxui::Event event) override;
};
#endif
