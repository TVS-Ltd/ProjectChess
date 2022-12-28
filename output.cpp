#include <iostream>

using namespace std;

void figuresPrinter()
{
    //White Set
    cout << "♕" << endl; //White Queen
    cout << "♔" << endl; //White King
    cout << "♘" << endl; //White Knight
    cout << "♖" << endl; //White Rook
    cout << "♙" << endl; //White Bishop
    cout << "♙" << endl; //White Pawn

    //Black Set
    cout << "♛" << endl; //Black Queen
    cout << "♚" << endl; //Black King
    cout << "♞" << endl; //Black Knight
    cout << "♜" << endl; //Black Rook
    cout << "♝" << endl; //Black Bishop
    cout << "♟" << endl; //Black Pawn
}

int main()
{
    figuresPrinter();

    return 0;
}