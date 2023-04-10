#include "card.h"
#include <string>

card::card(char fig, std::string icn, std::string pie) 
{
    this->figure = fig;
    this->icon = icn;
    this->piece = pie;
}   

void card::setFigure(char fig) 
{ 
    this->figure = fig; 
}

char card::getFigure() 
{ 
    return figure; 
}

void card::setIcon(std::string icn) 
{ 
    this->icon = icn; 
}

std::string card::getIcon() 
{
    return icon; 
}

void card::setPiece(std::string pie) 
{ 
    this->piece = pie; 
}

std::string card::getPiece() 
{ 
    return piece; 
}

