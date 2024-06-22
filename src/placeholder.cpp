#include <clocale>
#include <ftxui/component/component.hpp> // for Renderer, Button, Input, etc.
#include <ftxui/component/screen_interactive.hpp> // for ScreenInteractive
#include <string> // for std::string

using namespace ftxui;

int main() {
    setlocale(LC_ALL, "zh_CN.UTF-8");
  auto screen = ScreenInteractive::Fullscreen();
  screen.TrackMouse(false);

  std::string message = "你好，FTXUI！";

  auto quit_button = Button("按[回车]退出", screen.ExitLoopClosure());

  auto renderer = Renderer(quit_button, [&] {
    return vbox({
      text(message) | center,
      quit_button->Render() | center,
    }) | vcenter;
  });

  screen.Loop(renderer);
  return 0;
}


