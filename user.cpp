// user.cpp

#include "user.h"

#include "global.h"
#include <conio.h>
#include "file.h"
#include <cmath>
#include "menu.h"

void User::set_password()
{
    ClearScreen();
    std::cout << std::setw(WIDTH / 2) << "请输入密码: ";
    std::string origin = GetPass();
    if (origin != password_)
    {
        HighlightPrint("密码错误!");
        getch();
        return;
    }
    std::cout << std::setw(WIDTH / 2) << "新密码: ";
    std::string new_pwd = GetPass();
    if (!ValidPassword(new_pwd))
    {
        HighlightPrint("请设置6~15位密码!");
        getch();
        return;
    }
    std::cout << std::setw(WIDTH / 2) << "重复新密码: ";
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

void Administrator::add_user()
{
    ClearScreen();
    std::vector<std::string> identities;
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
    std::cout << std::setw(WIDTH / 2) << "ID: " << new_id << std::endl;
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
        std::cout << std::setw(WIDTH / 2) << "姓名: ";
        std::string new_name;
        std::cin >> new_name;
        Reader new_reader(new_id, new_pwd, new_name);
        new_reader.update();
    }
    getch();
    return;
}

void Administrator::del_user()
{
    ClearScreen();
    std::cout << std::setw(WIDTH / 2) << "学号(工号): ";
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
        Reader reader = readers[Find(readers, del_id)];
        if (reader.books(HOLDING).size() != 0 || reader.books(WANTED).size() != 0 || reader.books(OVERDUE).size() != 0)
        {
            HighlightPrint("该用户还有书未还! 删除失败!\n");
            getch();
            return;
        }
        MediatePrint("确认删除? [y/n] ");
        char tmp = getch();
        if (tmp == 'y' || tmp == 'Y')
        {
            Remove(readers, del_id);
            WriteReaders();
            ReadAll();
            MediatePrint("用户删除成功!\n")
            getch();
            return;
        }
        MediatePrint("用户未删除!\n");
        getch();
        return;
    }
}

void Administrator::all_book()
{
    PrintBooks(all_books);
    getch();
    return;
}

void Administrator::add_book()
{
    ClearScreen();
    std::string new_isbn, new_name, new_author, new_publish;
    std::cout << std::setw(WIDTH / 2) << "ISBN: ";
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
    std::cout << std::setw(WIDTH / 2) << "书名: ";
    getline(std::cin, new_name);
    std::cout << std::setw(WIDTH / 2) << "作者: ";
    getline(std::cin, new_author);
    std::cout << std::setw(WIDTH / 2) << "出版信息: ";
    getline(std::cin, new_publish);
    Book new_book(new_isbn, new_name, new_author, new_publish);
    new_book.update();
    MediatePrint("添加成功!\n");
    getch();
    return;
}

void Administrator::del_book()
{
    ClearScreen();
    std::string del_isbn;
    std::cout << std::setw(WIDTH / 2) << "ISBN: ";
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
        for (std::vector<Book>::iterator it = rst.begin(); it != rst.end(); ++it)
        {
            std::vector<Info> tmp = it->info();
            for (std::vector<Info>::iterator iter = tmp.begin(); iter != tmp.end(); ++iter)
            {
                if (iter->state == HOLDING || iter->state == WANTED || iter->state == OVERDUE)
                {
                    HighlightPrint("该书仍未被归还! 删除失败!\n");
                    getch();
                    return;
                }
            }
        }
        Remove(all_books, rst);
        WriteBooks();
        ReadAll();
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
    Book tmp = all_books[Find(all_books, del_isbn, del_index)];
    std::vector<Info> info = tmp.info();
    for (std::vector<Info>::iterator it = info.begin(); it != info.end(); ++it)
    {
        if (it->state == HOLDING || it->state == WANTED || it->state == OVERDUE)
        {
            HighlightPrint("该书仍未被归还! 删除失败!\n");
            getch();
            return;
        }
    }
    Remove(all_books, del_isbn, del_index);
    WriteBooks();
    ReadAll();
    MediatePrint("删除成功!\n");
    getch();
    return;
}

void Administrator::all_user()
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
    WriteAdmins();
    ReadAll();
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

bool Reader::change_state(Book book, int state)
{
    for (std::vector<Book>::iterator it = books_.begin(); it != books_.end(); ++it)
    {
        if (*it == book)
        {
            it->change_state(id_, state);
            return true;
        }
    }
    return false;
}

std::vector<Book> Reader::books(int state)
{
    std::vector<Book> rst;
    for (std::vector<Book>::iterator it = books_.begin(); it != books_.end(); ++it)
    {
        if (it->info(id_).state == state)
            rst.push_back(*it);
    }
    return rst;
}

void Reader::history()
{
    ClearScreen();
    PrintBooksOfReader(books(RETURNED), id_);
    getch();
    return;
}

void Reader::recommended()
{
    ClearScreen();
    std::vector<Book> recommended = HotBook(5);
    if (recommended.size() == 0)
    {
        MediatePrint("馆长还没有推荐什么好书哦!\n");
        getch();
        return;
    }
    std::vector<std::string> book_names;
    for (std::vector<Book>::iterator it = recommended.begin(); it != recommended.end(); ++it)
    {
        book_names.push_back(it->name());
    }
    int k = 0;
    while (true)
    {
        ClearScreen();
        for (int i = 0; i < book_names.size(); ++i)
        {
            if (i == k)
            {
                HighlightPrint(book_names[i] + '\n');
                continue;
            }
            MediatePrint(book_names[i] + '\n');
        }
        int ch = getch();
        if (ch == 13)
        {
            ClearScreen();
            Book target = recommended[k];
            target.print();
            target.display_now_state();
            MediatePrint("借阅/预约此书? [y/n] ");
            int tmp = getch();
            if (tmp == 'y' || tmp == 'Y')
            {
                target.wanted(id_);
            }
        }
        while (ch != 224)
            ;
        switch (getch())
        {
            case UP:
            {
                if (--k < 0)
                    k = book_names.size() - 1;
                break;
            }
            case DOWN:
            {
                if (++k > book_names.size() - 1)
                    k = 0;
                break;
            }
            default: break;
        }
    }
    getch();
    return;
}

void Reader::give_back()
{
    ClearScreen();
    std::vector<Book> can_return;
    std::vector<Book> tmp = books(HOLDING);
    can_return.insert(can_return.end(), tmp.begin(), tmp.end());
    tmp = books(WANTED);
    can_return.insert(can_return.end(), tmp.begin(), tmp.end());
    tmp = books(OVERDUE);
    can_return.insert(can_return.end(), tmp.begin(), tmp.end());
    std::vector<std::string> book_names;
    for (std::vector<Book>::iterator it = can_return.begin(); it != can_return.end(); ++it)
    {
        book_names.push_back(it->name());
    }
    int k = 0;
    while (true)
    {
        ClearScreen();
        for (int i = 0; i < book_names.size(); ++i)
        {
            if (i == k)
            {
                HighlightPrint(book_names[i] + '\n');
                continue;
            }
            MediatePrint(book_names[i] + '\n');
        }
        int ch = getch();
        if (ch == 13)
        {
            ClearScreen();
            Book target = can_return[k];
            target.print();
            target.display_now_state();
            MediatePrint("归还此书? [y/n] ");
            int tmp = getch();
            if (tmp == 'y' || tmp == 'Y')
            {
                target.give_back(id_);
            }
        }
        while (ch != 224)
            ;
        switch (getch())
        {
            case UP:
            {
                if (--k < 0)
                    k = book_names.size() - 1;
                break;
            }
            case DOWN:
            {
                if (++k > book_names.size() - 1)
                    k = 0;
                break;
            }
            default: break;
        }
    }
    getch();
    return;
}

void Reader::search()
{
    ClearScreen();
    std::cout << std::setw(WIDTH / 2) << "关键词/ISBN: ";
    std::vector<std::string> keywords;
    std::string tmp;
    while (std::cin >> tmp && std::cin.get() != '\n')
        keywords.push_back(tmp);
    std::vector<Book> search_result;
    for (std::vector<Book>::iterator it = all_books.begin(); it != all_books.end(); ++it)
    {
        for (std::vector<std::string>::iterator iter = keywords.begin(); iter != keywords.end(); ++iter)
        {
            if (it->name().find(*iter) != std::string::npos || it->isbn().find(*iter) != std::string::npos)
                search_result.push_back(*it);
        }
    }
    std::vector<std::string> book_names;
    for (std::vector<Book>::iterator it = search_result.begin(); it != search_result.end(); ++it)
    {
        book_names.push_back(it->name());
    }
    int k = 0;
    while (true)
    {
        ClearScreen();
        for (int i = 0; i < book_names.size(); ++i)
        {
            if (i == k)
            {
                HighlightPrint(book_names[i] + '\n');
                continue;
            }
            MediatePrint(book_names[i] + '\n');
        }
        int ch = getch();
        if (ch == 13)
        {
            ClearScreen();
            Book target = search_result[k];
            target.print();
            target.display_now_state();
            MediatePrint("借阅/预约此书? [y/n] ");
            int tmp = getch();
            if (tmp == 'y' || tmp == 'Y')
            {
                target.wanted(id_);
            }
        }
        while (ch != 224)
            ;
        switch (getch())
        {
            case UP:
            {
                if (--k < 0)
                    k = book_names.size() - 1;
                break;
            }
            case DOWN:
            {
                if (++k > book_names.size() - 1)
                    k = 0;
                break;
            }
            default: break;
        }
    }
    getch();
    return;
}

void Reader::update()
{
    Remove(readers, id_);
    readers.push_back(*this);
    WriteReaders();
    ReadAll();
    return;
}