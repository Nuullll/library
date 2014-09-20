// user.cpp

#include "user.h"

#include "global.h"
#include <conio.h>
#include "file.h"
#include <cmath>

void User::set_password()
{
    ClearScreen();
    std::cout << std::setw(40) << "请输入密码: ";
    std::string origin = GetPass();
    if (origin != password_)
    {
        HighlightPrint("密码错误!");
        getch();
        return;
    }
    std::cout << std::setw(40) << "新密码: ";
    std::string new_pwd = GetPass();
    if (!ValidPassword(new_pwd))
    {
        HighlightPrint("请设置6~15位密码!");
        getch();
        return;
    }
    std::cout << std::setw(40) << "重复新密码: ";
    std::string confirm = GetPass();
    if (new_pwd != confirm)
    {
        HighlightPrint("两次密码不一致!");
        getch();
        return;
    }
    password_ = new_pwd;
    update();
    HighlightPrint("设置成功!\n");
    getch();
    return;
}

std::ifstream &operator >>(std::ifstream &in, Administrator &admin)
{
    in >> admin.identity_;
    in >> admin.id_;
    std::string cipher;
    in >> cipher;
    admin.password_ = Decode(admin.id_, cipher);
    return in;
}

std::ofstream &operator <<(std::ofstream &out, const Administrator &admin)
{
    out << admin.identity_ << '\n';
    out << admin.id_ << '\n';
    out << Encode(admin) << '\n';
    return out;
}

void add_user()
{
    ClearScreen();
    std::vector<int> identities;
    identities.push_back("管理员");
    identities.push_back("读者");
    int k = 0;
    while (true)
    {
        MediatePrint("请选择新用户的身份: \n");
        for (int i = 0; i < 2; ++i)
        {
            if (i == k)
            {
                HighlightPrint(identities[i] + '\n');
                continue;
            }
            MediatePrint(identities[i] + '\n');
        }
        int ch = getch();
        if (ch == 13)
        {
            ClearScreen();
            HighlightPrint(identities[k] + '\n');
            break;
        }
        if (ch == 27)
            return;
        while (ch != 224)
            ;
        if ((ch = getch()) == UP || ch == DOWN)
            k = !k;
    }
    int new_id = 0;
    for (std::vector<User*>::iterator it = users.begin(); it != users.end(); ++it)
    {
        if (new_id <= (*it)->id())
            new_id = (*it)->id() + 1;
    }
    std::cout << std::setw(40) << "ID: " << new_id << std::endl;
    std::string new_pwd = RandomPass();
    MediatePrint("正在打印密码单...请稍候...\n\n");
    std::string space(10, ' ');
    std::cout << space << '|' << string(58,'-') << '|' << std::endl;
    Wait();
    std::cout << space << '|' << std::setw(59) << '|' << std::endl;
    Wait();
    std::cout << space << '|' << space + space << "ID: " << new_id << std::setw(34 - int(log10(new_id))) << '|' << std::endl;
    Wait();
    std::cout << space << '|' << space + space << "密码: " << new_pwd << std::setw(27) << '|' << std::endl;
    Wait();
    std::cout << space << '|' << std::setw(59) << '|' << std::endl;
    Wait();
    std::cout << space << '|' << string(58,'-') << '|' << std::endl << std::endl;
    // 密码单打印完毕.
    MediatePrint("密码单打印完毕! \n");
    MediatePrint("请尽快修改密码! \n");
    if (!k)     // 管理员
    {
        Administrator new_admin(new_id, new_pwd);
        new_admin.update();
    }
    else        // 读者
    {
        std::cout << std::setw(40) << "姓名: ";
        std::string new_name;
        std::cin >> new_name;
        Reader new_reader(new_id, new_pwd, new_name);
        new_reader.update();
    }
    getch();
    return;
}

void del_user()
{
    ClearScreen();
    std::cout << std::setw(40) << "学号(工号): ";
    int del_id;
    if (!(std::cin >> del_id))
    {
        HighlightPrint("输入错误! \n");
        getch();
        return;
    }
    int index = Find(users, del_id);
    if (index < 0)
    {
        HighlightPrint("用户不存在! \n");
        getch();
        return;
    }
    if (!(users[index]->identity()))    // 管理员
    {
        HighlightPrint("无权进行此操作! \n");
        getch();
        return;
    }
    else
    {
        users[index]->print();
        MediatePrint("确认删除? [y/n] ");
        char tmp = getch();
        if (tmp == 'y' || tmp == 'Y')
        {
            Remove(readers, del_id);
            WriteReaders();
            MediatePrint("用户删除成功!\n")
            getch();
            return;
        }
        MediatePrint("用户未删除!\n");
        getch();
        return;
    }
}

void all_book()
{
    PrintBooks(all_books);
    getch();
    return;
}

void add_book()
{
    ClearScreen();
    std::string new_isbn, new_name, new_author, new_publish;
    std::cout << std::setw(40) << "ISBN: ";
    std::cin >> new_isbn;
    if (new_isbn.size() != 10)
    {
        HighlightPrint("输入错误!\n");
        getch();
        return;
    }
    for (std::string::iterator it = new_isbn.begin(); it != new_isbn.end(); ++it)
    {
        if (!isdigit(*it))
        {
            HighlightPrint("输入错误!\n");
            getch();
            return;
        }
    }
    std::cout << std::setw(40) << "书名: ";
    getline(std::cin, new_name);
    std::cout << std::setw(40) << "作者: ";
    getline(std::cin, new_author);
    std::cout << std::setw(40) << "出版信息: ";
    getline(std::cin, new_publish);
    Book new_book(new_isbn, new_name, new_author, new_publish);
    new_book.update();
    MediatePrint("添加成功!\n");
    getch();
    return;
}

void del_book()
{
    ClearScreen();
    std::string del_isbn;
    std::cout << std::setw(40) << "ISBN: ";
    std::cin >> del_isbn;
    std::vector<Book> rst = Find(all_books, del_isbn);
    if (rst.size() == 0)
    {
        HighlightPrint("未找到该书!\n");
        getch();
        return;
    }
    PrintBooks(rst);
    MediatePrint("确认下架? [y全部下架; 或输入相应索引号] ");
    char tmp = getch();
    if (tmp == 'y' || tmp == 'Y')
    {
        Remove(all_books, rst);
        WriteBooks();
        MediatePrint("删除成功!\n");
        getch();
        return;
    }
    int del_index, index;
    if (!(std::cin >> index))
    {
        HighlightPrint("输入错误!\n");
        getch();
        return;
    }
    if ((index  = Find(all_books, del_isbn, del_index)) < 0)
    {
        HighlightPrint("输入错误!\n");
        getch();
        return;
    }
    Remove(all_books, del_isbn, del_index);
    WriteBooks();
    MediatePrint("删除成功!\n");
    getch();
    return;
}

void all_user()
{
    ClearScreen();
    for (std::vector<User*>::iterator it = users.begin(); it != users.end(); ++it)
        it->print();
    getch();
    return;
}

void Administrator::update()
{
    Remove(admins, id_);
    admins.push_back(*this);
    WriteAll();
    return;
}

std::ifstream &operator >>(std::ifstream &in, Reader &reader)
{
    in >> reader.identity_;
    in >> reader.id_;
    std::string cipher;
    in >> cipher;
    reader.password_ = Decode(reader.id_, cipher);
    in >> reader.name_;
    in >> reader.credit_;
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
                Book tmp;
                in >> tmp;
                reader.books_.push_back(tmp);
            }
        }
    }
    else
        in.seekg(-1, std::ios::cur);
    return in;
}

std::ofstream &operator <<(std::ofstream &out, const Reader &reader)
{
    out << reader.identity_ << '\n';
    out << reader.id_ << '\n';
    out << reader.password_ << '\n';
    out << reader.name_ << '\n';
    out << reader.credit_ << '\n';
    out << "*\n";
    for (std::vector<Book>::iterator it = reader.books_.begin(); it != reader.books_.end(); ++it)
        out << *it;
    out << "#\n\n";
    return out;
}

std::vector<Book> Reader::books(int state)
{
    std::vector<Book> v;
    for (std::vector<Book>::iterator it = books_.begin(); it != books_.end(); ++it)
    {
        if (it->state(id_) == state)
            v.push_back(*it);
    }
    return v;
}

void Reader::update()
{
    Remove(readers, id_);
    readers.push_back(*this);
    WriteAll();
    return;
}
