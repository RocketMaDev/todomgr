#ifndef __MODINTER_H__
#define __MODINTER_H__

#include <time.h>
#include "modfile.h"

int AddTodoItem(TodoInfo *g_info, const char *name, const char *subtasks, const int *tags,
       enum Priority priority, time_t startTime, time_t deadline, const char *desc);
int DeleteTodoItem(TodoInfo *g_info, int itemIndex);
int ModifyTodoItem(TodoInfo *g_info, int itemIndexm, const char *name, const char *subtasks, 
        const int *tags,enum Priority priority, time_t startTime, time_t deadline, const char *desc);
void SortTodoInfo(TodoInfo *g_info, enum SortType type);

void MarkDone(TodoInfo *g_info, int itemIndex);
void MarkUndone(TodoInfo *g_info, int itemIndex);
#endif
