// global.cpp

#include "global.h"

#include <stdlib.h>
#include <windows.h>

int GetX()
{
    HANDLE hStdout;
    CONSOLE_SCREEN_BUFFER_INFO pBuffer;
    hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(hStdout, &pBuffer);
    return pBuffer.dwCursorPosition.X;
}

int GetY()
{
    HANDLE hStdout;
    CONSOLE_SCREEN_BUFFER_INFO pBuffer;
    hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(hStdout, &pBuffer);
    return pBuffer.dwCursorPosition.Y;
}

void GotoXY(int x, int y)
{
    CONSOLE_SCREEN_BUFFER_INFO csbiInfo;                            
    HANDLE hConsoleOut;
    hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(hConsoleOut,&csbiInfo);
    csbiInfo.dwCursorPosition.X = x;                                    
    csbiInfo.dwCursorPosition.Y = y;                                    
    SetConsoleCursorPosition(hConsoleOut,csbiInfo.dwCursorPosition); 
}

std::string GetPass()
{
    std::string pwd;
    char tmp;
    while ((tmp = getch()) != 13)
    {
        if (tmp == 8)   // backspace
        {
            if (pwd.size() == 0)
                continue;
            pwd.erase(pwd.end() - 1);   // 删除末位
            int x = GetX(), y = GetY(); // 清掉屏幕上的一个'*'
            GotoXY(x - 1, y);
            std::cout << ' ';
            GotoXY(x - 1, y);
            continue;
        }
        pwd.append(1, tmp);
        std::cout << '*';
    }
    std::cout << '\n';
    return pwd;
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

bool ValidPassword(std::string pwd)
{
    for (int i = 0; i < pwd.size(); i++)
    {
        char ch = pwd[i];
        if (!((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9')))
            return false;
    }
    return ((pwd.size() >= 6) && (pwd.size() <= 15));
}

bool HighlightPrint(char* s_format, std::string text, int color = 9) 
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    if (handle == 0)
        return false;
    BOOL ret = SetConsoleTextAttribute(handle, WORD(color));
    printf(s_format, text.c_str());
    ret = SetConsoleTextAttribute(handle, WORD(7));
    return (ret == true);
}

void ClearScreen()
{
    system("cls");
    Title();
}

void Wait(int seconds)
{
    for (int i = 0; i < seconds * 60000000; ++i)
        ;
}

void Welcome()
{
    
}

void Title()
{
    HighlightPrint("%50s\n\n", "图书管理系统 v1.60.1");
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

int Find(std::vector<Book> v, std::string isbn, int index)
{
    for (int i = 0; i < v.size(); ++i)
    {
        if ((v[i].isbn() == isbn) && (v[i].index() == index))
            return i;
    }
    return -1;
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