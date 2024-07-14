# todomgr

A simple todo manager in TUI with cross-platform support, and multi-language support (Chinese & English).

## Short graphical demo

[![asciinema CLI demo](https://asciinema.org/a/nbJmJXkh6dyQ5W0ZSBydw1yxb.svg)](https://asciinema.org/a/nbJmJXkh6dyQ5W0ZSBydw1yxb?autoplay=1)

## Dependencies

[FTXUI](https://github.com/ArthurSonzogni/FTXUI), MIT License

## Todos

- [x] 程序主菜单设计
- [x] 任务条目（名称，子任务，标签，优先级，开始时间，结束时间，描述）
- [x] i18n（中文与英文）
- [x] TUI with ftxui

## Building

Clone the project, then run `cmake .` at the project root path.
The executable `todomgr` will be generated in `build/`. Note that
when using Windows, you may need to copy dependencies in `_deps/ftxui-build/`
to `build/` to help Windows load DLLs correctly.

## Progress

这个项目作为C语言程序设计的作业，已完成实现。本来我的想法是设计成一个日常可用的程序，
但是限于时间，最终只能完成在"demo"阶段。总体来说是可用的，但是有不少缺陷，
如没有考虑文件膨胀的情况，输入时间时也没有做检查。总而言之，这个项目已经走到了生命尽头，
将停止开发，如果有PR的话我会考虑合入，不接受issue，因为我没空去修了。

**不确保不含有重大漏洞**  
**Vulnerabilities may be contained in the project!!**

不过如果有人也想基于FTXUI来做“作业”的话，欢迎借鉴我的项目。如果觉得有用的话，
欢迎给我点个⭐。

## Copyright

Copyright 2024 RocketDev, dudu, hyl, distributed under GPL-3.0
