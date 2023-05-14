#include "handsdeck.h"
#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>

handsdeck::handsdeck() {}

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
    std::string figure = deck[index].getFigure();

    deck.erase(deck.begin() + index);

    return figure;
}

void handsdeck::addCard(card in)
{
    deck.push_back(in);
}

bool handsdeck::checkIsEmpty()
{
    if (deck.empty())
    {
        return true;
    }
}

card handsdeck::delete_card(char card_for_delete)
{
    if (card_for_delete == 'N')
    {
        card_for_delete = 'K';
    }

    card result;

    for (int i = 0; i < deck.size(); i++)
    {
        if (card_for_delete == deck[i].getFigure()[0])
        {
            result = deck[i];

            deck.erase(deck.begin() + i);

            /*std::cout << "Карта для удаления из колоды " << card_for_delete << std::endl;
            std::cout << "Карта, которой она типо равна " << deck[i].getFigure()[0] << std::endl;*/

            break;
        }
    }

    return result;
}

void handsdeck::appendOnBoard(std::string piece)
{

}