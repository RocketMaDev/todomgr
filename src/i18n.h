#ifndef _TODOMGR_I18N_H_
#define _TODOMGR_I18N_H_

#define TOTAL 30

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
    {"No", "否"},
#define NAME_HEADER 5
    {"Name", "任务"},
#define SUBTASK_HEADER 6
    {"Subtasks", "子任务"},
#define TAG_HEADER 7
    {"Tags", "标签"},
#define START_TIME_HEADER 8
    {"StartTime", "开始时间"},
#define DEADLINE_HEADER 9
    {"Deadline", "截止时间"},
#define DESCRIPTION_HEADER 10
    {"Description", "描述"},
#define NO_ITEM_PROMPT 11
    {"No item right now. Please add one.", "还没有任务"},
#define MAIN_TITLE 12
    {"TodoManager", "任务管理器"}
};

#define GETTEXT(STR) \
    (i18nstr[STR][zh])

#endif
