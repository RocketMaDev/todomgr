#include "modi18n.h"

#define EXPAND_STR(IDX, EN, ZH) \
    [IDX] = {EN, ZH}

int zh = 0;
const char *i18nstr[TOTAL][2] = {
    EXPAND_STR(INIT_PROMPT, "Loading...",  "加载中..."),
    EXPAND_STR(CONFIRM_EXIT, "Comfirm to exit",  "确认退出程序"),
    EXPAND_STR(EXIT_PROMPT, "Exiting...",  "正在退出..."),
    EXPAND_STR(YES, "Yes",  "是"),
    EXPAND_STR(NO, "No",  "否"),
    EXPAND_STR(NAME_HEADER, "Name",  "任务"),
    EXPAND_STR(SUBTASK_HEADER, "Subtasks",  "子任务"),
    EXPAND_STR(TAG_HEADER, "Tags",  "标签"),
    EXPAND_STR(START_TIME_HEADER, "StartTime",  "开始时间"),
    EXPAND_STR(DEADLINE_HEADER, "Deadline",  "截止时间"),
    EXPAND_STR(DESCRIPTION_HEADER, "Description",  "描述"),
    EXPAND_STR(NO_ITEM_PROMPT, "No item right now. Please add one.",  "还没有任务"),
    EXPAND_STR(MAIN_TITLE, "TodoManager",  "任务管理器"),
};

int *GetLanguage(void) {
    return &zh;
}

const char *GETTEXT(int idx) {
    return i18nstr[idx][zh];
}
