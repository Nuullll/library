// user.cpp

#include "user.h"

#include "global.h"
#include <conio.h>
#include "file.h"
#include <cmath>

void User::set_password()
{
    ClearScreen();
    printf("%40s", "原密码: ")
    std::string origin = GetPass();
    if (origin != password_)
    {
        HighlightPrint("%45s\n", "密码错误!");
        getch();
        return;
    }
    printf("%40s", "新密码: ")
    std::string new_pwd = GetPass();
    if (!ValidPassword(new_pwd))
    {
        HighlightPrint("%54s\n", "请设置6~15位数字字母密码!");
        getch();
        return;
    }
    printf("%40s", "重复新密码: ")
    std::string confirm = GetPass();
    if (new_pwd != confirm)
    {
        HighlightPrint("%47s\n", "两次密码不一致!");
        getch();
        return;
    }
    password_ = new_pwd;
    update();
    HighlightPrint("%44s\n", "设置成功!\n");
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
    std::cout << std::setw(40) << "请输入姓名: ";
    std::string new_name;
    std::cin >> new_name;
    MediatePrint("正在打印密码单...请稍候...\n");
    std::string space(10, ' ');
    std::cout << space << '|' << string(58,'-') << '|' << std::endl;
    std::cout << space << '|' << std::setw(59) << '|' << std::endl;
    std::cout << space << '|' << space + space << "ID: " << new_id << std::setw(34 - int(log10(new_id))) << '|' << std::endl;
    std::cout << space << '|' << space + space << "密码: " << RandomPass(6) << std::setw(27) << '|' << std::endl;
    std::cout << space << '|' << std::setw(59) << '|' << std::endl;
    std::cout << space << '|' << string(58,'-') << '|' << std::endl << std::endl;
}

void all_user()
{
    for (std::vector<User*>::iterator it = users.begin(); it != users.end(); ++it)
        it->print();
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
