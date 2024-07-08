#ifndef _TODOMGR_I18N_H_
#define _TODOMGR_I18N_H_

#define TOTAL 10

int zh = 0;
const char *i18nstr[TOTAL][2] = {
#define INIT_PROMPT 0
    {"Loading...", "加载中..."},
#define CONFIRM_EXIT 1
    {"Comfirm to exit", "确认退出程序"},
#define EXIT_PROMPT 2
    {"Exiting...", "正在退出..."},
#define YES 3
    {"Yes", "是"},
#define NO 4
    {"No", "否"}
};

#define GETTEXT(STR) \
    (i18nstr[STR][zh])

#endif
