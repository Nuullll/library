// book.h

#ifndef BOOK_H
#define BOOK_H 

#include <string>
#include <vector>
#include <ctime>

enum State 
{ 
    HOLDING,    // 正常持有
    REQUEST,    // 预约此书
    WANTED,     // 被其他读者催还
    OVERDUE,    // 逾期未还
    RETURNED    // 已还
};

const int DAYS = 30;

struct Info
{
	Info() {}
	Info(int reader, int state, time_t start_time, time_t end_time): reader(reader), state(state), start_time(start_time), end_time(end_time) {}
	bool operator ==(const int &reader_id) { return (reader == reader_id); }
    friend std::ifstream &operator >>(std::ifstream &in, Info &info);
    friend std::ofstream &operator <<(std::ofstream &out, const Info &info);
    void print();
    void print_duration();
    int reader;             // 读者ID
    int state;              // 借阅状态
    time_t start_time;      // 开始日期
    time_t end_time;        // 结束日期
};

class Book
{
public:
    Book() {}
    Book(std::string isbn, std::string name, std::string author, std::string publish, int days = DAYS);
    ~Book() {}

    bool operator ==(const Book &b) { return ((isbn_ == b.isbn_) && (index_ == b.index_)); }

    friend std::ifstream &operator >>(std::ifstream &in, Book &book);
    friend std::ofstream &operator <<(std::ofstream &out, const Book &book);

    bool on_shelf() { return on_shelf_; }
    Info &info(int reader_id);       // 返回该读者借阅此书的信息
    int days() { return days_; }
    int index() { return index_; }
    std::string author() { return author_; }
    std::string isbn() { return isbn_; }
    std::string name() { return name_; }
    std::string publish() { return publish_; }
    std::vector<Info> info() { return info_; }
    std::vector<Info> same_state(int state);
    void change_state() { on_shelf_ = !on_shelf_; }    // 借出/归还
    void change_state(int reader_id, int state);
    void display_now_state();
    void give_back(int reader_id);     // 读者归还
    void print();
    void update();
    void wanted(int reader_id);     // 有读者需要, 若在架上则成功借出, 否则变更相关状态.

private:
    std::string isbn_;      // ISBN号, 10位数字, unsigned不能完全存下, 故用string.
    std::string name_;      // 书名
    std::string author_;    // 作者
    std::string publish_;   // 出版信息
    int index_;             // 同一版书籍可能有多本, ISBN相同的书籍利用index相互区分.
    bool on_shelf_;         // 当前是否在架上
    int days_;              // 正常情况下可借阅的天数
    std::vector<Info> info_;// 与此书相关的所有读者信息
};


#endif