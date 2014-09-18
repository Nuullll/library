// book.cpp

#include "book.h"

#include "global.h"

Book::Book(std::string isbn, std::string name, int days)
:isbn_(isbn), name_(name), index_(0), on_shelf_(true), days_(days)
{
    for (std::vector<Book>::iterator it = books.begin(); it != books.end(); ++it)
    {
        if (it->isbn() == isbn)
            ++index_;
    }
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