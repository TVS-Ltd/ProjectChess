#ifndef CARD_H
#define CARD_H

#include <string>

#pragma once

class card
{
    private:
        std::string figure;
        std::string icon;
        std::string piece;
    public:
    // Constructor
    card(std::string fig, std::string icn, std::string pie);
    
    // Getters and setters
    std::string getFigure();
    void setFigure(std::string fig);

    std::string getIcon();
    void setIcon(std::string icn);

    std::string getPiece();
    void setPiece(std::string pie);

    

};

#endif // CARD_H
