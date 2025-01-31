#ifndef _TODOMGR_I18N_H_
#define _TODOMGR_I18N_H_

#ifdef __cplusplus
extern "C" {
#endif

int *GetLanguage(void);
const char *GETTEXT(int idx);

#define INIT_PROMPT 0
#define CONFIRM_EXIT 1
#define EXIT_PROMPT 2
#define YES 3
#define NO 4
#define NAME_HEADER 5
#define SUBTASK_HEADER 6
#define TAG_HEADER 7
#define START_TIME_HEADER 8
#define DEADLINE_HEADER 9
#define DESCRIPTION_HEADER 10
#define NO_ITEM_PROMPT 11
#define MAIN_TITLE 12
#define ADD_TASK_HINT 13
#define DELETE_TASK_HINT 14
#define VIEW_TAG_HINT 15
#define SWITCH_STATE_HINT 16
#define VIEW_DETAIL_HINT 17
#define EXIT_HINT 18
#define EDIT_SETTING_HINT 19
#define HINT_TITLE 20
#define SAVE_HINT 21
#define DETAIL_TITLE 22
#define NAME_INPUT 23
#define PRIORITY_INPUT 24
#define START_TIME_INPUT 25
#define DEADLINE_INPUT 26
#define DESCRIPTION_INPUT 27
#define TIME_FMT_HINT 28
#define OK 29
#define CRITIAL_PRIORITY 30
#define IMPORTANT_PRIORITY 31
#define NORMAL_PRIORITY 32
#define ORDINARY_PRIORITY 33
#define SUBTASK_INPUT 34
#define SUBTASK_HINT 35
#define SORT_TYPE_STR 36
#define SMART_SORT 37
#define NAME_SORT 38
#define PRIORITY_SORT 39
#define START_TIME_SORT 40
#define DEADLINE_SORT 41
#define LANGUAGE_STR 42
#define ENGLISH_STR 43
#define CHINESE_STR 44
#define SETTINGS_STR 45
#define NEW_TAG_PROMPT 46
#define NO_TAG_PROMPT 47
#define DELETE_TAG_PROMPT 48
#define SAVE_DONE 49

#ifdef __cplusplus
}
#endif

#endif
