// token.h

#ifndef TOKEN_H
#define TOKEN_H 

#include <string>
#include <vector>

class Token
{
public:
    Token() {}
    Token(int id, std::string password): id_(id), password_(password) {}
    ~Token() {}

    int id() { return id_; }
    int auth();         // 返回认证身份, 认证失败返回-1.

private:
    int id_;
    std::string password_;
};

#endif