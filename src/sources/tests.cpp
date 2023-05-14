#include "gtest/gtest.h"
#include "deck.h"

class DeckTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Set up the test database
        sqlite3_open(":memory:", &db_);
        // Create the necessary tables
        sqlite3_exec(db_, "CREATE TABLE DECK (type TEXT, value INTEGER, index INTEGER)", nullptr, nullptr, nullptr);
        sqlite3_exec(db_, "CREATE TABLE BIGBLACKVADIMVLADYMTSEV (type TEXT, value INTEGER, index INTEGER)", nullptr, nullptr, nullptr);
    }

    void TearDown() override {
        // Clean up the test database
        sqlite3_close(db_);
    }

    // Declare a pointer to the test database
    sqlite3* db_;
};

// // Test the setPoints function
// TEST_F(DeckTest, SetPoints) {
//     deck d;
//     d.setPoints(42);
//     EXPECT_EQ(d.getPoints(), 42);
// }

// Test the size_of_BD function
TEST_F(DeckTest, SizeOfBD) {
    deck d;
    EXPECT_EQ(d.size_of_BD(db_), 0);
    // Add a record to the DECK table
    d.appendToBD(":memory:", "Pawn", db_);
    EXPECT_EQ(d.size_of_BD(db_), 1);
}

// Test the size_of_BlackBD function
TEST_F(DeckTest, SizeOfBlackBD) {
    deck d;
    EXPECT_EQ(d.size_of_BlackBD(db_), 0);
    // Add a record to the BIGBLACKVADIMVLADYMTSEV table
    d.appendToBlackBD(":memory:", "Queen", db_);
    EXPECT_EQ(d.size_of_BlackBD(db_), 1);
}

// Test the clear_Bd function
TEST_F(DeckTest, ClearBd) {
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
