// user.h

#ifndef USER_H
#define USER_H 

#include <string>
#include <iostream>
#include <iomanip>
#include <vector>
#include "book.h"

const int WIDTH = 80;

enum UserType { ADMINISTRATOR, READER };

const int CREDIT = 80;      // ��ʼ���û���

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

    virtual void print() = 0;   // ��ʾ�û�������Ϣ
    virtual void update() = 0;  // ����ȫ��vector���ļ�
    
    int id() { return id_; }
    int identity() {return identity_; }
    void set_password();        // ��������

protected:
    int identity_;              // �û����
    int id_;                    // �û�ID
    std::string password_;      // ����
};

class Administrator: public User
{
public:
    Administrator() {}
    Administrator(int id, std::string password): User(ADMINISTRATOR, id, password) {}
    virtual ~Administrator() {}

    friend std::ifstream &operator >>(std::ifstream &in, Administrator &admin);
    friend std::ofstream &operator <<(std::ofstream &out, const Administrator &admin);

    void add_book();            // �����ϼ�
    void add_user();            // �������û�
    void all_book();            // �鿴�����鼮��Ϣ
    void all_user();            // �鿴�����û���Ϣ
    void del_book();            // �����¼�
    void del_user();            // ɾ���û�
    void print() { std::cout << std::setw(WIDTH / 2) << "����Ա: " << id_ << std::endl; }
    void update();

private:

};

class Reader: public User
{
public:
	Reader() { books_.reserve(256); }
    Reader(int id, std::string password, std::string name): User(READER, id, password), name_(name), credit_(CREDIT) { books_.reserve(256); }
    virtual ~Reader() {}

    friend std::ifstream &operator >>(std::ifstream &in, Reader &reader);
    friend std::ofstream &operator <<(std::ofstream &out, const Reader &reader);

    bool change_state(Book book, int state);        // ���book��״̬Ϊstate
    int change_credit(int diff) { return credit_ += diff; }        // ���û��ֱ仯
    int credit() { return credit_; }
    std::string name() { return name_; }
    std::vector<Book> books(int state = 0);
	void add_book(Book book);
	void del_book(Book book);
    void history();     // ������ʷ����
    void print() { std::cout << std::setw(WIDTH / 2 - 10) << "����: " << id_ << "  " << name_ << "  ��ǰ���û���: " << credit_ << std::endl; }
    void recommended(); // �ݳ��Ƽ�����
    void give_back();   // ���߻������
    void search();      // �����鼮����
    void update();

private:
    std::string name_;          // ��������
    int credit_;                // ���û���
    std::vector<Book> books_;   // ��ö�����ص���(�������Ĺ�, ����, ԤԼ��)    
};

#endif