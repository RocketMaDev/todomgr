#include <algorithm>
#include <cstring>
#include <ctime>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/color.hpp>
#include <locale>
#include <map>
#include <string>
#include <vector>

#include "modview.hpp"
#include "modi18n.h"
#include "modfile.h"
#include "modinter.h"

using namespace ftxui;
using namespace std;

int state = 0;
static int thisyear;
static constexpr int TIME_STR_MAX = 16;
static inline void mktimestr(char *dest, time_t cmp) {
    time_t timerepo = cmp;
    struct tm *timetm;
    if (cmp == 0) {
        *dest = '\0';
        return;
    }
    timetm = localtime(&timerepo);
    if (timetm->tm_year == thisyear)
        strftime(dest, TIME_STR_MAX, TIME_FMT_SHORT, timetm);
    else
        strftime(dest, TIME_STR_MAX, TIME_FMT_LONG, timetm);
}

//////////////////////////////////////////////////////////////////////
//                          MainView Part                           //
//////////////////////////////////////////////////////////////////////
Element MainView::Render(void) {
    int namelen = strlen(GETTEXT(NAME_HEADER)), 
        subtasklen = strlen(GETTEXT(SUBTASK_HEADER)), 
        startLen = strlen(GETTEXT(START_TIME_HEADER)), 
        ddlLen = strlen(GETTEXT(DEADLINE_HEADER)), 
        descLen = strlen(GETTEXT(DESCRIPTION_HEADER)), 
        tagLen = strlen(GETTEXT(TAG_HEADER));

    // calc time str
    vector<string> startList, ddlList;
    time_t nowtime = time(NULL);
    thisyear = localtime(&nowtime)->tm_year;
    for (int i = 0; i < info->todoCount; i++) {
        char timestr[TIME_STR_MAX];
        mktimestr(timestr, info->items[i].startTime);
        startList.push_back(timestr);
        mktimestr(timestr, info->items[i].deadline);
        ddlList.push_back(timestr);
    }

    // concat tags and subtask
    vector<string> tagList, subtaskList;
    for (int i = 0; i < info->todoCount; i++) {
        if (info->items[i].subtaskCount == 0)
            subtaskList.push_back("");
        else if (info->items[i].subtaskCount == 1)
            subtaskList.push_back(info->items[i].subtaskList[0]);
        else {
            string str = info->items[i].subtaskList[0];
            str.append(" ...");
            subtaskList.push_back(str);
        }
        if (info->items[i].tagCount == 0)
            tagList.push_back("");
        else if (info->items[i].tagCount == 1)
            tagList.push_back(info->tags[info->items[i].tagList[0]]);
        else {
            string str = info->tags[info->items[i].tagList[0]];
            str.append(" ...");
            tagList.push_back(str);
        }
    }

    // calc widget width
    for (int i = 0; i < info->todoCount; i++) {
        namelen = max(namelen, int(strlen(info->items[i].name)));
        subtasklen = max(subtasklen, int(subtaskList[i].size()));
        tagLen = max(tagLen, int(tagList[i].size()));
        startLen = max(startLen, int(startList[i].size()));
        ddlLen = max(ddlLen, int(ddlList[i].size()));
        descLen = max(descLen, int(strlen(info->items[i].desc)));
    }
    descLen = min(descLen, 40); // limit desc len in case it exceeds border

    auto header = hbox({
        text(" ") | size(WIDTH, EQUAL, 1), // checkbox for done state
        text(GETTEXT(NAME_HEADER)) | flex,
        separator(),
        text(GETTEXT(SUBTASK_HEADER)) | size(WIDTH, EQUAL, subtasklen),
        separator(),
        text(GETTEXT(TAG_HEADER)) | size(WIDTH, EQUAL, tagLen),
        separator(),
        text(GETTEXT(START_TIME_HEADER)) | size(WIDTH, EQUAL, startLen),
        separator(),
        text(GETTEXT(DEADLINE_HEADER)) | size(WIDTH, EQUAL, ddlLen),
        separator(),
        text(GETTEXT(DESCRIPTION_HEADER)) | size(WIDTH, EQUAL, descLen)
    });

    static map<Priority, Decorator> logStyle = {
        {CRITICAL, color(Color::Red)},
        {IMPORTANT, color(Color::Yellow)},
        {NORMAL, color(Color::Blue)},
        {ORDINARY, color(Color::White)}
    };

    Elements list;
    for (int i = 0; i < info->todoCount; i++) {
        bool is_focus = i == selected;
        Decorator nameDecorator = logStyle[info->items[i].priority];

        Decorator lineDecorator = nothing;
        if (is_focus) {
            lineDecorator = lineDecorator | focus | bold;
            if (Focused())
                lineDecorator = lineDecorator | inverted;
        }
        if (!info->items[i].done)
            lineDecorator = lineDecorator;

        Element document = hbox({
            Checkbox("", &info->items[i].done)->Render(),
            text(info->items[i].name) | flex | nameDecorator,
            separator(),
            text(subtaskList[i]) | size(WIDTH, EQUAL, subtasklen) | notflex | nameDecorator,
            separator(),
            text(tagList[i]) | size(WIDTH, EQUAL, tagLen) | notflex,
            separator(),
            text(startList[i]) | size(WIDTH, EQUAL, startLen) | notflex,
            separator(),
            text(ddlList[i]) | size(WIDTH, EQUAL, ddlLen) | notflex,
            separator(),
            text(info->items[i].desc) | size(WIDTH, EQUAL, descLen) | notflex
        }) | flex | lineDecorator;
        list.push_back(document);
    }

    if (list.empty())
        list.push_back(text(GETTEXT(NO_ITEM_PROMPT)));

    length = info->todoCount;

    auto hintBar = hbox({
        text(" E ") | color(Color::Blue),
        text(GETTEXT(EDIT_SETTING_HINT)),
        separator(),
        text(" A ") | color(Color::Blue),
        text(GETTEXT(ADD_TASK_HINT)),
        separator(),
        text(" D ") | color(Color::Blue),
        text(GETTEXT(DELETE_TASK_HINT)),
        separator(),
        text(" V ") | color(Color::Blue),
        text(GETTEXT(VIEW_TAG_HINT)),
        separator(),
        text(" W ") | color(Color::Blue),
        text(GETTEXT(SAVE_HINT)),
        separator(),
        text(" [SPACE] ") | color(Color::Blue),
        text(GETTEXT(SWITCH_STATE_HINT)),
        separator(),
        text(" [ENTER] ") | color(Color::Blue),
        text(GETTEXT(VIEW_DETAIL_HINT)),
        separator(),
        text(" [ESC] ") | color(Color::Blue),
        text(GETTEXT(EXIT_HINT))
    }) | hcenter | notflex;
    return vbox({
        window(text(GETTEXT(MAIN_TITLE)), vbox({
            header,
            separator(),
            vbox(list) | yframe,
        })),
        window(text(GETTEXT(HINT_TITLE)), hintBar)
    });
}

bool MainView::OnEvent(Event event) {
    if (event == Event::Escape && !(state & EXIT_DISPLAY)) {
        state |= EXIT_DISPLAY;
        return true;
    }
    if (event == Event::Return && (state & SAVE_DONE_DISPLAY)) {
        state &= ~SAVE_DONE_DISPLAY;
        return true;
    }
    if (event == Event::Return && !(state & DETAIL_VIEW_DISPLAY) && info->todoCount) {
        state |= DETAIL_VIEW_DISPLAY;
        itemHandle = info->items + selected;
        return true;
    }
    if (event == Event::Character('e') && !(state & SETTING_DISPLAY)) {
        state |= SETTING_DISPLAY;
        return true;
    }
    if (event == Event::Character('a') && !(state & DETAIL_VIEW_DISPLAY)) {
        state |= DETAIL_VIEW_DISPLAY;
        itemHandle = nullptr;
        return true;
    }
    if (event == Event::Character('v') && !(state & TAG_VIEW_DISPLAY)) {
        state |= TAG_VIEW_DISPLAY;
        return true;
    }
    if (event == Event::Character(' ') && info->todoCount) {
        if (info->items[selected].done)
            MarkUndone(info, selected);
        else
            MarkDone(info, selected);
        return true;
    }
    if ((event == Event::Delete || event == Event::Character('d')) && info->todoCount) {
        DeleteTodoItem(info, selected);
        return true;
    }
    if (event == Event::Character('w')) {
        WriteTodoFile(info, "tododb");
        state |= SAVE_DONE_DISPLAY;
        return true;
    }
    int old_selected = selected;
    if (event == Event::ArrowUp || event == Event::Character('k'))
        selected--;
    if (event == Event::ArrowDown || event == Event::Character('j'))
        selected++;

    selected = std::max(0, std::min(length - 1, selected));

    if (selected != old_selected) {
        return true;
    }

    return false;
}

//////////////////////////////////////////////////////////////////////
//                         DetailView Part                          //
//////////////////////////////////////////////////////////////////////
static ftxui::InputOption OneLine(void) {
    ftxui::InputOption base = ftxui::InputOption::Default();
    base.multiline = false; // multiline causes SIGSEGV, kinda weird
    return base;
}

DetailView::DetailView(TodoInfo *g_info): info(g_info), checkStates(nullptr), 
        nameInput(ftxui::Input(&name, OneLine())),
        subtaskInput(ftxui::Input(&subtask, OneLine())),
        startInput(ftxui::Input(&start, OneLine())),
        ddlInput(ftxui::Input(&ddl, OneLine())),
        descInput(ftxui::Input(&desc, OneLine())),
        checkboxContainer(ftxui::Container::Vertical({})) {
    priorityStrings = {
        GETTEXT(CRITIAL_PRIORITY),
        GETTEXT(IMPORTANT_PRIORITY),
        GETTEXT(NORMAL_PRIORITY),
        GETTEXT(ORDINARY_PRIORITY)
    };
    priorityToggle = ftxui::Toggle(&priorityStrings, &priority);
    confirmButton = ftxui::Button(GETTEXT(OK), [&] { // NOTE one lambda here
        struct tm tmp = {};
        time_t startTime;
        time_t deadline;
        if (start.empty())
            startTime = 0;
        else if (start == "1970-01-01")
            startTime = 0;
        else {
            // time str to time_t; no error handling due to lack of time
            std::stringstream startStream(start);
            startStream >> std::get_time(&tmp, TIME_FMT_LONG);
            startTime = mktime(&tmp);
        }
        if (ddl.empty())
            deadline = 0;
        else if (ddl == "1970-01-01")
            deadline = 0;
        else {
            std::stringstream ddlStream(ddl);
            ddlStream >> std::get_time(&tmp, TIME_FMT_LONG);
            deadline = mktime(&tmp);
        };

        state &= ~DETAIL_VIEW_DISPLAY;
        std::vector<int> tagList;
        for (int i = 0; i < info->tagCount; i++)
                if (checkStates[i])
                tagList.push_back(i);

        // concat subtaskList into one string
        std::vector<std::string> subtaskList;
        std::string token;
        std::stringstream stream(subtask);
        while (std::getline(stream, token, '|'))
            subtaskList.push_back(token);
        const char **subtasks = new const char *[subtaskList.size()];
        for (int i = 0; i < subtaskList.size(); i++)
            subtasks[i] = subtaskList[i].c_str();

        if (!item)
            AddTodoItem(info, name.c_str(), subtasks, subtaskList.size(),
                    tagList.data(), tagList.size(), (enum Priority)priority, startTime, deadline, desc.c_str());
        else
            ModifyTodoItem(info, item - info->items, name.c_str(), subtasks, subtaskList.size(),
                    tagList.data(), tagList.size(), (enum Priority)priority, startTime, deadline, desc.c_str());
    });
    container = ftxui::Container::Vertical({
        ftxui::Container::Horizontal({
            ftxui::Container::Vertical({
                nameInput,
                subtaskInput,
                priorityToggle,
                startInput,
                ddlInput,
                descInput
            }),
            checkboxContainer,
        }),
        confirmButton
    });
}

void DetailView::reset(TodoItem *itemIn) {
    item = itemIn;
    delete [] checkStates;                  // as we don't know how long the array will be, 
    checkStates = new bool[info->tagCount]; // so we need to free it and realloc
    memset(checkStates, 0, info->tagCount * sizeof(bool)); // clear bool field so it can be switched
    checkboxes = vector<Component>(info->tagCount);
    checkboxContainer->DetachAllChildren();
    for (int i = 0; i < info->tagCount; i++) {
        checkboxes[i] = Checkbox(info->tags[i], &checkStates[i]);
        checkboxContainer->Add(checkboxes[i]);
    }
    if (itemIn) {
        name.assign(itemIn->name); // use assign to prevent reference alternate

        // tokenize subtask field with delimiter |
        string mergeOfSubtask;
        for (int i = 0; i < itemIn->subtaskCount - 1; i++) {
            mergeOfSubtask += itemIn->subtaskList[i];
            mergeOfSubtask += "|";
        }
        if (itemIn->subtaskCount > 0)
            mergeOfSubtask += itemIn->subtaskList[itemIn->subtaskCount - 1];
        subtask.assign(mergeOfSubtask);

        for (int i = 0; i < itemIn->tagCount; i++)
            checkStates[itemIn->tagList[i]] = true;

        char tmp[TIME_STR_MAX];
        struct tm *tmpTime;
        tmpTime = localtime(&itemIn->startTime);
        strftime(tmp, TIME_STR_MAX, TIME_FMT_LONG, tmpTime);
        start.assign(tmp);
        tmpTime = localtime(&itemIn->deadline);
        strftime(tmp, TIME_STR_MAX, TIME_FMT_LONG, tmpTime);
        ddl.assign(tmp);

        priority = itemIn->priority;
        desc.assign(itemIn->desc);
    } else {
        name.clear();
        subtask.clear();
        start.clear();
        ddl.clear();
        desc.clear();
    }
}

Element DetailView::Render(void) {
    auto left = vbox({
        hbox({
            vbox({
                text(GETTEXT(NAME_INPUT)),
                text(GETTEXT(SUBTASK_INPUT)),
                text(GETTEXT(PRIORITY_INPUT)),
                text(GETTEXT(START_TIME_INPUT)),
                text(GETTEXT(DEADLINE_INPUT)),
                text(GETTEXT(DESCRIPTION_INPUT)),
                text(GETTEXT(TIME_FMT_HINT)) | color(Color::GrayDark),
                text(GETTEXT(SUBTASK_HINT)) | color(Color::GrayDark)
            }),
            vbox({
                nameInput->Render(),
                subtaskInput->Render(),
                priorityToggle->Render(),
                startInput->Render(),
                ddlInput->Render(),
                descInput->Render(),
            })
        })
    });

    auto middle = window(text(GETTEXT(TAG_HEADER)), vbox({
        checkboxContainer->Render() | vscroll_indicator | yframe
    }));
    return window(text(GETTEXT(DETAIL_TITLE)), vbox({
        hbox({
            left,
            middle
        }),
        confirmButton->Render() | hcenter | notflex
    })) | center;
}

bool DetailView::OnEvent(Event event) {
    if (event == Event::Escape) {
        state &= ~DETAIL_VIEW_DISPLAY;
        return true;
    }
    return container->OnEvent(event);
}

//////////////////////////////////////////////////////////////////////
//                           TagView Part                           //
//////////////////////////////////////////////////////////////////////
TagView::TagView(TodoInfo *g_info): info(g_info), selected(0),
        newInput(Input(&newTag, GETTEXT(NEW_TAG_PROMPT), OneLine())){
    for (int i = 0; i < info->tagCount; i++)
        tags.push_back(info->tags[i]);
}

Element TagView::Render(void) {
    Elements list;
    for (auto &it : tags) {
        list.push_back(text(it));
    }
    if (list.empty()) {
        list.push_back(text(GETTEXT(NO_TAG_PROMPT)));
    }
    newInput->TakeFocus();
    return window(text(GETTEXT(TAG_HEADER)), vbox({
        vbox(list) | yframe,
        separator(),
        newInput->Render(),
        text(GETTEXT(DELETE_TAG_PROMPT)) | color(Color::Blue) | hcenter
    })) | center;
}

bool TagView::OnEvent(Event event) {
    if (event == Event::Escape && (state & TAG_VIEW_DISPLAY)) {
        state &= ~TAG_VIEW_DISPLAY;
        return true;
    }
    if (event == Event::Return && (state & TAG_VIEW_DISPLAY)) {
        if (newTag.empty())
            return true;
        int idx = 0;
        for (auto &it : tags) {
            if (it == newTag) {
                tags.remove(newTag);
                DeleteTag(info, idx);
                return true;
            }
            idx++;
        }
        tags.push_back(newTag);
        AddTag(info, newTag.c_str());
        return true;
    }
    return newInput->OnEvent(event);
}
