// book.h

#ifndef BOOK_H
#define BOOK_H 

#include <string>
#include <vector>
#include <ctime>

enum State 
{ 
    HOLDING,    // 持有
    WANT,       // 预约此书
    WANTED,     // 被其他读者催还
    OVERDUE,    // 逾期未还
    RETURNED    // 已换
};

const int DAYS = 30;

struct Info
{
    int reader;             // 读者ID
    int state;              // 借阅状态
    tm * start_time;        // 开始日期
    tm * end_time;          // 结束日期
};

class Book
{
public:
    Book() {}
    Book(std::string isbn, std::string name, int days = DAYS);
    ~Book() {}

    bool operator ==(const Book &b) { return ((isbn_ == b.isbn_) && (index_ == b.index_)); }

    friend std::ifstream &operator >>(std::ifstream &in, Book &book);
    friend std::ofstream &operator <<(std::ofstream &out, const Book &book);

    bool change_state() { return (on_shelf_ = !on_shelf_); }    // 借出/归还
    bool on_shelf() { return on_shelf_; }
    bool return(int reader_id);     // 读者归还
    bool wanted(int reader_id);     // 有读者需要, 若在架上则成功借出, 否则变更相关状态.
    int days() { return days_; }
    int index() { return index_; }
    int state(int reader_id);       // 返回该读者与此书间的借阅状态
    std::string isbn() { return isbn_; }
    std::string name() { return name_; }
    std::vector<Info> info() { return info_; }
    void update();

private:
    std::string isbn_;      // ISBN号, 10位数字, unsigned不能完全存下, 故用string.
    std::string name_;      // 书名
    int index_;             // 同一版书籍可能有多本, ISBN相同的书籍利用index相互区分.
    bool on_shelf_;         // 当前是否在架上
    int days_;              // 正常情况下可借阅的天数
    std::vector<Info> info_;// 与此书相关的所有读者信息
};


#endif