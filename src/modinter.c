#include "modinter.h"
#include <string.h>
#include <time.h>
#include <stdlib.h>
static time_t curtime;

void MarkDone(TodoInfo *g_info, int itemIndex) {
    g_info->items[itemIndex].done = true;
}

void MarkUndone(TodoInfo *g_info, int itemIndex) {
    g_info->items[itemIndex].done = false;
}

static int cmp_name(const void* a, const void* b) {
    return strcmp(((TodoItem *)a)->name, ((TodoItem *)b)->name);
}

static int cmp_priority(const void* a, const void* b) {
    return ((TodoItem *)a)->priority - ((TodoItem *)b)->priority;
}

static int cmp_starttime(const void* a, const void* b) {
    return ((TodoItem *)a)->startTime - ((TodoItem *)b)->startTime;
}

static int cmp_deadline(const void* a, const void* b) {
    return ((TodoItem *)a)->deadline - ((TodoItem *)b)->deadline;
}

void SortTodoInfo(TodoInfo *g_info, enum SortType type) {
    curtime = time(NULL);
    switch (type) {
    case NAME :
        qsort(g_info->items, g_info->todoCount, sizeof(TodoItem), cmp_name);
        break;
    case PRIORITY :
        qsort(g_info->items, g_info->todoCount, sizeof(TodoItem), cmp_priority);
        break;
    case START_TIME :
        qsort(g_info->items, g_info->todoCount, sizeof(TodoItem), cmp_starttime);
        break;
    case DEADLINE :
        qsort(g_info->items, g_info->todoCount, sizeof(TodoItem), cmp_deadline);
        break;
    default:
        break;
    }
}