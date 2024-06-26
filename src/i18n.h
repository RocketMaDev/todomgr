#ifndef _TODOMGR_I18N_H_
#define _TODOMGR_I18N_H_

#define TOTAL 2

int zh = 0;
const char *i18nstr[TOTAL][2] = {
#define HELLO_FTXUI 0
    {"Hello, FTXUI!", "你好，FTXUI！"},
#define EXIT 1
    {"Press [ENTER] to exit", "按回车键退出"}
};

#define GETTEXT(STR) \
    (i18nstr[STR][zh])

#endif
