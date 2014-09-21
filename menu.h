// menu.h

#ifndef MENU_H
#define MENU_H 

#include <string>
#include <vector>
#include <utility>

struct MenuNode;

typedef std::pair<std::string, MenuNode*> Option;
// 每个选项由名称和目标菜单构成.

struct MenuNode
{
    std::vector<Option> options;
    MenuNode* previous;
    void append(Option option);
    void show(int index);   // 选中的选项索引为index
};

#endif