#include "card.h"
#include <vector>
#include <iterator>
#include <algorithm>
#include "position.h"
#include <stack>
#pragma once
using namespace std;

class handsdeck
{
    private:
        std::string figure;

        std::vector <card> deck;
    public:
        handsdeck();

        void setFigure(std::string in);

        std::string getFigure();
        
        void print();

        void push_b(card in)
        {
            deck.push_back(in);
        }

        std::vector <card> *getVector()
        {
            return &deck;
        }

        void CardChange(int index, card in);

        std::string getCard(int index);

        void addCard(card in);

        void appendOnBoard(std::string piece);        

        bool checkIsEmpty();

        void delete_card(char card_for_delete, std::stack<std::string>&input);

        bool checkForCard(char type);

        int getSize();

};
    