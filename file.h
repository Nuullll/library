// file.h

#ifndef FILE_H
#define FILE_H 

#include <iostream>
#include <fstream>

void ReadAdmins();
void ReadAll();         // 读取所有文件中的信息
void ReadBooks();
void ReadReaders();
void ReadUsers();
void WriteAdmins();
void WriteAll();        // 将所有全局信息写入文件
void WriteBooks();
void WriteReaders();

#endif