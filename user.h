// user.h

#ifndef USER_H
#define USER_H 

#include <string>
#include <iostream>
#include <vector>
#include "book.h"

enum UserType { ADMINISTRATOR, READER };

const int CREDIT = 80;      // 初始信用积分

class User
{
public:
    User() {}
    User(int identity, int id, std::string password): identity_(identity), id_(id), password_(password) {}
    virtual ~User() {}

    friend class Token;
    template <class T>
    friend std::string Encode(T user);

    bool operator ==(const int &id) { return id_ == id; }

    virtual void print() = 0;   // 显示用户基本信息
    virtual void update() = 0;  // 更新全局vector及文件
    
    int id() { return id_; }
    int identity() {return identity_; }
    void set_password();        // 重设密码

protected:
    int identity_;              // 用户身份
    int id_;                    // 用户ID
    std::string password_;      // 密码
};

class Administrator: public User
{
public:
    Administrator() {}
    Administrator(int id, std::string password): User(ADMINISTRATOR, id, password) {}
    virtual ~Administrator() {}

    friend std::ifstream &operator >>(std::ifstream &in, Administrator &admin);
    friend std::ofstream &operator <<(std::ofstream &out, const Administrator &admin);

    void add_book();            // 新书上架
    void add_user();            // 增加新用户
    void all_book();            // 查看所有书籍信息
    void all_user();            // 查看所有用户信息
    void del_book();            // 旧书下架
    void del_user();            // 删除用户
    void print() { printf("%40s", "管理员: "), std::cout << id_ << std::endl; }
    void update();

private:

};

class Reader: public User
{
public:
    Reader() {}
    Reader(int id, std::string password, std::string name): User(id, password), name_(name), credit_(CREDIT) { books_.reserve(256); }
    virtual ~Reader() {}

    friend std::ifstream &operator >>(std::ifstream &in, Reader &reader);
    friend std::ofstream &operator <<(std::ofstream &out, const Reader &reader);

    bool change_state(Book book, int state);        // 变更book的状态为state
    int credit() { return credit_; }
    std::string name() { return name_; }
    std::vector<Book> books(int state);
    void print() { printf("%40s", "读者"), std::cout << id_ << ": " << name_ << std::endl; }
    void update();

private:
    std::string name_;          // 读者姓名
    int credit_;                // 信用积分
    std::vector<Book> books_;   // 与该读者相关的书(包括借阅过, 持有, 预约等)    
};

#endif