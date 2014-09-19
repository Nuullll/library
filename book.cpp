// book.cpp

#include "book.h"

#include "global.h"

std::ifstream &operator >>(std::ifstream &in, Info &info)
{
    in >> info.reader;
    in >> info.state;
    in >> info.start_time;
    in >> info.end_time;
    return in;
}

std::ofstream &operator <<(std::ofstream &out, const Info &info)
{
    out << info.reader << '\n';
    out << info.state << '\n';
    out << info.start_time << '\n';
    out << info.end_time << '\n';
    return out;
}

Book::Book(std::string isbn, std::string name, std::string author, 
           std::string publish, int days): 
           isbn_(isbn), name_(name), author_(author), 
           publish_(publish), index_(0), on_shelf_(true), days_(days)
{
    for (std::vector<Book>::iterator it = books.begin(); it != books.end(); ++it)
    {
        if (it->isbn() == isbn)
            ++index_;
    }
}

friend std::ifstream &operator >>(std::ifstream &in, Book &book)
{
    in >> book.isbn_;
    in >> book.name_;
    in >> book.author_;
    in >> book.publish_;
    in >> index_;
    in >> on_shelf_;
    in >> days_;
    char start_flag;
    while ((start_flag = in.get()) == '\n')
        ;
    if (start_flag == '*')
    {
        while (true)
        {
            char end_flag;
            while ((end_flag = in.get()) == '\n')
                ;
            if (end_flag == '#')
                break;
            else
            {
                in.seekg(-1, std::ios::cur);
                Info tmp;
                in >> tmp;
                book.info_.push_back(tmp);
            }
        }
    }
    else
        in.seekg(-1, std::ios::cur);
    return in;
}

friend std::ofstream &operator <<(std::ofstream &out, const Book &book)
{
    out << book.isbn_ << '\n';
    out << book.name_ << '\n';
    out << book.author_ << '\n';
    out << book.publish_ << '\n';
    out << book.index_ << '\n';
    out << book.on_shelf_ << '\n';
    out << book.days << '\n';
    out << "*\n";
    for (std::vector<Info>::iterator it = book.info_.begin(); it != book.info_.end(); ++it)
        out << *it;
    out << "#\n\n";
    return out;
}

int Book::state(int reader_id)
{
    for (std::vector<Info>::iterator it = info_.begin(); it != info_.end(); ++it)
    {
        if (it->reader == reader_id)
            return it->state;
    }
    return -1;      // 无借阅关系
}