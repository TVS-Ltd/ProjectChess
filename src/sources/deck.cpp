#include "deck.h"
#include <vector>
#include <string>
#include <bits/stdc++.h>

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

void deck::appendToBD(const char* filename, std::string type_of_figure,sqlite3 *db)    
{
      char* errMsg = nullptr;
    std::string value; 
    std::string index;

    sqlite3_open(filename, &db);

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
    
    std::string sql = "INSERT INTO DECK (type, value, inndex) VALUES ('" + type_of_figure + "', '" + value + "', '" + index + "');";
    int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
    char* err_msg = "BD is full";
    if (rc != SQLITE_OK) 
    {
        printf("SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
    } 
    else 
    {
        printf("Record added successfully!\n");
    }

    if (size_of_BD(db) > 15)
    {
        printf("BD is full\n");
        sqlite3_free(err_msg);
    }
    
    sqlite3_close(db);
}

int deck::size_of_BD(sqlite3 *db)
{
    sqlite3_open("/home/danila/CCG/ProjectChess/src/DataBase/DB.db", &db); // открытие базы данных

    char* errMsg = 0;

    int count = 0;

    int size = sqlite3_exec(db, "SELECT COUNT(*) FROM DECK", callback, &count, &errMsg);

    sqlite3_close(db);

    return size;
} 

void deck::clear_Bd(sqlite3* db)
{
    // const char* sql = "DELETE FROM DECK";
    // char* err_msg = nullptr;
    // int rc = sqlite3_exec(db, sql, nullptr, nullptr, &err_msg);
    // if (rc != SQLITE_OK) 
    // {
    //     sqlite3_free(err_msg);
    // }
    char* error_message = nullptr;
    int result = SQLITE_OK;

    // Получаем список таблиц в базе данных
    sqlite3_stmt* stmt;
    const char* query = "SELECT name FROM sqlite_master WHERE type = 'table'";
    result = sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);
    if (result != SQLITE_OK) {
        printf("Error preparing statement: %s\n", sqlite3_errmsg(db));
        return;
    }

    // Очищаем каждую таблицу в базе данных
    while (sqlite3_step(stmt) == SQLITE_ROW) 
    {
        const char* table_name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        std::string delete_query = "DELETE FROM " + std::string(table_name);
        result = sqlite3_exec(db, delete_query.c_str(), nullptr, nullptr, &error_message);
        if (result != SQLITE_OK) {
            printf("Error clearing table %s: %s\n", table_name, error_message);
            sqlite3_free(error_message);
            continue;
        }
        printf("\nCleared table %s\n", table_name);
    }

    sqlite3_finalize(stmt);

    printf("Database cleared successfully!\n");
}

void deck::shuffle(sqlite3 *db)
{
   const char* tableName = "DECK";
   
   // Получаем количество строк в таблице
    std::string sql = "SELECT COUNT(*) FROM " + std::string(tableName) + ";";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_step(stmt);
    int rowCount = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt);

    // Получаем случайный порядок индексов строк
    std::vector<int> rowIndices;
    for (int i = 1; i <= rowCount; i++) {
        rowIndices.push_back(i);
    }
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(rowIndices.begin(), rowIndices.end(), g);

    // Меняем порядок строк в таблице
    sql = "BEGIN TRANSACTION;";
    sqlite3_exec(db, sql.c_str(), nullptr, nullptr, nullptr);
    for (int i = 0; i < rowCount; i++) {
        int fromIndex = rowIndices[i];
        int toIndex = i + 1;
        sql = "UPDATE " + std::string(tableName) + " SET rowid = -1 WHERE rowid = " + std::to_string(fromIndex) + ";";
        sqlite3_exec(db, sql.c_str(), nullptr, nullptr, nullptr);
        sql = "UPDATE " + std::string(tableName) + " SET rowid = " + std::to_string(fromIndex) + " WHERE rowid = " + std::to_string(toIndex) + ";";
        sqlite3_exec(db, sql.c_str(), nullptr, nullptr, nullptr);
        sql = "UPDATE " + std::string(tableName) + " SET rowid = " + std::to_string(toIndex) + " WHERE rowid = -1;";
        sqlite3_exec(db, sql.c_str(), nullptr, nullptr, nullptr);
    }
    sql = "END TRANSACTION;";
    sqlite3_exec(db, sql.c_str(), nullptr, nullptr, nullptr);

}

void deck::print_data_base(sqlite3_stmt *stmt)
{
    while (sqlite3_step(stmt) == SQLITE_ROW) 
    {
        int column_count = sqlite3_column_count(stmt);

        for (int i = 0; i < column_count; i++) 
        {
            printf("%s\t", sqlite3_column_text(stmt, i));
        }

        printf("\n");
    }

    sqlite3_finalize(stmt);
}

std::string deck::get_random_value(sqlite3* db)
{
    // Получаем количество строк в таблице
    sqlite3_stmt* stmt;
    std::string sql = "SELECT COUNT(*) FROM DECK;";
    
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
    int count = 0;
    
    if (rc == SQLITE_OK)
    {
        rc = sqlite3_step(stmt);
        if (rc == SQLITE_ROW)
        {
            count = sqlite3_column_int(stmt, 0);
        }
    }
    
    sqlite3_finalize(stmt);

    // Получаем значение случайной строки начиная с 6-й строки
    static std::vector<std::string> used_values;
    std::string value;
    if (count > 5)
    {        
    // Получаем значение случайной строки начиная с 6-й строки
    
        do {
            int random_index = rand() % (count - 5) + 5; // вычитаем 5, чтобы начать со 6-й строки
        
            sql = "SELECT * FROM DECK LIMIT 1 OFFSET " + std::to_string(random_index) + ";";
            rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
        
            if (rc == SQLITE_OK)
            {
                rc = sqlite3_step(stmt);
        
                if (rc == SQLITE_ROW)
                {
                    value = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
                }
            }
         
            sqlite3_finalize(stmt);
        
        } while (std::find(used_values.begin(), used_values.end(), value) != used_values.end());
        
        used_values.push_back(value);
    }

    return value;
}

    



deck::~deck()
{

}