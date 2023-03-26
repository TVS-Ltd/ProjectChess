#include "handsdeck.h"
#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>

    handsdeck::handsdeck(){}
        
    void handsdeck::setFigure(char in)
    {
        figure = in;
    }
        
    char handsdeck::getFigure()
    {
        return figure;
    }
      
    void handsdeck::print()
    {
        for (int i = 0;i < deck.size();i++)
        {
            std::cout << deck[i].getFigure() << ' ';
        } 
    }
        
    void handsdeck::CardChange(int index, card in)
    {
        deck[index] = in;
    }

    char handsdeck::getCard(int index)
    {
        deck.erase(deck.begin() + index);

        return deck[index].getFigure();
    }

    void handsdeck::addCard(card in)
    {
        deck.push_back(in);
    }