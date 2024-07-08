#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <errno.h>
#include "modfile.h"

static char *tagbuf;
// 初始化 TodoInfo 结构
TodoInfo *InitTodoInfo(void) {
    TodoInfo *info = (TodoInfo *)malloc(sizeof(TodoInfo));
    if (info == NULL) {
        return NULL;
    }
    info->lang = false;
    info->sortType = SMART;

    info->tagCount = 0;
    info->tags = NULL;

    info->todoCount = 0;
    info->items = NULL;

    return info;
}

// 读取 Todo 文件
int ReadTodoFile(TodoInfo *g_info, const char *filepath) {
    // 打开文件
    FILE *file = fopen(filepath, "rb");
    if (file == NULL) {
        return errno;
    }
    unsigned char *buf = malloc(BUFSIZE);
    memset(buf, 0, BUFSIZE);
    fread(buf, 1, BUFSIZE, file);

    // resolve file header
    TodoFile fTodo;
    fTodo.tagPos = buf[0];
    fTodo.lang = buf[1];
    fTodo.sortType = buf[2];

    // resolve tags
    char *cursor = buf + fTodo.tagPos;
    fTodo.tagcnt = *(unsigned short *)cursor;
    cursor += sizeof(unsigned short);
    char *tagend = cursor; // tag where tags ends
    char **tagitem = malloc(sizeof(char *) * fTodo.tagcnt);
    for (int i = 0; i < fTodo.tagcnt; i++) {
        tagitem[i] = tagend;
        while (*(tagend++) != '\0');
    }
    tagbuf = malloc(tagend - cursor);
    memcpy(tagbuf, cursor, tagend - cursor);
    for (int i = 0; i < fTodo.todocnt; i++)
        tagitem[i] = tagitem[i] - buf + tagbuf;
    g_info->tags = tagitem;
    cursor = tagend;

    
    // 读取文件内容并解析
    // （此处需要根据具体的文件格式进行解析和数据存储）
    
    fclose(file);
    return 0;
}

// 写入 Todo 文件
int WriteTodoFile(TodoInfo *g_info, const char *filepath) {
    // 打开文件
    FILE *file = fopen(filepath, "wb");
    if (file == NULL) {
        return errno;
    }

    // 将数据写入文件
    // （此处需要根据具体的需求将数据格式化为文本并写入文件）
     
    int numInfos;
    printf("请输入要写入的 TodoInfo 数量: ");
    scanf("%d", &numInfos);

    TodoInfo *infos = (TodoInfo *)malloc(numInfos * sizeof(TodoInfo));

    for (int i = 0; i < numInfos; i++) {
        printf("请输入第 %d 个 TodoInfo 的 lang 值（0 或 1）: ", i + 1);
        scanf("%d", &infos[i].lang);

        printf("请输入第 %d 个 TodoInfo 的 sortType（0 - 5）: ", i + 1);
        int sortTypeInput;
        scanf("%d", &sortTypeInput);
        infos[i].sortType = (enum SortType)sortTypeInput;

        printf("请输入第 %d 个 TodoInfo 的 tagCount: ", i + 1);
        scanf("%d", &infos[i].tagCount);

        infos[i].tags = (char **)malloc(infos[i].tagCount * sizeof(char *));
        for (int j = 0; j < infos[i].tagCount; j++) {
            printf("请输入第 %d 个 tag: ", j + 1);
            char tag[100];
            scanf("%s", tag);
            infos[i].tags[j] = (char *)malloc(strlen(tag) + 1);
            strcpy(infos[i].tags[j], tag);
        }

        printf("请输入第 %d 个 TodoInfo 的 todoCount: ", i + 1);
        scanf("%d", &infos[i].todoCount);

        infos[i].items = (TodoItem *)malloc(infos[i].todoCount * sizeof(TodoItem));
        for (int j = 0; j < infos[i].todoCount; j++) {
            printf("请输入第 %d 个 TodoItem 的 name: ", j + 1);
            char name[100];
            scanf("%s", name);
            infos[i].items[j].name = (char *)malloc(strlen(name) + 1);
            strcpy(infos[i].items[j].name, name);

            printf("请输入第 %d 个 TodoItem 的 subtaskCount: ", j + 1);
            scanf("%d", &infos[i].items[j].subtaskCount);

            infos[i].items[j].subtaskList = (char **)malloc(infos[i].items[j].subtaskCount * sizeof(char *));
            for (int k = 0; k < infos[i].items[j].subtaskCount; k++) {
                printf("请输入第 %d 个 subtask: ", k + 1);
                char subtask[100];
                scanf("%s", subtask);
                infos[i].items[j].subtaskList[k] = (char *)malloc(strlen(subtask) + 1);
                strcpy(infos[i].items[j].subtaskList[k], subtask);
            }

            printf("请输入第 %d 个 TodoItem 的 tagCount: ", j + 1);
            scanf("%d", &infos[i].items[j].tagCount);

            infos[i].items[j].tagList = (int *)malloc(infos[i].items[j].tagCount * sizeof(int));
            for (int k = 0; k < infos[i].items[j].tagCount; k++) {
                printf("请输入第 %d 个 tag 值: ", k + 1);
                scanf("%d", &infos[i].items[j].tagList[k]);
            }

            printf("请输入第 %d 个 TodoItem 的 done 值（0 或 1）: ", j + 1);
            scanf("%d", &infos[i].items[j].done);

            printf("请输入第 %d 个 TodoItem 的 priority（0 - 3）: ", j + 1);
            int priorityInput;
            scanf("%d", &priorityInput);
            infos[i].items[j].priority = (enum Priority)priorityInput;

            printf("请输入第 %d 个 TodoItem 的 startTime（格式：YYYY-MM-DD HH:MM:SS）: ", j + 1);
            char startTimeStr[20];
            scanf("%s", startTimeStr);
            struct tm tm;
            strptime(startTimeStr, "%d-%d-%d %d:%d:%d", &tm);
            infos[i].items[j].startTime = mktime(&tm);

            printf("请输入第 %d 个 TodoItem 的 deadline（格式：YYYY-MM-DD HH:MM:SS）: ", j + 1);
            char deadlineStr[20];
            scanf("%s", deadlineStr);
            strptime(deadlineStr, "%d-%d-%d %d:%d:%d", &tm);
            infos[i].items[j].deadline = mktime(&tm);

            printf("请输入第 %d 个 TodoItem 的 desc: ", j + 1);
            char desc[100];
            scanf("%s", desc);
            infos[i].items[j].desc = (char *)malloc(strlen(desc) + 1);
            strcpy(infos[i].items[j].desc, desc);
        }

        // 将结构体写入文件
        fwrite(&infos[i], sizeof(TodoInfo), 1, file);
    }

    fclose(file);
return 0;
   }


// 释放 TodoInfo 占用的内存
void ReleaseTodoInfo(TodoInfo *g_info) {
    if (g_info == NULL) {
        return;
    }

    if (g_info->tags) {
        for (int i = 0; i < g_info->tagCount; i++) {
            free(g_info->tags[i]);
        }
        free(g_info->tags);
    }

    if (g_info->items) {
        for (int i = 0; i < g_info->todoCount; i++) {
            TodoItem *item = &g_info->items[i];
            free(item->name);
            if (item->subtaskList) {
                for (int j = 0; j < item->subtaskCount; j++) {
                    free(item->subtaskList[j]);
                }
                free(item->subtaskList);
            }
            if (item->desc) {
                free(item->desc);
            }
        }
        free(g_info->items);
    }

    free(g_info);
}