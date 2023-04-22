// Fill out your copyright notice in the Description page of Project Settings.


#include "OpeningBook.h"

OpeningBook::OpeningBook() = default;
OpeningBook::OpeningBook(const string &path)
{
    ifstream file(path);

    if (!file.is_open())
    {
        cout << "Could not find the opening book." << endl;
        exit(255);
    }

    string game;
    stringstream gameThread;

    string stringMove;
    uint8_t from;
    uint8_t to;

    MoveList possibleMoves;
    bool MoveFound;

    BoardPosition buff;

    while (getline(file, game))
    {
        gameThread = stringstream(game);
        this->moves.resize(this->moves.size() + 1);

        buff = {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR", 255, true, true, true, true, 1};

        while (gameThread >> stringMove && gameThread.good())
        {
            from = (stringMove[1] - '1') * 8 + stringMove[0] - 'a';
            to = (stringMove[3] - '1') * 8 + stringMove[2] - 'a';

            possibleMoves = LegalMoveGen::generate(buff, buff.MoveCtr - floor(buff.MoveCtr) > 1e-7);
            MoveFound = false;

            for (uint8_t i = 0; i < possibleMoves.size(); i = i + 1)
            {
                if (possibleMoves[i].From == from && possibleMoves[i].To == to)
                {
                    this->moves.back().push_back(possibleMoves[i]);

                    buff.move(possibleMoves[i]);
                    MoveFound = true;

                    break;
                }
            }

            if (!MoveFound)
            {
                cout << "Error in the opening book." << endl;
                exit(255);
            }
        }
    }

    file.close();
}

tuple<class ChessMove, int32_t> OpeningBook::tryToFindMove(const BoardPosition &position)
{
    BoardPosition buff;

    vector<class ChessMove> possibleMoves;
    bool moveExist;

    for (int32_t game = 0; game < (int)this->moves.size(); game++)
    {
        buff = {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR", 255, true, true, true, true, 1};

        if (buff.pieces == position.pieces)
        {
            moveExist = false;

            for (auto added_move : possibleMoves)
            {
                if (added_move == this->moves[game][0])
                {
                    moveExist = true;
                    break;
                }
            }

            if (!moveExist)
                possibleMoves.push_back(this->moves[game][0]);
            continue;
        }

        for (int32_t move = 0; move < (int)this->moves[game].size() - 1; move++)
        {
            buff.move(this->moves[game][move]);

            if (buff.pieces == position.pieces)
            {
                moveExist = false;

                for (auto addedMove : possibleMoves)
                {
                    if (addedMove == this->moves[game][move + 1])
                    {
                        moveExist = true;
                        break;
                    }
                }

                if (!moveExist)
                    possibleMoves.push_back(this->moves[game][move + 1]);
            }
        }
    }

    if (possibleMoves.empty())
    {
        return std::make_tuple(ChessMove(), 0);
    }

    return std::make_tuple(possibleMoves[time(nullptr) % possibleMoves.size()], possibleMoves.size());
}

OpeningBook::~OpeningBook()
{
}
