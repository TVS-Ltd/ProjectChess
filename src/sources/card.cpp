#include "card.h"

card::card(char fig, string icn, string pie) 
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


string card::getIcon() 
{
    return icon; 
}

void card::setIcon(string icn) 
{ 
    this->icon = icn; 
}

string card::getPiece() 
{ 
    return piece; 
}

void card::setPiece(string pie) 
{ 
    this->piece = pie; 
}