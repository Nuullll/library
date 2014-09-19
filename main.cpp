// main.cpp

#include "global.h"
#include "menu.h"
#include "file.h"
#include <map>

int main(int argc, char const *argv[])
{
login:                          // 登录标签
    ClearScreen();
    ReadAll();
    Welcome();
    Token tmp;
    User* user;
    while (true)
    {
        tmp = Login();
        if (tmp.auth() != -1)
        {
            user = users[Find(users, tmp.id())];
            printf("%44s\n", "登录成功!");
            printf("%40s", "欢迎 ");
            user->print();
            getch();
            break;
        }
        else
        {
            ClearScreen();
            printf("%56s\n", "学号(工号)或密码错误! 重新输入? ");
            printf("%50s\n", "[回车继续, q退出系统] ");
            char tmp = getch();
            if (tmp == 'q' || tmp == 'Q')
            {
                HighlightPrint("%41s\n", "再见!");
                return;
            }    
            ClearScreen();
        }
    }
    switch (user->identity())
    {
        // 管理员菜单
        case ADMINISTRATOR:
        {
            Administrator admin = admins[Find(admins, tmp.id())];
            MenuNode top_menu, user_menu, book_menu;
            top_menu.previous = user_menu.previous = book_menu.previous = &top_menu;
            top_menu.append(Option("用户管理", &user_menu));
            top_menu.append(Option("书籍管理", &book_menu));
            top_menu.append(Option("修改密码", NULL));
            top_menu.append(Option("退出登录", NULL));
            top_menu.append(Option("退出系统", NULL));
            user_menu.append(Option("所有用户", NULL));
            user_menu.append(Option("注册新用户", NULL));
            user_menu.append(Option("删除用户", NULL));
            book_menu.append(Option("所有书籍", NULL));
            book_menu.append(Option("新书上架", NULL));
            book_menu.append(Option("旧书下架", NULL));
            MenuNode now = top_menu;
            int k = 0;
            while (true)
            {
                now.show(k);
                int ch = getch();
                if (ch == 13)
                    goto admin_right;
                if (ch == 27)
                    goto admin_left;
                while (ch != 224)
                    ;
                switch (getch())
                {
                    case UP:
                    {
                        if (--k < 0)
                            k = now.options.size() - 1;
                        break;
                    }
                    case DOWN:
                    {
                        if (++k > now.options.size() - 1)
                            k = 0;
                        break;
                    }
                    case LEFT:
                    {
                        admin_left:
                        now = *now.previous;
                        k = 0;
                        break;
                    }
                    case RIGHT:
                    {
                        admin_right:
                        if (now.options[k].second == NULL)
                        {
                            std::map<std::string, int> mp;
                            mp["修改密码"] = 0;
                            mp["退出登录"] = 1;
                            mp["退出系统"] = 2;
                            mp["所有用户"] = 3;
                            mp["注册新用户"] = 4;
                            mp["删除用户"] = 5;
                            mp["所有书籍"] = 6;
                            mp["新书上架"] = 7;
                            mp["旧书下架"] = 8;
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
                            break;
                        }
                        else
                        {
                            now = *now.options[k].second;
                            k = 0;
                            break;
                        }
                    }
                }
            }
        }
        // 读者菜单
        case READER:
        {
            Reader reader = readers[Find(readers, tmp.id())];
            MenuNode top_menu, checkout_menu;
            top_menu.previous = checkout_menu.previous = &top_menu;
            top_menu.append(Option("借阅书籍", &checkout_menu));
            top_menu.append(Option("自助还书", NULL));
            top_menu.append(Option("借阅历史", NULL));
            top_menu.append(Option("修改密码", NULL));
            top_menu.append(Option("退出登录", NULL));
            top_menu.append(Option("退出系统", NULL));
            checkout_menu.append(Option("查询书籍", NULL));
            checkout_menu.append(Option("馆长推荐", NULL));
            checkout_menu.append(Option("电子资源", NULL));
            MenuNode now = top_menu;
            int k = 0;
            while (true)
            {
                now.show(k);
                int ch = getch();
                if (ch == 13)
                    goto reader_right;
                if (ch == 27)
                    goto reader_left;
                while (ch != 224)
                    ;
                switch (getch())
                {
                    case UP:
                    {
                        if (--k < 0)
                            k = now.options.size() - 1;
                        break;
                    }
                    case DOWN:
                    {
                        if (++k > now.options.size() - 1)
                            k = 0;
                        break;
                    }
                    case LEFT:
                    {
                        reader_left:
                        now = *now.previous;
                        k = 0;
                        break;
                    }
                    case RIGHT:
                    {
                        reader_right:
                        if (now.options[k].second == NULL)
                        {
                            std::map<std::string, int> mp;
                            mp["自助还书"] = 0;
                            mp["借阅历史"] = 1;
                            mp["修改密码"] = 2;
                            mp["退出登录"] = 3;
                            mp["退出系统"] = 4;
                            mp["查询书籍"] = 5;
                            mp["馆长推荐"] = 6;
                            mp["电子资源"] = 7;
                            switch (mp[now.options[k].first])
                            {
                                case 0: reader.return(); break;
                                case 1: reader.history(); break;
                                case 2: reader.set_password(); break;
                                case 3: goto login;
                                case 4: Exit(); break;
                                case 5: reader.search(); break;
                                case 6: reader.recommended(); break;
                                case 7: EBook(); break;
                            }
                            k = 0;
                            break;
                        }
                        else
                        {
                            now = *now.options[k].second;
                            k = 0;
                            break;
                        }
                    }
                }
            }
        }
    }
    return 0;
}