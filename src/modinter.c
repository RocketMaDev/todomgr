#include "modinter.h"

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include "modfile.h"
static time_t curtime;

void MarkDone(TodoInfo *g_info, int itemIndex) {
    g_info->items[itemIndex].done = true;
}

void MarkUndone(TodoInfo *g_info, int itemIndex) {
    g_info->items[itemIndex].done = false;
}

static int cmp_name(const void *a, const void *b) {
    if (!((TodoItem *)a)->done && ((TodoItem *)b)->done)
        return -1;
    else if (((TodoItem *)a)->done && !((TodoItem *)b)->done)
        return 1;
    return strcmp(((TodoItem *)a)->name, ((TodoItem *)b)->name);
}

static int cmp_priority(const void *a, const void *b) {
    if (!((TodoItem *)a)->done && ((TodoItem *)b)->done)
        return -1;
    else if (((TodoItem *)a)->done && !((TodoItem *)b)->done)
        return 1;
    return ((TodoItem *)a)->priority - ((TodoItem *)b)->priority;
}

static int cmp_starttime(const void *a, const void *b) {
    if (!((TodoItem *)a)->done && ((TodoItem *)b)->done)
        return -1;
    else if (((TodoItem *)a)->done && !((TodoItem *)b)->done)
        return 1;
    return ((TodoItem *)a)->startTime - ((TodoItem *)b)->startTime;
}

static int cmp_deadline(const void *a, const void *b) {
    if (!((TodoItem *)a)->done && ((TodoItem *)b)->done)
        return -1;
    else if (((TodoItem *)a)->done && !((TodoItem *)b)->done)
        return 1;
    return ((TodoItem *)a)->deadline - ((TodoItem *)b)->deadline;
}

static int cmp_smart(const void *a, const void *b) {
    if (!((TodoItem *)a)->done && ((TodoItem *)b)->done)
        return -1;
    else if (((TodoItem *)a)->done && !((TodoItem *)b)->done)
        return 1;
    if (((TodoItem *)a)->deadline > ((TodoItem *)b)->deadline)
        return 1;
    else if (((TodoItem *)a)->deadline == ((TodoItem *)b)->deadline)
        if (((TodoItem *)a)->priority > ((TodoItem *)b)->priority)
            return 1;
        else
            return -1;
    else
        return -1;
}

void SortTodoInfo(TodoInfo *g_info, enum SortType type) {
    switch (type) {
        case NAME:
            qsort(g_info->items, g_info->todoCount, sizeof(TodoItem), cmp_name);
            break;
        case PRIORITY:
            qsort(g_info->items, g_info->todoCount, sizeof(TodoItem), cmp_priority);
            break;
        case START_TIME:
            qsort(g_info->items, g_info->todoCount, sizeof(TodoItem), cmp_starttime);
            break;
        case DEADLINE:
            qsort(g_info->items, g_info->todoCount, sizeof(TodoItem), cmp_deadline);
            break;
        case SMART:
            qsort(g_info->items, g_info->todoCount, sizeof(TodoItem), cmp_smart);
            break;
        default:
            puts("Shold never happen");
            break;
    }
}

int AddTodoItem(TodoInfo *g_info, const char *name, const char **subtasks,
                int subtaskCount, const int *tags, int tagCount, enum Priority priority,
                time_t startTime, time_t deadline, const char *desc) {
    if (!g_info || !name || !desc)
        return -1;
    TodoItem new_item;
    new_item.name = strdup(name);
    new_item.desc = strdup(desc);
    new_item.done = false;
    new_item.tagList = (int *)malloc(sizeof(int) * tagCount);
    new_item.tagCount = tagCount;
    memcpy(new_item.tagList, tags, tagCount * sizeof(int));
    new_item.subtaskList = (char **)malloc(subtaskCount * sizeof(char *));
    for (int i = 0; i < subtaskCount; i++)
        new_item.subtaskList[i] = strdup((char *)subtasks[i]);
    new_item.subtaskCount = subtaskCount;
    new_item.priority = priority;
    new_item.startTime = startTime;
    new_item.deadline = deadline;
    new_item.desc = strdup(desc);

    g_info->todoCount++;
    TodoItem *newItems = (TodoItem *)malloc(sizeof(TodoItem) * g_info->todoCount);
    memcpy(newItems, g_info->items, sizeof(TodoItem) * (g_info->todoCount - 1));
    newItems[g_info->todoCount - 1] = new_item;
    free(g_info->items);
    g_info->items = newItems;
    return 0;
}

int DeleteTodoItem(TodoInfo *g_info, int itemIndex) {
    if (!g_info || itemIndex < 0 || itemIndex >= g_info->todoCount)
        return -1;
    TodoItem *item = g_info->items + itemIndex;
    free(item->desc);
    free(item->name);
    free(item->tagList);
    for (int i = 0; i < item->subtaskCount; i++)
        free(item->subtaskList[i]);
    memmove(item, item + 1, (g_info->todoCount - itemIndex - 1) * sizeof(TodoItem));
    g_info->todoCount--;
    return 0;
}

int ModifyTodoItem(TodoInfo *g_info, int itemIndex, const char *name,
                   const char **subtasks, int subtaskCount, const int *tags, int tagCount,
                   enum Priority priority, time_t startTime, time_t deadline,
                   const char *desc) {
    if (!g_info || itemIndex < 0 || itemIndex >= g_info->todoCount)
        return -1;
    TodoItem *item = g_info->items + itemIndex;
    if (strcmp(name, item->name)) {
        free(item->name);
        item->name = strdup(name);
    }
    if (strcmp(desc, item->desc)) {
        free(item->desc);
        item->desc = strdup(desc);
    }
    if (tagCount > item->tagCount) {
        free(item->tagList);
        item->tagList = (int *)malloc(tagCount * sizeof(int));
    }
    memcpy(item->tagList, tags, tagCount * sizeof(int));
    item->tagCount = tagCount;
    item->deadline = deadline;
    item->startTime = startTime;
    item->priority = priority;
    if (subtaskCount > item->subtaskCount) {
        free(item->subtaskList);
        item->subtaskList = (char **)malloc(subtaskCount * sizeof(char *));
    }
    for (int i = 0; i < item->subtaskCount; i++)
        free(item->subtaskList[i]);
    for (int i = 0; i < subtaskCount; i++)
        item->subtaskList[i] = strdup(subtasks[i]);
    item->subtaskCount = subtaskCount;
    return 0;
}

int AddTag(TodoInfo *g_info, const char *newTag) {
    if (g_info == NULL || newTag == NULL)
        return -1;
    char **newTagList = (char **)malloc((g_info->tagCount + 1) * sizeof(char *));
    if (newTagList == NULL)
        return -1;
    memcpy(newTagList, g_info->tags, g_info->tagCount * sizeof(char *));
    newTagList[g_info->tagCount] = strdup(newTag);
    free(g_info->tags);
    g_info->tags = newTagList;
    g_info->tagCount++;
    return 0;
}

int DeleteTag(TodoInfo *g_info, int toDeleteTag) {
    if (g_info == NULL || g_info->tagCount - 1 < 0)
        return -1;
    memmove(g_info->tags + toDeleteTag, g_info->tags + toDeleteTag + 1, 
            (g_info->tagCount - toDeleteTag - 1) * sizeof(char *));
    g_info->tagCount--;

    // clear tag indexes in todo items
    for (int i = 0; i < g_info->todoCount; i++) {
        int found = 0;
        int *taglist = g_info->items[i].tagList;
        for (int j = 0; j < g_info->items[i].tagCount; j++) {
            if (taglist[j] == toDeleteTag) {
                found = 1;
                break;
            }
        }
        if (found) {
            for (int j = 0; j < g_info->items[i].tagCount - found; j++)
                if (taglist[j] >= toDeleteTag)
                    taglist[j] = taglist[j + 1] - 1;
            g_info->items[i].tagCount--;
        } else {
            for (int j = 0; j < g_info->items[i].tagCount; j++)
                if (taglist[j] > toDeleteTag)
                    taglist[j]--;
        }
    }
    return 0;
}
