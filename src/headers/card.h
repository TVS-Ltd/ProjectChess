#ifndef CARD_H
#define CARD_H

#include <string>

#pragma once

class card
{
    private:
        char figure;
        std::string icon;
        std::string piece;
    public:
    // Constructor
    card(char fig, std::string icn, std::string pie);
    
    // Getters and setters
    char getFigure();
    void setFigure(char fig);

    std::string getIcon();
    void setIcon(std::string icn);

    std::string getPiece();
    void setPiece(std::string pie);

    

};

#endif // CARD_H
