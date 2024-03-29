#include "card.h"

#include <vector>
#include <iterator>
#include <algorithm>
#include <stack>
#include <iostream>

#pragma once

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

    std::vector <card>* getVector()
    {
        return &deck;
    }

    void CardChange(int index, card in);

    std::string getCard(int index);

    void addCard(card in);

    void appendOnBoard(std::string piece);

    bool checkIsEmpty();

    void delete_card(char card_for_delete, std::stack<std::string>& input);

    int32_t size() const {
        return deck.size();
    }

    bool checkForCard(char type);

    int getSize();
};
