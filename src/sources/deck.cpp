#include "deck.h"

deck::deck(/* args */)
{
}

void deck::setPoints(int value)
{
    this->points = value;
}

int deck::callback(void* data, int argc, char** argv, char** azColName)
{
    int* count = static_cast<int*>(data);
    *count = atoi(argv[0]);

    return *count;
}

void deck::appendToBD(const char* filename,std::string type_of_figure)    
{
    int border = 0;
    char* error_message = nullptr;
    std::string value; 
    std::string index;

    if(type_of_figure == "Queen")
    {
        value = "4";
        //index;
    }else
    if(type_of_figure == "Rook")
    {
        value = "3";
        //index;
    }else
    if(type_of_figure == "Knight")
    {
        value = "1";
        //index;
    }else
    if(type_of_figure == "Pawn")
    {
        value = "0";
        //index;
    }
    
    sqlite3_open("DB.db", &db);

    // int size = SELECT COUNT(*) FROM DECK;

    std::string sql = "INSERT INTO CARDDECK (type, value, inndex) VALUES (" + type_of_figure + "," + value + "," + index + ");";
    
    // Чтобы выполнить этот запрос в C++ с помощью библиотеки SQLite,
    // можно использовать функцию sqlite3_exec(), передав ей указатель на базу данных,
    // строку с SQL-запросом и коллбэк-функцию для обработки
    
    if (size_of_BD() > 15)
    {
        throw "Deck is full";
    }else
    {
        sqlite3_exec(db, sql.c_str(), nullptr, 0, &error_message);
    }
    

    sqlite3_close(db);

    points -= border;
}

int deck::size_of_BD()
{
    sqlite3_open("DB.db", &db); // открытие базы данных

     char* errMsg = 0;

    int count = 0;

    int size = sqlite3_exec(db, "SELECT COUNT(*) FROM CARDDECK", callback, &count, &errMsg);

    sqlite3_close(db);

    return size;

} 

void deck::clear_Bd()
{
    const char* sql = "DELETE FROM DECK";
    char* err_msg = nullptr;
    int rc = sqlite3_exec(db, sql, nullptr, nullptr, &err_msg);
    if (rc != SQLITE_OK) 
    {
            sqlite3_free(err_msg);
    }

}

deck::~deck()
{

}