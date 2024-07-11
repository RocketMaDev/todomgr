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
class MainView : public ftxui::ComponentBase {
  private:
    TodoInfo *info;
    int selected;
    int length;
  public:
    MainView(TodoInfo *g_info): info(g_info) {}
    ftxui::Element RenderView(ftxui::ScreenInteractive *screen);
    bool OnEvent(ftxui::Event event) override;
};


#endif
