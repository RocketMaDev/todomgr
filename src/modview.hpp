#ifndef __MODVIEW_H__
#define __MODVIEW_H__

#include "modfile.h"
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

#define TIME_FMT_SHORT "%m-%d"
#define TIME_FMT_LONG "%Y-%m-%d"
extern int state;
#define EXIT_DISPLAY 0x1
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


#endif
