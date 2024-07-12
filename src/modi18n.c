#include "modi18n.h"

#define EXPAND_STR(IDX, EN, ZH) \
    [IDX] = {EN, ZH}

int zh = 0;
const char *i18nstr[][2] = {
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
    EXPAND_STR(ADD_TASK_HINT, "Add a task", "添加一个任务"),
    EXPAND_STR(DELETE_TASK_HINT, "Delete a task", "删除一个任务"),
    EXPAND_STR(VIEW_TAG_HINT, "View tags", "查看所有标签"),
    EXPAND_STR(SWITCH_STATE_HINT, "Switch done state", "切换完成状态"),
    EXPAND_STR(VIEW_DETAIL_HINT, "View task detail", "查看任务细节"),
    EXPAND_STR(EXIT_HINT, "Exit", "退出"),
    EXPAND_STR(EDIT_SETTING_HINT, "Settings", "设置"),
    EXPAND_STR(HINT_TITLE, "Keyboard shortcuts", "快捷键"),
    EXPAND_STR(SAVE_HINT, "Save", "保存"),
    EXPAND_STR(DETAIL_TITLE, "Task detail", "任务详情"),
    EXPAND_STR(NAME_INPUT, "Task title: ", "任务名称："),
    EXPAND_STR(PRIORITY_INPUT, "Priority: ", "优先级"),
    EXPAND_STR(START_TIME_INPUT, "Start time: ", "开始时间："),
    EXPAND_STR(DEADLINE_INPUT, "Deadline: ", "截止时间："),
    EXPAND_STR(DESCRIPTION_INPUT, "Description: ", "描述："),
    EXPAND_STR(TIME_FMT_HINT, "Time format: e.g. 2024-12-30, subtask split with '|'", "时间格式：如2024-12-30子任务以'|'分割"),
    EXPAND_STR(OK, "OK", "确定"),
    EXPAND_STR(CRITIAL_PRIORITY, "Critial", "严重"),
    EXPAND_STR(IMPORTANT_PRIORITY, "Important", "重要"),
    EXPAND_STR(NORMAL_PRIORITY, "Normal", "正常"),
    EXPAND_STR(ORDINARY_PRIORITY, "Ordinary", "一般"),
    EXPAND_STR(SUBTASK_INPUT, "Subtasks: ", "子任务：")
};

int *GetLanguage(void) {
    return &zh;
}

const char *GETTEXT(int idx) {
    return i18nstr[idx][zh];
}
