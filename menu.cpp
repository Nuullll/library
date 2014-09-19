// menu.cpp

#include "menu.h"
#include "global.h"

void MenuNode::append(Option option)
{
    options.push_back(option);
}

void MenuNode::show(int index)
{
    for (int i = 0; i < options.size(); ++i)
    {
        if (i == index)
        {
            HighlightPrint(options[i].first + '\n');
            continue;
        }
        MediatePrint(options[i].first + '\n');
    }
}