#include <algorithm>
#include <cstring>
#include <ctime>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/color.hpp>
#include <map>
#include <string>
#include <vector>

#include "modview.hpp"
#include "modi18n.h"
#include "modfile.h"

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
    if (event == Event::Return && !(state & DETAIL_VIEW_DISPLAY)) {
        state |= DETAIL_VIEW_DISPLAY;
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


void DetailView::reset(TodoItem *itemIn) {
    item = itemIn;
    delete [] checkStates;
    checkStates = new bool[info->tagCount];
    checkboxes = vector<Component>(info->tagCount);
    for (int i = 0; i < info->tagCount; i++)
        checkboxes[i] = Checkbox(info->tags[i], &checkStates[i]);
    checkboxContainer = Container::Vertical(checkboxes);
    if (itemIn) {
        name = itemIn->name;
        char tmp[TIME_STR_MAX];
        struct tm *tmpTime;
        tmpTime = localtime(&itemIn->startTime);
        strftime(tmp, TIME_STR_MAX, TIME_FMT_LONG, tmpTime);
        start = tmp;
        tmpTime = localtime(&itemIn->deadline);
        strftime(tmp, TIME_STR_MAX, TIME_FMT_LONG, tmpTime);
        ddl = tmp;
        desc = itemIn->desc;
    } else {
        name = "";
        start = "";
        ddl = "";
        desc = "";
    }
    container = Container::Vertical({
        Container::Horizontal({
            Container::Vertical({
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
                text(GETTEXT(TIME_FMT_HINT))
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
        checkboxContainer->Render() | vscroll_indicator | yframe | border,
        confirmButton->Render()
    }));
    return window(text(GETTEXT(DETAIL_TITLE)), vbox({
        hbox({
            left,
            middle
        }),
        confirmButton->Render() | border
    }));
}

bool DetailView::OnEvent(Event event) {
    if (event == Event::Escape) {
        state &= ~DETAIL_VIEW_DISPLAY;
        return true;
    }
    return container->OnEvent(event);
}
