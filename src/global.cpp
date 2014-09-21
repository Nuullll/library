// global.cpp

#include "global.h"

#include <stdlib.h>
#include <windows.h>
#include <algorithm>
#include <iomanip>
#include <ctime>
#include <conio.h>
#include "token.h"

std::vector<User*> users;
std::vector<Administrator> admins;
std::vector<Reader> readers;
std::vector<Book> all_books;

bool CompareHot(Book b1, Book b2)
{
    return (b1.info().size() > b2.info().size());
}

bool HighlightPrint(int setw, std::string text, int color)
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    if (handle == 0)
        return false;
    BOOL ret = SetConsoleTextAttribute(handle, WORD(color));
    std::cout << std::setw(setw) << text;
    ret = SetConsoleTextAttribute(handle, WORD(7));
    return (ret == true);
}

bool HighlightPrint(std::string text, int color) 
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

std::string RandomPass(int digits)
{
    char set[] = {'0','1','2','3','4','5','6','7','8','9',
                  'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',
                  'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
    std::string pwd;
    for (int i = 0; i < digits; ++i)
    {
        srand((unsigned int)((i + 1) * time(NULL)));
        pwd.push_back(set[rand() % 52]);
    }
    return pwd;
}

std::vector<Book> Find(std::vector<Book> v, std::string isbn)
{
    std::vector<Book> rst;
    for (std::vector<Book>::iterator it = v.begin(); it != v.end(); ++it)
    {
        if (it->isbn() == isbn)
            rst.push_back(*it);
    }
    return rst;
}

std::vector<Book> HotBook(int num)
{
    std::sort(all_books.begin(), all_books.end(), CompareHot);
	num = (num <= all_books.size() ? num : all_books.size());
	return std::vector<Book>(all_books.begin(), all_books.begin() + num);
}

Token Login()
{
    MediatePrint("请登录...\n");
    int id;
    std::string pwd;
    std::cout << std::setw(WIDTH / 2) << "请输入您的学号(工号): ";
    std::cin >> id;
    std::cout << std::setw(WIDTH / 2) << "请输入您的密码: ";
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
	getch();
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

void PrintBooks(std::vector<Book> v)
{
	ClearScreen();
    std::string seperator(80, '-');
    for (std::vector<Book>::iterator it = v.begin(); it != v.end(); ++it)
    {
        it->print();
        std::cout << seperator << std::endl;
    }
    return;
}

void PrintBooksOfReader(std::vector<Book> v, int reader_id)
{
    std::string seperator(80, '-');
    for (std::vector<Book>::iterator it = v.begin(); it != v.end(); ++it)
    {
        it->print();
        it->info(reader_id).print();
        std::cout << seperator << std::endl;
    }
    return;
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

void Remove(std::vector<Book> &v, std::vector<Book> dels)
{
    for (std::vector<Book>::iterator it = dels.begin(); it != dels.end(); ++it)
        Remove(v, *it);
}

void Title()
{
    HighlightPrint("图书管理系统 v1.60.1\n\n");
}

void Wait(int milliseconds)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}
