#include "handsdeck.h"
#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>

    handsdeck::handsdeck(){}
        
    void handsdeck::setFigure(std::string in)
    {
        figure = in;
    }
        
    std::string handsdeck::getFigure()
    {
        return figure;
    }
      
    void handsdeck::print()
    {
        for (size_t i = 0; i < deck.size(); i++)
        {
            std::cout << deck[i].getFigure() << ' ';
        } 
    }
        
    void handsdeck::CardChange(int index, card in)
    {
        deck[index] = in;
    }

    std::string handsdeck::getCard(int index)
    {
        deck.erase(deck.begin() + index);

        return deck[index].getFigure();
    }

    void handsdeck::addCard(card in)
    {
        deck.push_back(in);
    }

    void handsdeck::appendOnBoard(std::string piece)
    {
        
    }