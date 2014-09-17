// user.cpp

#include "user.h"

#include "global.h"
#include <conio.h>

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