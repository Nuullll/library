// menu.h

#ifndef MENU_H
#define MENU_H 

#include <string>
#include <vector>
#include <utility>

struct MenuNode;

typedef std::pair<std::string, MenuNode*> Option;
// ÿ��ѡ�������ƺ�Ŀ��˵�����.

struct MenuNode
{
    std::vector<Option> options;
    MenuNode* previous;
    void append(Option option);
    void show(int index);   // ѡ�е�ѡ������Ϊindex
};

#endif