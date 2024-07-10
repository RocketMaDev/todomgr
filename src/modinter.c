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
    if(!((TodoItem *)a)->done&&((TodoItem *)b)->done) 
        return -1;
    else if(((TodoItem *)a)->done&&!((TodoItem *)b)->done) 
        return 1;
    return strcmp(((TodoItem *)a)->name, ((TodoItem *)b)->name);
}

static int cmp_priority(const void* a, const void* b) {
    if(!((TodoItem *)a)->done&&((TodoItem *)b)->done) 
        return -1;
    else if(((TodoItem *)a)->done&&!((TodoItem *)b)->done) 
        return 1;
    return ((TodoItem *)a)->priority - ((TodoItem *)b)->priority;
}

static int cmp_starttime(const void* a, const void* b) {
    if(!((TodoItem *)a)->done&&((TodoItem *)b)->done) 
        return -1;
    else if(((TodoItem *)a)->done&&!((TodoItem *)b)->done) 
        return 1;
    return ((TodoItem *)a)->startTime - ((TodoItem *)b)->startTime;
}

static int cmp_deadline(const void* a, const void* b) {
    if(!((TodoItem *)a)->done&&((TodoItem *)b)->done) 
        return -1;
    else if(((TodoItem *)a)->done&&!((TodoItem *)b)->done) 
        return 1;
    return ((TodoItem *)a)->deadline - ((TodoItem *)b)->deadline;
}

static int cmp_smart(const void* a, const void* b) {
    if(!((TodoItem *)a)->done && ((TodoItem *)b)->done) 
        return -1;
    else if(((TodoItem *)a)->done && !((TodoItem *)b)->done) 
        return 1;
    if(((TodoItem *)a)->deadline > ((TodoItem *)b)->deadline) 
        return 1;
    else if(((TodoItem *)a)->deadline == ((TodoItem *)b)->deadline) {
        if(((TodoItem *)a)->priority > ((TodoItem *)b)->priority)
            return 1;
        else return -1;
    }
    else return -1;
}

void SortTodoInfo(TodoInfo *g_info,  enum SortType type) {
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
    case SMART :
        qsort(g_info->items, g_info->todoCount, sizeof(TodoItem), cmp_smart);
        break;
    default:
        break;
    }
}

void parseSubtasks(char *subtasksStr , TodoItem *new_item) {
    char *token = strtok(subtasksStr , " ");
    int subtasksCount = 0;
    while(token!=NULL) {
        strcpy(new_item->subtaskList[subtasksCount++] , token);
        token = strtok(NULL , " ");
    }
    new_item->subtaskCount = subtasksCount;
    new_item->subtaskList = *token;
}

int AddTodoItem(TodoInfo *g_info, const char *name, const char *subtasks, const int *tags ,
       enum Priority priority, time_t startTime, time_t deadline, const char *desc) {
        if(!g_info||!name||!desc)
            return -1;
        TodoItem *new_item = (TodoItem*)malloc(sizeof(TodoItem));
        if(!new_item)
            return -1;
        parseSubtasks(subtasks , new_item);
        new_item->name = strdup(name);
        new_item->desc = strdup(desc);
        new_item->done = false;
        new_item->tagList = tags;
        new_item->priority = priority;
        new_item->startTime = startTime;
        new_item->deadline = deadline;
        new_item->desc = desc;
        g_info->todoCount++;
        g_info->items[g_info->todoCount-1] = *new_item;
        free(new_item);
        return 0;
}

int DeleteTodoItem(TodoInfo *g_info, int itemIndex) {
    if(!g_info || itemIndex < 0 || itemIndex >= g_info->todoCount) {
        return -1;
    }
    free(g_info->items[itemIndex].desc);
    g_info->items[itemIndex].desc == NULL;
    free(g_info->items[itemIndex].name);
    g_info->items[itemIndex].name == NULL;
    for(int i = itemIndex;i<g_info->todoCount-1;i++) {
        g_info->items[i] = g_info->items[i+1];
    }
    g_info->todoCount--;
    g_info->items = (TodoItem*)realloc(g_info->items, g_info->todoCount*sizeof(TodoItem));
    return 0;
}

int ModifyTodoItem(TodoInfo *g_info, int itemIndex, const char *name, const char *subtasks, 
        const int *tags,enum Priority priority, time_t startTime, time_t deadline, const char *desc) {
            if(!g_info || itemIndex < 0 || itemIndex >=g_info->todoCount) {
                return -1;
            }
            free(g_info->items[itemIndex].name);
            free(g_info->items[itemIndex].desc);
            g_info->items[itemIndex].name = strdup(name);
            g_info->items[itemIndex].desc = strdup(desc);
            g_info->items[itemIndex].deadline = deadline;
            g_info->items[itemIndex].startTime = startTime;
            g_info->items[itemIndex].priority = priority;
            parseSubtasks(subtasks , g_info->items[itemIndex].subtaskList);
            parseTags(tags , g_info->items[itemIndex].tagList);
            return 0;
        }

int AddTags(TodoInfo *g_info, const int *newTags, int newTagCount) {
    if(g_info == NULL || newTags == NULL || newTagCount <= 0)
        return -1;
    int *newTagList = (int *)malloc((g_info->tagCount + newTagCount)*sizeof(int));
    if(newTagList == NULL)
        return -1;
    for(int i = 0; i<g_info->tagCount; i++) {
        newTagList[i] = g_info->tags[i];
    }
    for(int i = 0; i<newTagCount; i++) {
        newTagList[g_info->tagCount+i] = newTags[i];
    }
    free(g_info->tags);
    g_info->tags = newTagList;
    g_info->tagCount += newTagCount;
    return 0;
}

int DeleteTags(TodoInfo *g_info, const int *toDeleteTags, int toDeleteTagCount) {
    int remainingCount = 0;
    if(g_info == NULL || toDeleteTags == NULL || toDeleteTagCount <= 0)
        return -1;
    int *remainingTags = (int *)malloc(g_info->tagCount*sizeof(int));
    if(remainingTags == NULL)
        return -1;
    for(int i = 0; i<g_info->tagCount; i++) {
        int found = 0;
        for(int j = 0; j<toDeleteTagCount; j++) {
            if(toDeleteTags[j] == g_info->tags[i]) {
                found = 1;
                break;}
        }
        if(!found) {
            remainingTags[remainingCount++] = g_info->tags[i];
        }
    }
    free(g_info->tags);
    g_info->tags = remainingTags;
    g_info->tagCount = remainingCount;
    return 0;
}