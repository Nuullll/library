// global.cpp

#include "global.h"

#include <stdlib.h>
#include <windows.h>
#include <algorithm>
#include <iomanip>

bool CompareHot(Book b1, Book b2)
{
    return (b1.info().size() > b2.info().size());
}

bool HighlightPrint(int setw, std::string text, int color = 9)
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    if (handle == 0)
        return false;
    BOOL ret = SetConsoleTextAttribute(handle, WORD(color));
    std::cout << std::setw(setw) << text;
    ret = SetConsoleTextAttribute(handle, WORD(7));
    return (ret == true);
}

bool HighlightPrint(std::string text, int color = 9) 
{
    return HighlightPrint((WIDTH + text.size()) / 2, text, color);
}

bool ValidPassword(std::string pwd)
{
    return ((pwd.size() >= 6) && (pwd.size() <= 15));
}

int Find(std::vector<Book> v, std::string isbn, int index)
{
    for (int i = 0; i < v.size(); ++i)
    {
        if ((v[i].isbn() == isbn) && (v[i].index() == index))
            return i;
    }
    return -1;
}

int Find(std::vector<User*> v, int id)
{
    for (int i = 0; i < v.size(); ++i)
    {
        if (v[i]->id() == id)
            return i;
    }
    return -1;
}

std::string Decode(int id, std::string cipher)
{
    std::vector<int> shifts;
    for (int i = 0; id >= 10; ++i)
    {
        shifts.push_back(((i % 2) ? 1 : -1) * (id % 10));
        id /= 10;
    }
    std::string clear;
    int i = 0;
    while (cipher.size())
    {
        clear.insert(clear.begin(), cipher.front() + shifts[i]);
        cipher.erase(cipher.begin());
        if (++i > shifts.size() - 1)
            i = 0;
    }
    return clear;
}

std::string GetPass()
{
    int c;
    std::string password;
    std::string::iterator it = password.end();
    while ((c = getch()) != '\r')
    {
        switch (c)
        {
        case 0: 
            {
                getch();
                std::cout << '\a';
                break;
            }
        case 224: 
            {
                switch (getch())
                {
                case 75: // left
                    {
                        if (it != password.begin())
                        {
                            --it;
                            std::cout << '\b';
                        }
                        else 
                            std::cout << '\a';
                        break;
                    }
                case 77: // right
                    {
                        if (it != password.end())
                        {
                            ++it;
                            std::cout << '*';
                        }
                        else
                            std::cout << '\a';
                        break;
                    }
                default: 
                    {
                        std::cout << '\a';
                        break;
                    }
                }
                break;
            }
        case 163: 
            {
                getch();
                std::cout << '\a';
                break;
            }
        case '\b':
            {
                if (password.size() != 0 && it != password.begin())
                {
                    std::cout << '\b';
                    for (int i = 0; i < password.end() - it; ++i)
                        std::cout << '*';
                    std::cout << ' ';
                    for (int i = 0; i < password.end() - it + 1; ++i)
                        std::cout << '\b';
                    password.erase(it - 1);
                    --it;
                }
                else
                    std::cout << '\a';
                break;
            }
        default:
            {
                if (isalnum(c) || ispunct(c))
                {
                    password.insert(it, c);
                    ++it;
                    for (int i = 0; i < password.end() - it + 1; ++i)
                        std::cout << '*';
                    for (int i = 0; i < password.end() - it; ++i)
                        std::cout << '\b';
                }
                else
                    std::cout << '\a';
                break;
            }
        }
    }
    std::cout << std::endl;
    return password;
}

std::vector<Book> HotBook(int num)
{
    std::sort(all_books.begin(), all_books.end(), CompareHot);
    return std::vector<Book>(all_books.begin(), all_books.begin() + num + 1);
}

Token Login()
{
    printf("%44s\n", "请登录...");
    int id;
    std::string pwd;
    printf("%51s", "请输入您的学号(工号): ");
    std::cin >> id;
    printf("%48s", "请输入您的密码: ");
    pwd = GetPass();
    return Token(id, pwd);
}

void ClearScreen()
{
    system("cls");
    Title();
}

void EBook()
{
    HighlightPrint("请登录 http://lib.tsinghua.edu.cn 访问查阅电子资源库!");
}

void Exit()
{
    ClearScreen();
    MediatePrint("确认退出? [y/n] \n");
    char ch = getch();
    if (ch == 'y' || ch == 'Y')
    {
        MediatePrint("再见! \n");
        exit(1);
    }
}

void MediatePrint(std::string text)
{
    std::cout << std::setw((WIDTH + text.size()) / 2) << text;
}

void Remove(std::vector<Book> &v, std::string isbn, int index)
{
    for (std::vector<Book>::iterator it = v.begin(); it != v.end(); ++it)
    {
        if ((it->isbn() == isbn) && (it->index() == index))
        {
            v.erase(it);
            break;
        }
    }
}

void Title()
{
    HighlightPrint("图书管理系统 v1.60.1");
}

void Wait(int milliseconds)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

void Welcome()
{
    
}