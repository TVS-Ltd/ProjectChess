#include "card.h"
#include <vector>
#include <iterator>
#include <algorithm>
class handsdeck
{
    private:
        char figure;

        std::vector <card> deck;
    public:
        handsdeck();

        void setFigure(char in);

        char getFigure();
        
        void print();

        void push_b(card in)
        {
            deck.push_back(in);
        }

        std::vector <card> *getVector()
        {
            return &deck;
        }

        void CardChange(int index, card in);

        char getCard(int index);

        void addCard(card in);

};
    // void Gwent()
    // {

    //     sideChoose();

    //     std::string str = "prkbq"; // p - pawn r - rock etc

    //     class Deck
    //     {
    //     private:
    //         char figure;

    //     public:
    //         Deck(char figure)
    //         {
    //             this->figure = figure;
    //         }

    //         void setFigure(char in)
    //         {
    //             figure = in;
    //         }

    //         char getFigure()
    //         {
    //             return figure;
    //         }

    //         void print()
    //         {
    //             cout << getFigure() << " ";
    //         }
    //     };

        

    //     std::vector<Deck> deck; // Вектор объектов

    //     cout << "Your deck" << endl;

    //     Deck obj_1(str[RandFigure()]); // Создаем 5 карт с рандомными фигурами
    //     Deck obj_2(str[RandFigure()]);
    //     Deck obj_3(str[RandFigure()]);
    //     Deck obj_4(str[RandFigure()]);
    //     Deck obj_5(str[RandFigure()]); 

    //     deck.push_back(obj_1); // Кладем их в вектор
    //     deck.push_back(obj_2);
    //     deck.push_back(obj_3);
    //     deck.push_back(obj_4);
    //     deck.push_back(obj_5);

    //     for (int i = 0; i < 5; i++) // Выводим на экран колоду для пользователя
    //     {
    //         deck[i].print();
    //     }

    //     cout << endl;

    //     cout << "You can make two substitutions. Enter the card number (1-5) you want to replace. Enter 0 if you don't want to make substitutions at all." << endl;

    //     int t = 2;

    //     while (t != 0) // Две замены карты
    //     {
    //         int choice;

    //         cin >> choice;

    //         if (choice == 0)
    //         {
    //             break;
    //         }
    //         else if (choice == 1)
    //         {
    //             obj_1.setFigure(str[RandFigure()]);

    //             deck[0] = obj_1;
    //         }
    //         else if (choice == 2)
    //         {
    //             obj_2.setFigure(str[RandFigure()]);

    //             deck[1] = obj_2;
    //         }
    //         else if (choice == 3)
    //         {
    //             obj_3.setFigure(str[RandFigure()]);

    //             deck[2] = obj_3;
    //         }
    //         else if (choice == 4)
    //         {
    //             obj_4.setFigure(str[RandFigure()]);

    //             deck[3] = obj_4;
    //         }
    //         else if (choice == 5)
    //         {
    //             obj_5.setFigure(str[RandFigure()]);

    //             deck[4] = obj_5;
    //         }
    //         else if (choice > 5 || choice < 0 || double(choice))
    //         {
    //             cout << "Incorrect input. Please, try again" << endl;
    //             t++;
    //         }

    //         cout << "Updated deck " << endl;

    //         for (int i = 0; i < 5; i++) // Выводим на экран измененную колоду для пользователя
    //         {
    //             deck[i].print();
    //         }

    //         cout << endl;

    //         t--;
    //     }