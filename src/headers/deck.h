#ifndef DECK_H
#define DECK_H

#include <sqlite3.h>
#include <string>
#pragma once

class deck
{
private:
    sqlite3* db;
    int points;
public:
    deck();
    void setPoints(int value);
    static int callback(void* data, int argc, char** argv, char** azColName);
    void appendToBD(const char* filename,std::string type_of_figure);
    int size_of_BD(); 
    void clear_Bd();
    ~deck();
};

#endif // DECK_H
