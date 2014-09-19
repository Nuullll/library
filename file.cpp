// file.cpp

#include "file.h"
#include "user.h"
#include "global.h"
#include <vector>

void ReadAdmins()
{
    ifstream in("./data/admins.txt", std::ios::in);
    admins.clear();
    Administrator tmp;
    while (in >> tmp)
        admins.push_back(tmp);
}

void ReadBooks()
{
    ifstream in("./data/books.txt", std::ios::in);
    all_books.clear();
    Book tmp;
    while (in >> tmp)
        all_books.push_back(tmp);
}

void ReadReaders()
{
    ifstream in("./data/readers.txt", std::ios::in);
    readers.clear();
    Reader tmp;
    while (in >> tmp)
        readers.push_back(tmp);
}

void ReadUsers()
{
    users.clear();
    User* tmp;
    for (std::vector<Administrator>::iterator it = admins.begin(); it != admins.end(); ++it)
    {
        tmp = &*it;
        users.push_back(tmp);
    }
    for (std::vector<Reader>::iterator it = readers.begin(); it != readers.end(); ++it)
    {
        tmp = &*it;
        users.push_back(tmp);
    }
}

void ReadAll()
{
    ReadAdmins();
    ReadReaders();
    ReadUsers();
    ReadBooks();
}

void WriteAdmins()
{
    ofstream out("./data/admins.txt", std::ios::out|std::ios::trunc);
    for (std::vector<Administrator>::iterator it = admins.begin(); it != admins.end(); ++it)
        out << *it;
}

void WriteBooks()
{
    ofstream out("./data/books.txt", std::ios::out|std::ios::trunc);
    for (std::vector<Book>::iterator it = all_books.begin(); it != all_books.end(); ++it)
        out << *it;
}

void WriteReaders()
{
    ofstream out("./data/readers.txt", std::ios::__out|std::ios::trunc);
    for (std::vector<Reader>::iterator it = readers.begin(); it != readers.end(); ++it)
        out << *it;
}

void WriteAll()
{
    WriteAdmins();
    WriteReaders();
    WriteBooks();
}