#include "handsdeck.h"

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

bool handsdeck::checkIsEmpty()
{
    if(deck.empty())
    {
        return true;
    }
}

void handsdeck::delete_card(char card_for_delete, std::stack<std::string>& input)
{
    if(card_for_delete == 'N')
    {
        card_for_delete = 'K';
    }
    
    for(int i = 0;i < deck.size(); i++)
    {
        if(card_for_delete == deck[i].getFigure()[0])
        {
            deck[i].setFigure(input.top()); 
            
            input.pop();

            break;
        }
    }    
}

bool handsdeck::checkForCard(char type)
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

int handsdeck::getSize()
{
    return deck.size();
}