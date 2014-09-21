// main.cpp

#include "global.h"
#include "token.h"
#include "menu.h"
#include "file.h"
#include <conio.h>
#include <map>

int main(int argc, char const *argv[])
{
login:                          // ��¼��ǩ
    ClearScreen();
    ReadAll();
    Token tmp;
    User* user;
    while (true)
    {
        tmp = Login();
        if (tmp.auth() != -1)
        {
            user = users[Find(users, tmp.id())];
            MediatePrint("��¼�ɹ�!\n");
            MediatePrint("��ӭ \n");
            user->print();
            MyGetCh();
            break;
        }
        else
        {
            ClearScreen();
            MediatePrint("ѧ��(����)���������! ��������? \n");
            MediatePrint("[�س�����, q�˳�ϵͳ] ");
            Key tmp = MyGetCh();
            if (tmp == std::make_pair(1, 'q') || tmp == std::make_pair(1, 'Q'))
            {
                HighlightPrint("\n�ټ�!\n");
                return 0;
            }    
            ClearScreen();
        }
    }
	ClearScreen();
    switch (user->identity())
    {
        // ����Ա�˵�
        case ADMINISTRATOR:
        {
            Administrator admin = admins[Find(admins, tmp.id())];
            MenuNode top_menu, user_menu, book_menu;
            top_menu.previous = user_menu.previous = book_menu.previous = &top_menu;
            top_menu.append(Option("�û�����", &user_menu));
            top_menu.append(Option("�鼮����", &book_menu));
            top_menu.append(Option("�޸�����", NULL));
            top_menu.append(Option("�˳���¼", NULL));
            top_menu.append(Option("�˳�ϵͳ", NULL));
            user_menu.append(Option("�����û�", NULL));
            user_menu.append(Option("ע�����û�", NULL));
            user_menu.append(Option("ɾ���û�", NULL));
            book_menu.append(Option("�����鼮", NULL));
            book_menu.append(Option("�����ϼ�", NULL));
            book_menu.append(Option("�����¼�", NULL));
            MenuNode now = top_menu;
            int k = 0;
            while (true)
            {
                now.show(k);
                Key ch = MyGetCh();
                if (ch == UP)
                {
                    if (--k < 0)
                        k = now.options.size() - 1;
                    continue;
                }
                if (ch == DOWN)
                {
                    if (++k > now.options.size() - 1)
                        k = 0;
                    continue;
                }
                if (ch == LEFT || ch == ESCAPE)
                {
                    now = *now.previous;
                    k = 0;
                    continue;
                }
                if (ch == RIGHT || ch == ENTER)
                {
                    if (now.options[k].second == NULL)
                    {
                        std::map<std::string, int> mp;
                        mp["�޸�����"] = 0;
                        mp["�˳���¼"] = 1;
                        mp["�˳�ϵͳ"] = 2;
                        mp["�����û�"] = 3;
                        mp["ע�����û�"] = 4;
                        mp["ɾ���û�"] = 5;
                        mp["�����鼮"] = 6;
                        mp["�����ϼ�"] = 7;
                        mp["�����¼�"] = 8;
                        switch (mp[now.options[k].first])
                        {
                            case 0: admin.set_password(); break;
                            case 1: goto login;
                            case 2: Exit(); break;
                            case 3: admin.all_user(); break;
                            case 4: admin.add_user(); break;
                            case 5: admin.del_user(); break;
                            case 6: admin.all_book(); break;
                            case 7: admin.add_book(); break;
                            case 8: admin.del_book(); break;
                        }
                        k = 0;
                        continue;
                    }
                    else
                    {
                        now = *now.options[k].second;
                        k = 0;
                        continue;
                    }
                }
            }
        }
        // ���߲˵�
        case READER:
        {
            Reader reader = readers[Find(readers, tmp.id())];
            MenuNode top_menu, checkout_menu;
            top_menu.previous = checkout_menu.previous = &top_menu;
            top_menu.append(Option("�����鼮", &checkout_menu));
            top_menu.append(Option("��������", NULL));
            top_menu.append(Option("������ʷ", NULL));
            top_menu.append(Option("�޸�����", NULL));
            top_menu.append(Option("�˳���¼", NULL));
            top_menu.append(Option("�˳�ϵͳ", NULL));
            checkout_menu.append(Option("��ѯ�鼮", NULL));
            checkout_menu.append(Option("�ݳ��Ƽ�", NULL));
            checkout_menu.append(Option("������Դ", NULL));
            MenuNode now = top_menu;
            int k = 0;
            while (true)
            {
                now.show(k);
                Key ch = MyGetCh();
                if (ch == UP)
                {
                    if (--k < 0)
                        k = now.options.size() - 1;
                    continue;
                }
                if (ch == DOWN)
                {
                    if (++k > now.options.size() - 1)
                        k = 0;
                    continue;
                }
                if (ch == LEFT || ch == ESCAPE)
                {
                    now = *now.previous;
                    k = 0;
                    continue;
                }
                if (ch == RIGHT || ch == ENTER)
                {
                    if (now.options[k].second == NULL)
                    {
                        std::map<std::string, int> mp;
                        mp["��������"] = 0;
                        mp["������ʷ"] = 1;
                        mp["�޸�����"] = 2;
                        mp["�˳���¼"] = 3;
                        mp["�˳�ϵͳ"] = 4;
                        mp["��ѯ�鼮"] = 5;
                        mp["�ݳ��Ƽ�"] = 6;
                        mp["������Դ"] = 7;
                        switch (mp[now.options[k].first])
                        {
                            case 0: reader.give_back(); break;
                            case 1: reader.history(); break;
                            case 2: reader.set_password(); break;
                            case 3: goto login;
                            case 4: Exit(); break;
                            case 5: reader.search(); break;
                            case 6: reader.recommended(); break;
                            case 7: EBook(); break;
                        }
                        k = 0;
                        continue;
                    }
                    else
                    {
                        now = *now.options[k].second;
                        k = 0;
                        continue;
                    }
                }
            }
        }
    }
    return 0;
}