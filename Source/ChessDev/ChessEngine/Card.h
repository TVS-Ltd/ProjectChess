#ifndef CARD_H
#define CARD_H

#pragma once

#include "CoreMinimal.h"

#include <string>

class CHESSDEV_API card
{
private:
    std::string figure;
    std::string icon;
    std::string piece;
public:
    card() = default;

    // Constructor
    card(std::string fig, std::string icn, std::string pie);

    // Getters and setters
    std::string getFigure();
    void setFigure(std::string fig);

    std::string getIcon();
    void setIcon(std::string icn);

    std::string getPiece();
    void setPiece(std::string pie);

    bool operator <(const card& card) 
    {
        return (figure.size() < card.figure.size() || (figure.size() == card.figure.size() && figure < card.figure)) && card.figure != "Queen";
    }

    bool operator <=(const card& card) 
    {
        return ((figure.size() < card.figure.size() && card.figure != "Queen") || (figure.size() == card.figure.size() && figure <= card.figure));
    }

};

#endif // CARD_H