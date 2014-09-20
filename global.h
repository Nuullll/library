// global.h
#ifndef GLOBAL_H
#define GLOBAL_H

#include <vector>
#include <iomanip>
#include <iostream>
#include "user.h"
#include <cmath>
#include <thread>
#include <chrono>

extern std::vector<User*> users;
extern std::vector<Administrator> admins;
extern std::vector<Reader> readers;
extern std::vector<Book> all_books;     // 与Reader::books_区分

const int UP = 72;
const int DOWN = 80;
const int LEFT = 75;
const int RIGHT = 77;

const int WIDTH = 80;

bool CompareHot(Book b1, Book b2);
bool HighlightPrint(int setw, std::string text, int color);
bool HighlightPrint(std::string text, int color = 9);
bool ValidPassword(std::string pwd);    // 检查是否符合6~15位数字字母密码标准
int Find(std::vector<Book> v, std::string isbn, int index);
int Find(std::vector<User*> v, int id);     // 返回vector中找到的索引, 未找到则返回-1
std::string Decode(int id, std::string cipher); // 解密
std::string GetPass();      // 输入密码
std::string RandomPass(int digits = 6);     // 返回digits位随机密码
std::vector<Book> Find(std::vector<Book> v, std::string isbn);
std::vector<Book> HotBook(int num = 3);    // 借阅排行榜
Token Login();              // 登录, 返回令牌
void ClearScreen();         // 保留标题的清屏函数
void EBook();               // 电子资源界面
void Exit();                // 退出系统
void MediatePrint(std::string text);    // 居中打印字符串
void PrintBooks(std::vector<Book> v);
void Remove(std::vector<Book> &v, std::string isbn, int index);
void Remove(std::vector<Book> &v, std::vector<Book> dels);
void Scroll(std::string text);  // 滚动播放文本
void Title();               // 系统标题
void Wait(int milliseconds = 1000);  // 等待毫秒数
void Welcome();             // 欢迎界面

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