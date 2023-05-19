#ifndef DECK_H
#define DECK_H

#include <sqlite3.h>
#include <string>
#pragma once

class deck
{
private:

    int points;

public:
 
    deck();
 
    void setPoints(int value);
 
    static int callback(void* data, int argc, char** argv, char** azColName);
 
    void appendToBD(const char* filename,std::string type_of_figure,sqlite3 *db);

    void appendToBlackBD(const char* filename, std::string type_of_figure,sqlite3 *db);

    int size_of_BD(sqlite3 *db); 

    int size_of_BlackBD(sqlite3 *db);
 
    void clear_Bd(sqlite3* db);
 
    void shuffle(sqlite3 *db);

    void blackShuffle(sqlite3 *db);
 
    void print_data_base(sqlite3_stmt *stmt);
 
    std::string get_random_value(sqlite3* db);

    std::string get_random_BlackValue(sqlite3* db);

    ~deck();
};

#endif // DECK_H
