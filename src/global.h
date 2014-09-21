// global.h

#ifndef GLOBAL_H
#define GLOBAL_H

#include "user.h"
#include <vector>
#include <iomanip>
#include <iostream>
#include <cmath>
#include <thread>
#include <chrono>
#include <utility>

extern std::vector<User*> users;
extern std::vector<Administrator> admins;
extern std::vector<Reader> readers;
extern std::vector<Book> all_books;     // ��Reader::books_����

typedef std::pair<int, char> Key;	// first = times, second = ch;

const Key ENTER = std::make_pair(1, char(13));
const Key ESCAPE = std::make_pair(1, char(27));
const Key UP = std::make_pair(2, char(72));
const Key DOWN = std::make_pair(2, char(80));
const Key LEFT = std::make_pair(2, char(75));
const Key RIGHT = std::make_pair(2, char(77));


const int SECONDS_A_DAY = 24 * 3600;

Key MyGetCh();
bool CompareHot(Book b1, Book b2);
bool HighlightPrint(int setw, std::string text, int color = 9);
bool HighlightPrint(std::string text, int color = 9);
bool ValidPassword(std::string pwd);    // ����Ƿ����6~15λ������ĸ�����׼
int Find(std::vector<Book> v, std::string isbn, int index);
int Find(std::vector<User*> v, int id);     // ����vector���ҵ�������, δ�ҵ��򷵻�-1
std::string Decode(int id, std::string cipher); // ����
std::string GetPass();      // ��������
std::string RandomPass(int digits = 6);     // ����digitsλ�������
std::vector<Book> Find(std::vector<Book> v, std::string isbn);
std::vector<Book> HotBook(int num = 3);    // �������а�
Token Login();              // ��¼, ��������
void ClearScreen();         // �����������������
void EBook();               // ������Դ����
void Exit();                // �˳�ϵͳ
void MediatePrint(std::string text);    // ���д�ӡ�ַ���
void PrintBooks(std::vector<Book> v);
void PrintBooksOfReader(std::vector<Book> v, int reader_id);
void Remove(std::vector<Book> &v, std::string isbn, int index);
void Remove(std::vector<Book> &v, std::vector<Book> dels);
void Scroll(std::string text);  // ���������ı�
void Title();               // ϵͳ����
void Wait(int milliseconds = 1000);  // �ȴ�������
void Welcome();             // ��ӭ����

template <class T>
std::string Encode(T user)      // ����
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