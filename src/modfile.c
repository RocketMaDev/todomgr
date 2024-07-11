#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <errno.h>
#include "modfile.h"

static char *tagbuf = NULL;
static int tagbuf_len = 0;
bool fileIsOpened = false;
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

int ReadTodoFile(TodoInfo *g_info, const char *filepath) {
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
    g_info->lang = fTodo.lang;
    g_info->sortType = fTodo.sortType;

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
    tagbuf_len = tagend - cursor;
    tagbuf = malloc(tagbuf_len);
    memcpy(tagbuf, cursor, tagbuf_len);
    for (int i = 0; i < fTodo.todocnt; i++)
        tagitem[i] = tagitem[i] - (char *)buf + tagbuf;
    g_info->tags = tagitem;
    g_info->tagCount = fTodo.tagcnt;
    cursor = tagend;

    // read Item
    fTodo.todocnt=*(int *)cursor;
    g_info->items =malloc(sizeof(TodoItem)*fTodo.todocnt);
    for (int i = 0; i < fTodo.todocnt; i++) {
        g_info->items[i].name = strdup(cursor);
        cursor += strlen(cursor) + 1;
        g_info->items[i].subtaskCount = *(unsigned short *)cursor;
        cursor += sizeof(unsigned char);
        g_info->items[i].subtaskList = malloc(sizeof(char *) * g_info->items[i].subtaskCount);
        for (int j = 0; j < g_info->items[i].subtaskCount; j++) {
           g_info->items[i].subtaskList[j] = strdup(cursor);
           cursor += strlen(cursor) + 1;
        }
        int tagCount = *(unsigned short *)cursor;
        cursor += sizeof(unsigned short);
        g_info->items[i].tagCount = tagCount;
        g_info->items[i].tagList = malloc(sizeof(unsigned short) * tagCount);
        for (int j = 1; j <= tagCount; j++) {
            g_info->items[i].tagList[j] = *(unsigned short *)cursor;
            cursor += sizeof(unsigned short);
        }
        *(&g_info->items[i].done) = *(unsigned char *)cursor;
        cursor += sizeof(unsigned char);

        g_info->items[i].startTime = *(time_t *)cursor;
        cursor += sizeof(time_t);
        g_info->items[i].deadline = *(time_t *)cursor;
        cursor += sizeof(time_t);

        g_info->items[i].desc = strdup(cursor);
        cursor += strlen(cursor) + 1;
    }

    fclose(file);
    return 0;
}


int WriteTodoFile(TodoInfo *g_info, const char *filepath) {
    FILE *file = fopen(filepath, "wb");
    if (file == NULL) {
        return errno;
    }
    unsigned char *buf=malloc(BUFSIZE);
    memset(buf,0,BUFSIZE);
    buf[0] = 3;
    buf[1] = g_info->lang;
    buf[2] = g_info->sortType;
    char *cursor=buf+buf[0];

    *(unsigned short*) cursor = g_info->tagCount;
    cursor+=sizeof(unsigned short);
    for(int i = 0; i< g_info->tagCount;i++){
       strcpy(cursor, g_info->tags[i]);
       cursor+=strlen(cursor) + 1;
    }
    *(int *)cursor = g_info->todoCount;
    cursor+=sizeof(int);
    for (int i = 0; i < g_info->todoCount; i++) {
        strcpy(cursor, g_info->items[i].name);
        cursor+=strlen(cursor) + 1;
        *(unsigned char*)cursor = g_info->items[i].subtaskCount;
        cursor += sizeof(unsigned char);
        int subcnt = g_info->items[i].subtaskCount;
        for (int j = 0; j < subcnt; j++){
            strcpy(cursor, g_info->items[i].subtaskList[j]);
            cursor+=strlen(cursor) + 1;
        }
        *(unsigned short*) cursor = g_info->items[i].tagCount;
        cursor += sizeof(short);
        for (int j = 0; j < g_info->items[i].tagCount ; j++)
        {
           *(unsigned short*) cursor = g_info->items[i].tagList[j];
            cursor += sizeof(short);
        }

        *(unsigned char *)cursor = g_info->items[i].done | (g_info->items[i].priority << 1);
        cursor+=sizeof(unsigned char);
        *(time_t*)cursor = g_info->items[i].startTime;
        cursor += sizeof(time_t);
        *(time_t*)cursor = g_info->items[i].deadline;
        cursor += sizeof(time_t);
        strcpy(cursor, g_info->items[i].desc);
        cursor+=strlen(cursor) + 1;
    }
    
    fwrite(buf,1,BUFSIZE,file);
    fclose(file);
    free(buf);
    return 0;
}


// 释放 TodoInfo 占用的内存
void ReleaseTodoInfo(TodoInfo *g_info) {
    if (g_info == NULL) {
        return;
    }
    for (int i = 0; i < g_info->tagCount; i++)
    {
     if ( !(g_info->tags[i]>=tagbuf&&g_info->tags[i]<tagbuf+tagbuf_len)) 
         free(g_info->tags[i]);
    }
    if (tagbuf) {
        free(tagbuf);
        tagbuf = NULL;
    }

    for (int i = 0; i < g_info->todoCount; i++) {
        if (g_info->items[i].name) {
            free(g_info->items[i].name);
        }

        for (int j = 0; j < g_info->items[i].subtaskCount; j++) {
            if (g_info->items[i].subtaskList[j]) {
                free(g_info->items[i].subtaskList[j]);
            }
        }
        if (g_info->items[i].tagList) {
            free(g_info->items[i].tagList);
        }
            }
    if (g_info->items) {
        free(g_info->items);
        g_info->items = NULL;
    }
    free(g_info);
    
}

