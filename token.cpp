// token.cpp

#include "token.h"
#include "global.h"

int Token::auth()
{
    User* tmp = users[Find(users, id_)];
    if (password_ == tmp.password_)
        return tmp.identity_;
    return -1;
}