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
    std::cout << std::setw(WIDTH / 2) << "����������: ";
    std::string origin = GetPass();
    if (origin != password_)
    {
        HighlightPrint("�������!");
        MyGetCh();
        return;
    }
    std::cout << std::setw(WIDTH / 2) << "������: ";
    std::string new_pwd = GetPass();
    if (!ValidPassword(new_pwd))
    {
        HighlightPrint("������6~15λ����!");
        MyGetCh();
        return;
    }
    std::cout << std::setw(WIDTH / 2) << "�ظ�������: ";
    std::string confirm = GetPass();
    if (new_pwd != confirm)
    {
        HighlightPrint("�������벻һ��!");
        MyGetCh();
        return;
    }
    password_ = new_pwd;
    update();
    HighlightPrint("���óɹ�!\n");
    MyGetCh();
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
    identities.push_back("����Ա");
    identities.push_back("����");
    int k = 0;
    while (true)
    {
		ClearScreen();
        MediatePrint("��ѡ�����û������: \n");
        for (int i = 0; i < 2; ++i)
        {
            if (i == k)
            {
                HighlightPrint(identities[i] + '\n');
                continue;
            }
            MediatePrint(identities[i] + '\n');
        }
        Key ch = MyGetCh();
        if (ch == RIGHT || ch == ENTER)
        {
            ClearScreen();
            HighlightPrint(identities[k] + '\n');
            break;
        }
        if (ch == LEFT || ch == ESCAPE)
            return;
        if (ch == UP || ch == DOWN)
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
    MediatePrint("���ڴ�ӡ���뵥...���Ժ�...\n\n");
    std::string space(10, ' ');
    std::cout << space << '|' << std::string(58,'-') << '|' << std::endl;
    Wait();
    std::cout << space << '|' << std::setw(59) << '|' << std::endl;
    Wait();
    std::cout << space << '|' << space + space << "ID: " << new_id << std::setw(34 - int(log10(new_id))) << '|' << std::endl;
    Wait();
    std::cout << space << '|' << space + space << "����: " << new_pwd << std::setw(27) << '|' << std::endl;
    Wait();
    std::cout << space << '|' << std::setw(59) << '|' << std::endl;
    Wait();
    std::cout << space << '|' << std::string(58,'-') << '|' << std::endl << std::endl;
    // ���뵥��ӡ���.
    MediatePrint("���뵥��ӡ���! \n");
    MediatePrint("�뾡���޸�����! \n");
    if (!k)     // ����Ա
    {
        Administrator new_admin(new_id, new_pwd);
        new_admin.update();
    }
    else        // ����
    {
        std::cout << std::setw(WIDTH / 2) << "����: ";
        std::string new_name;
        std::cin >> new_name;
        Reader new_reader(new_id, new_pwd, new_name);
        new_reader.update();
    }
    MyGetCh();
    return;
}

void Administrator::del_user()
{
    ClearScreen();
    std::cout << std::setw(WIDTH / 2) << "ѧ��(����): ";
    int del_id;
    if (!(std::cin >> del_id))
    {
        HighlightPrint("�������! \n");
        MyGetCh();
        return;
    }
    int index = Find(users, del_id);
    if (index < 0)
    {
        HighlightPrint("�û�������! \n");
        MyGetCh();
        return;
    }
    if (!(users[index]->identity()))    // ����Ա
    {
        HighlightPrint("��Ȩ���д˲���! \n");
        MyGetCh();
        return;
    }
    else
    {
        users[index]->print();
        Reader reader = readers[Find(readers, del_id)];
        if (reader.books(HOLDING).size() != 0 || reader.books(WANTED).size() != 0 || reader.books(OVERDUE).size() != 0)
        {
            HighlightPrint("���û�������δ��! ɾ��ʧ��!\n");
            MyGetCh();
            return;
        }
        MediatePrint("ȷ��ɾ��? [y/n] \n");
        Key ch = MyGetCh();
        if (ch == std::make_pair(1, 'y') || ch == std::make_pair(1, 'Y'))
        {
            Remove(readers, del_id);
            WriteReaders();
            ReadAll();
            MediatePrint("�û�ɾ���ɹ�!\n");
            MyGetCh();
            return;
        }
        MediatePrint("�û�δɾ��!\n");
        MyGetCh();
        return;
    }
}

void Administrator::all_book()
{
    PrintBooks(all_books);
    MyGetCh();
    return;
}

void Administrator::add_book()
{
    ClearScreen();
    std::string new_isbn, new_name, new_author, new_publish;
    std::cout << std::setw(WIDTH / 2) << "ISBN: ";
    std::cin >> new_isbn;
	std::cin.get();		// �����س�
    if (new_isbn.size() != 10 && new_isbn.size() != 13)
    {
        HighlightPrint("�������!\n");
        MyGetCh();
        return;
    }
    for (std::string::iterator it = new_isbn.begin(); it != new_isbn.end(); ++it)
    {
        if (!isdigit(*it))
        {
            HighlightPrint("�������!\n");
            MyGetCh();
            return;
        }
    }
    std::cout << std::setw(WIDTH / 2) << "����: ";
    getline(std::cin, new_name);
    std::cout << std::setw(WIDTH / 2) << "����: ";
    getline(std::cin, new_author);
    std::cout << std::setw(WIDTH / 2) << "������Ϣ: ";
    getline(std::cin, new_publish);
    Book new_book(new_isbn, new_name, new_author, new_publish);
    new_book.update();
    MediatePrint("��ӳɹ�!\n");
    MyGetCh();
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
        HighlightPrint("δ�ҵ�����!\n");
        MyGetCh();
        return;
    }
    PrintBooks(rst);
    MediatePrint("ȷ���¼�? [yȫ���¼�; ��������Ӧ������] \n");
    Key ch = MyGetCh();
    if (ch == std::make_pair(1, 'y') || ch == std::make_pair(1, 'Y'))
    {
        for (std::vector<Book>::iterator it = rst.begin(); it != rst.end(); ++it)
        {
            std::vector<Info> tmp = it->info();
            for (std::vector<Info>::iterator iter = tmp.begin(); iter != tmp.end(); ++iter)
            {
                if (iter->state == HOLDING || iter->state == WANTED || iter->state == OVERDUE)
                {
                    HighlightPrint("������δ���黹! ɾ��ʧ��!\n");
                    MyGetCh();
                    return;
                }
            }
        }
        Remove(all_books, rst);
        WriteBooks();
        ReadAll();
        MediatePrint("ɾ���ɹ�!\n");
        MyGetCh();
        return;
    }
    int del_index, index;
    if (!(std::cin >> del_index))
    {
        HighlightPrint("�������!\n");
        MyGetCh();
        return;
    }
    if ((index  = Find(all_books, del_isbn, del_index)) < 0)
    {
        HighlightPrint("�������!\n");
        MyGetCh();
        return;
    }
    Book tmp = all_books[Find(all_books, del_isbn, del_index)];
    std::vector<Info> info = tmp.info();
    for (std::vector<Info>::iterator it = info.begin(); it != info.end(); ++it)
    {
        if (it->state == HOLDING || it->state == WANTED || it->state == OVERDUE)
        {
            HighlightPrint("������δ���黹! ɾ��ʧ��!\n");
            MyGetCh();
            return;
        }
    }
    Remove(all_books, del_isbn, del_index);
    WriteBooks();
    ReadAll();
    MediatePrint("ɾ���ɹ�!\n");
    MyGetCh();
    return;
}

void Administrator::all_user()
{
    ClearScreen();
    for (std::vector<User*>::iterator it = users.begin(); it != users.end(); ++it)
        (*it)->print();
    MyGetCh();
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
    out << Encode(reader) << '\n';
    out << reader.name_ << '\n';
    out << reader.credit_ << '\n';
    out << "*\n";
    for (std::vector<Book>::const_iterator it = reader.books_.begin(); it != reader.books_.end(); ++it)
        out << *it;
    out << "#\n\n";
    return out;
}

bool Reader::change_state(Book book, int state)
{
    for (std::vector<Book>::iterator it = books_.begin(); it != books_.end(); ++it)
    {
        if (*it == book)
        {
            it->change_state(id_, state);
			update();
            return true;
        }
    }
    return false;
}

void Reader::add_book(Book book) 
{ 
	books_.push_back(book); 
}

void Reader::del_book(Book book) 
{ 
	Remove(books_, book); 
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
    MyGetCh();
    return;
}

void Reader::recommended()
{
    ClearScreen();
    std::vector<Book> recommended = HotBook(5);
    if (recommended.size() == 0)
    {
        MediatePrint("�ݳ���û���Ƽ�ʲô����Ŷ!\n");
        MyGetCh();
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
        Key ch = MyGetCh();
        if (ch == RIGHT || ch == ENTER)
        {
            ClearScreen();
            Book target = recommended[k];
            target.print();
            target.display_now_state();
            MediatePrint("����/ԤԼ����? [y/n] \n");
            Key tmp = MyGetCh();
            if (tmp == std::make_pair(1, 'y') || tmp == std::make_pair(1, 'Y'))
            {
                target.wanted(id_);
            }
			MyGetCh();
			return;
        }
        if (ch == LEFT || ch == ESCAPE)
            return;
        if (ch == UP)
        {
            if (--k < 0)
                k = book_names.size() - 1;
            continue;
        }
        if (ch == DOWN)  
        {
            if (++k > book_names.size() - 1)
                k = 0;
            continue;
        }
    }
    MyGetCh();
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
	if (can_return.size() == 0)
	{
		MediatePrint("��û����Ҫ������!\n");
		MyGetCh();
		return;
	}
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
        Key ch = MyGetCh();
        if (ch == RIGHT || ch == ENTER)
        {
            ClearScreen();
            Book target = can_return[k];
            target.print();
            target.display_now_state();
            MediatePrint("�黹����? [y/n] \n");
            Key tmp = MyGetCh();
            if (tmp == std::make_pair(1, 'y') || tmp == std::make_pair(1, 'Y'))
            {
                target.give_back(id_);
            }
			MyGetCh();
			return;
        }
        if (ch == LEFT || ch == ESCAPE)
            return;
        if (ch == UP)
        {
            if (--k < 0)
                k = book_names.size() - 1;
            continue;
        }
        if (ch == DOWN)  
        {
            if (++k > book_names.size() - 1)
                k = 0;
            continue;
        }
    }
    MyGetCh();
    return;
}

void Reader::search()
{
    ClearScreen();
    std::cout << std::setw(WIDTH / 2) << "�ؼ���/ISBN: ";
    std::vector<std::string> keywords;
    std::string tmp;
    while (std::cin >> tmp)
	{
        keywords.push_back(tmp);
		if (std::cin.get() == '\n')
			break;
	}
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
        Key ch = MyGetCh();
        if (ch == RIGHT || ch == ENTER)
        {
            ClearScreen();
            Book target = search_result[k];
            target.print();
            target.display_now_state();
            MediatePrint("����/ԤԼ����? [y/n] \n");
            Key tmp = MyGetCh();
            if (tmp == std::make_pair(1, 'y') || tmp == std::make_pair(1, 'Y'))
            {
                target.wanted(id_);
            }
			MyGetCh();
			return;
        }
        if (ch == LEFT || ch == ESCAPE)
            return;
        if (ch == UP)
        {
            if (--k < 0)
                k = book_names.size() - 1;
            continue;
        }
        if (ch == DOWN)  
        {
            if (++k > book_names.size() - 1)
                k = 0;
            continue;
        }
    }
    MyGetCh();
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