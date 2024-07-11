#ifndef _TODOMGR_I18N_H_
#define _TODOMGR_I18N_H_

#ifdef __cplusplus
extern "C" {
#endif

#define TOTAL 30
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

#ifdef __cplusplus
}
#endif

#endif
