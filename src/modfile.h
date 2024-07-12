#ifndef __MODFILE_H__
#define __MODFILE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <time.h>

#define BUFSIZE 0x8000

enum SortType { SMART, NAME, PRIORITY, START_TIME, DEADLINE };

enum Priority { CRITICAL, IMPORTANT, NORMAL, ORDINARY };

typedef struct __todo_file_item {
    char *name;
    unsigned char subtaskCnt;
    char **subtasks;
    unsigned short tags;
    unsigned short *taglist;
    unsigned char done : 1;
    unsigned char priority : 7;
    time_t start_time;
    time_t deadline;
    char *desc;
} TodoFileItem;

typedef struct __todo_file {
    unsigned char tagPos;
    unsigned int lang : 1;
    unsigned char sortType;

    unsigned short tagcnt;
    char **tagitem;

    int todocnt;
    TodoFileItem *items;
} TodoFile;

typedef struct __todo_item {
    char *name;
    int subtaskCount;
    char **subtaskList;
    int tagCount;
    int *tagList;
    bool done;
    enum Priority priority;
    time_t startTime;
    time_t deadline;
    char *desc;
} TodoItem;

typedef struct __todo_info {
    bool lang;
    enum SortType sortType;

    int tagCount;
    char **tags;

    int todoCount;
    TodoItem *items;
} TodoInfo;

TodoInfo *InitTodoInfo(void);
int ReadTodoFile(TodoInfo *g_info, const char *filepath);
int WriteTodoFile(TodoInfo *g_info, const char *filepath);
void ReleaseTodoInfo(TodoInfo *g_info);

#ifdef __cplusplus
}
#endif

#endif
