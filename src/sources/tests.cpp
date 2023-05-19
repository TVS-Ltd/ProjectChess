#include <limits.h>
#include "game.h"
#include "deck.h"
#include "card.h"
#include "position.h"
#include "handsdeck.h"
#include <sqlite3.h>
#include "gtest/gtest.h"
namespace
{
    sqlite3 *db;    
    
    TEST(deckTest, callbackTest)
    {
        deck myDeck;
        int count = 0;
        char** argv = new char*[1];
        argv[0] = "42";
        int result = myDeck.callback(&count, 1, argv, nullptr);
        EXPECT_EQ(42, count);
        EXPECT_EQ(42, result);
    }

    TEST(deckTest, appendToBDTest)
    {
        deck myDeck;
        myDeck.appendToBD("/home/danila/CCG/ProjectChess/src/DataBase/DB.db", "Queen", db);
        int size = myDeck.size_of_BD(db);
        EXPECT_EQ(4, size);
    }

    TEST(deckTest, size_of_BDTest)
    {
        deck myDeck;
        myDeck.appendToBD("/home/danila/CCG/ProjectChess/src/DataBase/DB.db", "Queen", db);
        int size = myDeck.size_of_BD(db);
        EXPECT_EQ(4, size);
    }


    TEST(deckTest, clear_BdTest)
    {
        deck myDeck;
        myDeck.appendToBD("/home/danila/CCG/ProjectChess/src/DataBase/DB.db", "Queen", db);
        myDeck.appendToBD("/home/danila/CCG/ProjectChess/src/DataBase/DB.db", "Rook", db);
        int size = myDeck.size_of_BD(db);
        EXPECT_EQ(4, size);
        myDeck.clear_Bd(db);
        size = myDeck.size_of_BD(db);
        EXPECT_EQ(4, size);
        myDeck.clear_Bd(db);
    }
    
    TEST(fig__Test, appendOnBoardTEST)
    {

    }
   
    TEST(HandsdeckTest, TestCheckForCard) 
    {
        handsdeck deck;
        card Bis("bishop","sdf","dsf");
        card Que("queen","sdf","dsf");
        deck.addCard(Bis);
        deck.addCard(Que);

        EXPECT_EQ(deck.checkForCard('b'), true);
        EXPECT_EQ(deck.checkForCard('q'), true);
        EXPECT_EQ(deck.checkForCard('r'), false);
    }

    TEST(HandsdeckTest2, TestCheckForCard2) 
    {
        handsdeck deck;
        card Paw("pawn","sdf","dsf");
        card Kni("Knight","sdf","dsf");
        deck.addCard(Paw);
        deck.addCard(Kni);

        EXPECT_EQ(deck.checkForCard('K'), true);
        EXPECT_EQ(deck.checkForCard('P'), true);
        EXPECT_EQ(deck.checkForCard('r'), false);
    }
}














/*#include "gtest/gtest.h"
#include "deck.h"

class DeckTest : public ::testing::Test 
{
 public:
    void SetUp() override 
    {
        // Set up the test database
        sqlite3_open(":memory:", &db_);
        // Create the necessary tables
        sqlite3_exec(db_, "CREATE TABLE DECK (type TEXT, value INTEGER, index INTEGER)", nullptr, nullptr, nullptr);
        sqlite3_exec(db_, "CREATE TABLE BIGBLACKVADIMVLADYMTSEV (type TEXT, value INTEGER, index INTEGER)", nullptr, nullptr, nullptr);
    }

    void TearDown() override 
    {
        // Clean up the test database
        sqlite3_close(db_);
    }

        // Declare a pointer to the test database
        sqlite3* db_;
    };

    // Test the size_of_BD function
    TEST_F(DeckTest, SizeOfBD) 
    {
        deck d;
        EXPECT_EQ(d.size_of_BD(db_), 0);
        // Add a record to the DECK table
        d.appendToBD(":memory:", "Pawn", db_);
        EXPECT_EQ(d.size_of_BD(db_), 1);
    }

    // Test the size_of_BlackBD function
    TEST_F(DeckTest, SizeOfBlackBD) 
    {
        deck d;
        EXPECT_EQ(d.size_of_BlackBD(db_), 0);
        // Add a record to the BIGBLACKVADIMVLADYMTSEV table
        d.appendToBlackBD(":memory:", "Queen", db_);
        EXPECT_EQ(d.size_of_BlackBD(db_), 1);
    }

    // Test the clear_Bd function
    TEST_F(DeckTest, ClearBd) 
    {
        deck d;
        // Add some records to the DECK and BIGBLACKVADIMVLADYMTSEV tables
        d.appendToBD(":memory:", "Pawn", db_);
        d.appendToBD(":memory:", "Queen", db_);
        d.appendToBlackBD(":memory:", "Rook", db_);
        EXPECT_EQ(d.size_of_BD(db_), 2);
        EXPECT_EQ(d.size_of_BlackBD(db_), 1);
        // Clear the database
        d.clear_Bd(db_);
        EXPECT_EQ(d.size_of_BD(db_), 0);
        EXPECT_EQ(d.size_of_BlackBD(db_), 0);
    }
*/