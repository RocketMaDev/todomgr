#include "modinter.h"

void MarkDone(TodoInfo *g_info, int itemIndex) {
    g_info->items[itemIndex].done = true;
}
void MarkUndone(TodoInfo *g_info, int itemIndex){
    g_info->items[itemIndex].done = false;
}