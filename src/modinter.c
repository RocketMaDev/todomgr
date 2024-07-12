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


int AddTodoItem(TodoInfo *g_info, const char *name, const char **subtasks, int subtaskCount, const int *tags ,int tagCount,
       enum Priority priority, time_t startTime, time_t deadline, const char *desc) {
        if(!g_info||!name||!desc)
            return -1;
        TodoItem *new_item = (TodoItem*)malloc(sizeof(TodoItem));
        if(!new_item)
            return -1;
        new_item->name = strdup(name);
        new_item->desc = strdup(desc);
        new_item->done = false;
        new_item->tagList = (int *)tags;
        new_item->tagCount+=tagCount ;
        for(int i = 0; i<subtaskCount; i++){
        new_item->subtaskList[i] = strdup((char *)subtasks[i]);
        }
        new_item->subtaskCount = subtaskCount;
        new_item->priority = priority;
        new_item->startTime = startTime;
        new_item->deadline = deadline;
        new_item->desc = (char *)desc;
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

int ModifyTodoItem(TodoInfo *g_info, int itemIndex, const char *name, const char **subtasks, int subtaskCount,
        const int *tags, int tagCount, enum Priority priority, time_t startTime, time_t deadline, const char *desc) {
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
            g_info->items[itemIndex].tagList = (int *)tags;
            g_info->items[itemIndex].tagCount+=tagCount;
            free(g_info->items[itemIndex].subtaskList);
            for(int i = 0; i<subtaskCount; i++){
            g_info->items[itemIndex].subtaskList[i] = strdup((char *)subtasks[i]);
            }
            g_info->items[itemIndex].subtaskCount = subtaskCount;
            return 0;
        }

int AddTag(TodoInfo *g_info, const char *newTag) {
    int newTagCount = g_info->tagCount + 1;
    if(g_info == NULL || newTag == NULL )
        return -1;
    char **newTagList = (char **)malloc(newTagCount*sizeof(char*));
    if(newTagList == NULL)
        return -1;
    for(int i = 0; i<g_info->tagCount; i++) {
        newTagList[i] = g_info->tags[i];
    }
    newTagList[g_info->tagCount + 1] = (char *)newTag;
    free(g_info->tags);
    g_info->tags = newTagList;
    g_info->tagCount = newTagCount;
    return 0;
}

int DeleteTag(TodoInfo *g_info, int toDeleteTag) {
    int remainingCount =  g_info->tagCount - 1;
    if(g_info == NULL || remainingCount <= 0)
        return -1;
    char **remainingTags = (char **)malloc(remainingCount*sizeof(char*));
    if(remainingTags == NULL)
        return -1;
    for(int i = 0; i<toDeleteTag; i++) {
        remainingTags[i] = g_info->tags[i];
    }
    for(int i = toDeleteTag;i<remainingCount; i++) {
        remainingTags[i] = g_info->tags[i+1];
    }
    free(g_info->tags);
    g_info->tags = remainingTags;
    g_info->tagCount = remainingCount;
//重新为tagList分配空间
    for(int i = 0; i<g_info->todoCount; i++) {
    int *tagList = g_info->items[i].tagList;
        for(int j = 0; j<g_info->items[i].tagCount; j++) {
            if(tagList[j] == toDeleteTag) {
                g_info->items[i].tagCount--;
                break;
            }
        }
        for(int j = 0; j<g_info->items[i].tagCount; j++) {
            if(tagList[j] >= toDeleteTag) {
                tagList[j] = tagList[j+1]-1 ;
            }
        }
    }
    return 0;
}
