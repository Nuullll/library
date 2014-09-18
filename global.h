// global.h
#ifndef GLOBAL_H
#define GLOBAL_H

#include <vector>
#include "user.h"
#include <cmath>

extern std::vector<User*> users;
extern std::vector<Administrator> admins;
extern std::vector<Reader> readers;
extern std::vector<Book> all_books;     // 与Reader::books_区分

const int UP = 72;
const int DOWN = 80;
const int LEFT = 75;
const int RIGHT = 77;

int GetX();         // 获取光标x坐标
int GetY();         // 获取光标y坐标
void GotoXY(int x, int y);  // 将光标移至(x,y)位置
std::string GetPass();      // 输入密码
std::string Decode(int id, std::string cipher); // 解密
bool ValidPassword(std::string pwd);    // 检查是否符合6~15位数字字母密码标准
Token Login();              // 登录, 返回令牌
void Logout();
bool HighlightPrint(char* s_format, std::string text, int color = 9);
void ClearScreen();         // 保留标题的清屏函数
void Wait(int seconds = 1); // 暂停seconds
void Scroll(std::string text);  // 滚动播放文本
void Welcome();             // 欢迎界面
void Exit();                // 退出系统
void Title();               // 系统标题
int Find(std::vector<User*> v, int id);     // 返回vector中找到的索引, 未找到则返回-1
int Find(std::vector<Book> v, std::string isbn, int index);
void Remove(std::vector<Book> &v, std::string isbn, int index);
std::vector<Book> HotBook(int num);    // 借阅排行榜

template <class T>
std::string Encode(T user)      // 加密
{
    int id = user.id_;
    std::string clear = user.password_;
    std::vector<int> shifts;
    std::string cipher;
    for (int i = 0; id >= 10; ++i)
    {
        shifts.push_back(((i % 2) ? -1 : 1) * (id % 10));
        id /= 10;
    }
    int i = 0;
    while (clear.size())
    {
        cipher.push_back(clear.back() + shifts[i]);
        clear.pop_back();
        if (++i > shifts.size() - 1)
            i = 0;
    }
    return cipher;
}

template <class A, class B>
int Find(std::vector<A> v, B id)
{
    for (int i = 0; i < v.size(); ++i)
    {
        if (v[i] == id)
            return i;
    }
    return -1;
}

template <class A, class B>
void Remove(std::vector<A> &v, B id)
{
    for (std::vector<A>::iterator it = v.begin(); it != v.end(); ++it)
    {
        if (*it == id)
        {
            v.erase(it);
            break;
        }
    }
}

#endif