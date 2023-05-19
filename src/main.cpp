#include "game.h"

#define DEBUG false
#define TEST_WHITE_WIN false
#define TEST_PROMOTION false
#define TEST_CASTLING false
#define TEST_EN_PASSANT false
#define TEST_THREEE_POS_RULE false
#define TEST_CARD_MOVES false
#define YESNO_MENU false
#define LOG_TO_FILE false
#define LINUX false

void start()
{
    Game game;
    try
    {
        game.start();
    }
    catch (const exception& e)
    {
        cout << "Error: " << e.what() << endl;
    }
}

int main()
{
    bool exit = false;

    while (!exit)
    {
        start();

#if LINUX && YESNO_MENU
        system("printf \"\033[100m\"");
        int statusCode = system("dialog --title \"Chess\" --colors --yesno \"\\Zb\\Z1Do you want to play again ?\" 0 0 --erase-on-exit");
        system("printf \"\033[0m\"");

        if (statusCode == 256)
        {
            exit = true;
        }

        system("clear");
#else
        cout << "Do you want to play again ?" << endl;
        cout << "Enter N to exit or any other key to continue." << endl;

        char answer;
        cin >> answer;

        if (answer == 'N' || answer == 'n')
        {
            exit = true;
        }
#endif
    }

    return 0;
}