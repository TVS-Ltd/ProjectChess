#pragma once

#include "CoreMinimal.h"

#include "Card.h"

#include <vector>
#include <iterator>
#include <algorithm>
#include <stack>
#include <iostream>



class CHESSDEV_API Handsdeck
{
private:
    std::string figure;

    std::vector<card> deck;
public:
    Handsdeck();

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

    void delete_card(char cardToDelete, std::stack<std::string>& input);

    int32_t size() const {
        return deck.size();
    }

    bool checkForCard(char type);

    int getSize();
};