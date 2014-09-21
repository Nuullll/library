// book.cpp

#include "book.h"

#include "global.h"
#include "file.h"
#include <conio.h>

std::ifstream &operator >>(std::ifstream &in, Info &info)
{
    time_t now = time(NULL);
    int state;
    in >> info.reader;
    in >> state;
    in >> info.start_time;
    in >> info.end_time;
    if (now > info.end_time)
    {
        switch (state)
        {
            case HOLDING: info.state = OVERDUE; break;
            case REQUEST: break; // Book::wanted()����
            case WANTED: info.state = OVERDUE; break;
            case OVERDUE: info.state = OVERDUE; break;
            case RETURNED: info.state = RETURNED; break;
        }
    }
    else
        info.state = state;
    return in;
}

std::ofstream &operator <<(std::ofstream &out, const Info &info)
{
    out << info.reader << '\n';
    out << info.state << '\n';
    out << info.start_time << '\n';
    out << info.end_time << '\n';
    return out;
}

void Info::print()
{
    std::cout << std::setw(WIDTH / 2) << "����ID: " << reader;
    switch (state)
    {
        case HOLDING:
        {
            std::cout << "  ������\n";
            break;
        }
        case REQUEST:
        {
            std::cout << "  ��ԤԼ\n";
            break;
        }
        case WANTED:
        {
            std::cout << "  ������, ��ԤԼ, ��������\n";
            break;
        }
        case OVERDUE:
        {
            std::cout << "  ����δ��, �ܵ��ͷ�\n";
            break;
        }
        case RETURNED:
        {
            std::cout << "  ������\n";
            break;
        }
        default: break;
    }
    print_duration();
    return;
}

void Info::print_duration()
{
//    struct tm * time_start, * time_end;
//    time_start = localtime(&start_time);
 //   time_end = localtime(&end_time);
    std::cout << std::setw(WIDTH / 2) << "��ʼʱ��: " << ctime(&start_time);
	std::cout << std::setw(WIDTH / 2) << "����ʱ��: " << ctime(&end_time);
    return;
}

Book::Book(std::string isbn, std::string name, std::string author, 
           std::string publish, int days): 
           isbn_(isbn), name_(name), author_(author), 
           publish_(publish), index_(0), on_shelf_(true), days_(days)
{
    for (std::vector<Book>::iterator it = all_books.begin(); it != all_books.end(); ++it)
    {
        if (it->isbn() == isbn)
            ++index_;
    }
}

std::ifstream &operator >>(std::ifstream &in, Book &book)
{
    in >> book.isbn_;
	in.get();
	getline(in, book.name_);
	getline(in, book.author_);
	getline(in, book.publish_);
    in >> book.index_;
    in >> book.on_shelf_;
    in >> book.days_;
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
                Info tmp;
                in >> tmp;
                book.info_.push_back(tmp);
            }
        }
    }
    else
        in.seekg(-1, std::ios::cur);
    return in;
}

std::ofstream &operator <<(std::ofstream &out, const Book &book)
{
    out << book.isbn_ << '\n';
    out << book.name_ << '\n';
    out << book.author_ << '\n';
    out << book.publish_ << '\n';
    out << book.index_ << '\n';
    out << book.on_shelf_ << '\n';
    out << book.days_ << '\n';
    out << "*\n";
    for (std::vector<Info>::const_iterator it = book.info_.begin(); it != book.info_.end(); ++it)
        out << *it;
    out << "#\n\n";
    return out;
}

Info &Book::info(int reader_id)
{
    for (std::vector<Info>::iterator it = info_.begin(); it != info_.end(); ++it)
    {
        if (it->reader == reader_id)
            return *it;
    }
}

std::vector<Info> Book::same_state(int state)
{
    std::vector<Info> rst;
    for (std::vector<Info>::iterator it = info_.begin(); it != info_.end(); ++it)
    {
        if (it->state == state)
            rst.push_back(*it);
    }
    return rst;
}

void Book::change_state(int reader_id, int state)
{
    info(reader_id).state = state;
    update();
}

void Book::display_now_state()
{
    for (std::vector<Info>::iterator it = info_.begin(); it != info_.end(); ++it)
        it->print();
    return;
}

void Book::give_back(int reader_id)
{
	time_t now = time(NULL);
    if (info(reader_id).state == OVERDUE)
    {
        Reader tmp = readers[Find(readers, reader_id)];
        int diff = - int(pow(2, (now - info(reader_id).end_time) / SECONDS_A_DAY));
        tmp.change_credit(diff);
		tmp.change_state(*this, RETURNED);
		info(tmp.id()).end_time = now;
        tmp.update();
		HighlightPrint("�黹�ɹ�, �ѳ�������, ���ý���!\n");
    }
    else
    {
        Reader tmp = readers[Find(readers, reader_id)];
        tmp.change_credit(1);
		tmp.change_state(*this, RETURNED);
		info(tmp.id()).end_time = now;
        tmp.update();
		HighlightPrint("��ʱ����ɹ�, �������!\n");
    }
    on_shelf_ = true;
    change_state(reader_id, RETURNED);
    return;
}

void Book::print()
{
    std::cout << std::setw(WIDTH / 2) << "ISBN: " << isbn_ << std::endl;
	std::cout << std::setw(WIDTH / 2) << "ͬ������: " << index_ << std::endl;
    std::cout << std::setw(WIDTH / 2) << "����: " << name_ << std::endl;
    std::cout << std::setw(WIDTH / 2) << "����: " << author_ << std::endl;
    std::cout << std::setw(WIDTH / 2) << "������Ϣ: " << publish_ << std::endl;
	HighlightPrint((on_shelf_ ? "�ڼ���\n" : "�ѽ��\n" ));
    return;
}

void Book::update()
{
    Remove(all_books, isbn_, index_);
    all_books.push_back(*this);
    WriteBooks();
    ReadAll();
    return;
}

void Book::wanted(int reader_id)
{
    Reader reader = readers[Find(readers, reader_id)];
    std::vector<Info> tmp = same_state(WANTED);
    time_t now = time(NULL);
    time_t start_time = now;
    time_t end_time = now + days_ * reader.credit() * SECONDS_A_DAY / CREDIT;
    if (tmp.size() != 0) // ����ԤԼ
    {
        if (tmp[0].end_time < now)
        {
            Remove(info_, tmp[0].reader);
            Remove(info_, reader_id);
            info_.push_back(Info(reader_id, HOLDING, start_time, end_time));
			on_shelf_ = false;
			reader.del_book(*this);
			reader.add_book(*this);
			reader.update();
            update();
            HighlightPrint("���ĳɹ�! \n");
            info(reader_id).print_duration();
            return;
        }
        if (tmp[0].reader == reader_id)
        {
			if (same_state(HOLDING).size() == 0)
			{
				change_state(reader_id, HOLDING);
				info(reader_id).start_time = start_time;
				info(reader_id).end_time = end_time;
				on_shelf_ = false;
				reader.del_book(*this);
				reader.add_book(*this);
				reader.update();
				update();
				HighlightPrint("������ǰԤԼ, ���ĳɹ�!\n");
				info(reader_id).print_duration();
				return;
			}
			HighlightPrint("����ԤԼ��, �����ظ�ԤԼ!\n");
			return;
        }
        HighlightPrint("ԤԼʧ��, ������ԤԼ!\n");
        return;
    }
    tmp = same_state(OVERDUE);
    if (tmp.size() != 0)
    {
        if (tmp[0].reader == reader_id)
        {
            HighlightPrint("�뾡��黹����!\n");
            return;
        }
        Remove(info_, reader_id);
        info_.push_back(Info(reader_id, REQUEST, start_time, start_time + 3 * SECONDS_A_DAY));
		reader.del_book(*this);
		reader.add_book(*this);
		reader.update();
		update();
        HighlightPrint("ԤԼ�ɹ�, ������һλ����������δ��, ���Ӽ�����ӵ������Ĵ������Ƚ���Ȩ\n");
        info(reader_id).print_duration();
        return;
    }
    tmp = same_state(HOLDING);
    if (tmp.size() != 0)
    {
        if (tmp[0].reader == reader_id)
        {
            HighlightPrint("�����ڽ��Ĵ���!\n");
            return;
        }
        info(tmp[0].reader).state = WANTED;
        info(tmp[0].reader).end_time = (now + info(tmp[0].reader).end_time) / 2;
        Remove(info_, reader_id);
		info_.push_back(Info(reader_id, REQUEST, start_time, tmp[0].end_time + 3 * SECONDS_A_DAY));
		reader.del_book(*this);
		reader.add_book(*this);
		reader.update();
		update();
        HighlightPrint("ԤԼ�ɹ�, ������λ���߻�������ӵ������Ĵ������Ƚ���Ȩ\n");
        info(reader_id).print_duration();
        return;
    }
    Remove(info_, reader_id);
    info_.push_back(Info(reader_id, HOLDING, start_time, end_time));
	on_shelf_ = false;
	reader.del_book(*this);
	reader.add_book(*this);
	reader.update();
    update();
    HighlightPrint("���ĳɹ�! \n");
    info(reader_id).print_duration();
    return;
}