#include "Handsdeck.h"

Handsdeck::Handsdeck() {}
    
void Handsdeck::setFigure(std::string in)
{
    figure = in;
}
    
std::string Handsdeck::getFigure()
{
    return figure;
}
    
void Handsdeck::print()
{
    for (size_t i = 0; i < deck.size(); i++)
    {
        // std::cout << deck[i].getFigure() << ' ';
    } 
}
    
void Handsdeck::CardChange(int index, card in)
{
    deck[index] = in;
}

std::string Handsdeck::getCard(int index)
{
    deck.erase(deck.begin() + index);

    return deck[index].getFigure();
}

void Handsdeck::addCard(card in)
{
    deck.push_back(in);
}

bool Handsdeck::checkIsEmpty()
{
    if (deck.empty())
    {
        return true;
    }

    return false;
}

void Handsdeck::delete_card(char cardToDelete, std::stack<std::string>& input)
{
    if (cardToDelete == 'N')
    {
        cardToDelete = 'K';
    }
    
    for (int i = 0;i < deck.size(); i++)
    {
        if (cardToDelete == deck[i].getFigure()[0])
        {
            deck[i].setFigure(input.top()); 
            
            input.pop();

            break;
        }
    }    
}

bool Handsdeck::checkForCard(char type)
{
    if (type == 'N') type = 'K';
    
    for (int i = 0;i < deck.size();i++)
    {
        if(toupper(type) == toupper(deck[i].getFigure()[0]))
        {
            return true;
        }
    }

    return false;
}

int Handsdeck::getSize()
{
    return deck.size();
}