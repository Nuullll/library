// token.cpp

#include "token.h"
#include "global.h"

int Token::auth()
{
	int index = Find(users, id_);
	if (index < 0)
		return -1;
    User* tmp = users[Find(users, id_)];
    if (password_ == tmp->password_)
        return tmp->identity_;
    return -1;
}