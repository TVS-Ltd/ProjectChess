#include "game.h"
using namespace std;

    void Game::start()
    {
        this->position = {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR", 255, true, true, true, true, 1};//Ставим, что никого нет, и потом меняем эту строку

        #if TEST_WHITE_WIN
            this->position = {"rnbqkbnr/pppppppp/8/8/2B5/5Q2/PPPPPPPP/RNB1K1NR", 255, true, true, true, true, 1};
        #endif

        #if TEST_PROMOTION
                this->position = {"rnbqkbnr/11111111/8/8/8/8/K111111p/11111111", 255, true, true, true, true, 1};
        #endif

        #if TEST_CASTLING
                this->position = {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQK11R", 255, true, true, true, true, 1};
        #endif

        #if TEST_EN_PASSANT
                this->position = {"rnbqkbnr/pppppppp/8/1111P111/8/8/PPPP1PPP/RNBQKBNR", 255, true, true, true, true, 1};
        #endif

        #if TEST_THREEE_POS_RULE
                this->position = {"rnbqkbnr/8/8/8/8/8/8/RNBQKBNR", 255, true, true, true, true, 1};
        #endif

        cout << "Welcome to the chess engine!" << endl;

        #if LOG_TO_FILE
                log << "Welcome to the chess engine!";
        #endif

        chooseGameMode();
    }

    

    bool Game::whiteVictory()
    {
        bool blackHaventGotMoves = (LegalMoveGen::generate(this->position, Pieces::Black).size() == 0);
        bool blackInCheck = PsLegalMoveMaskGen::inDanger(this->position.pieces, bsf(this->position.pieces.pieceBitboards[Pieces::Black][Pieces::King]), Pieces::Black);

        if (playerSide == Pieces::White && blackHaventGotMoves && blackInCheck)
        {
            cout << GREEN << endl;
            cout << "White win!" << END << endl;

            #if LOG_TO_FILE
                log << GREEN << "White win!" << END;
            #endif
        } else 
        if (blackHaventGotMoves && blackInCheck)
        {
            cout << RED << endl;
            cout << "White win!" << END << endl;

            #if LOG_TO_FILE
                log << RED << "White win!" << END;
            #endif
        } else
        if(playerSide == Pieces::Black && timerBlack.getElapsedSeconds() > timerLose)
        {
            cout << RED << "White win!" << END << endl;
            clog << RED << "Time is over!" << END << endl;

            #if LOG_TO_FILE
                log << GREEN << "White win!" << END;
            #endif

            return true;
        } else
        if (playerSide == Pieces::White && timerBlack.getElapsedSeconds() > timerLose)
        {
            cout << GREEN << "White win!" << END << endl;
            clog << RED << "Time is over!" << END << endl;

            #if LOG_TO_FILE
                log << GREEN << "White win!" << END;
            #endif

            return true;
        }
        
        

        return (blackHaventGotMoves && blackInCheck);
    }

    bool Game::blackVictory()
    {
        bool whiteHaventGotMoves = (LegalMoveGen::generate(this->position, Pieces::White).size() == 0);
        bool whiteInCheck = PsLegalMoveMaskGen::inDanger(this->position.pieces, bsf(this->position.pieces.pieceBitboards[Pieces::White][Pieces::King]), Pieces::White);

        if (playerSide == Pieces::Black && whiteHaventGotMoves && whiteInCheck)
        {
            cout << GREEN << endl;
            cout << "Black win!" << END << endl;

            #if LOG_TO_FILE
                log << GREEN << "Black win!" << END;
            #endif
        }
        else if (whiteHaventGotMoves && whiteInCheck)
        {
            cout << RED << endl;
            cout << "Black win!" << END << endl;

            #if LOG_TO_FILE
                log << RED << "Black win!" << END;
            #endif
        } else
        if (playerSide == Pieces::White && timerWhite.getElapsedSeconds() > timerLose)
        {
            cout << RED << "Black win!" << END << endl;
            clog << RED << "Time is over!" << END << endl;

            #if LOG_TO_FILE
                log << RED << "Black win!" << END;
            #endif

            return true;
        } else 
        if (playerSide == Pieces::Black && timerWhite.getElapsedSeconds() > timerLose)
        {
            cout << GREEN << "Black win!" << END << endl;
            clog << RED << "Time is over!" << END << endl;

            #if LOG_TO_FILE
                log << RED << "Black win!" << END;
            #endif

            return true;
        } 
        
        return (whiteHaventGotMoves && whiteInCheck);
    }

    bool Game::draw()
    {
        bool whiteHaventGotMoves = (LegalMoveGen::generate(this->position, Pieces::White).size() == 0);
        bool whiteInCheck = PsLegalMoveMaskGen::inDanger(this->position.pieces, bsf(this->position.pieces.pieceBitboards[Pieces::White][Pieces::King]), Pieces::White);

        bool blackHaventGotMoves = (LegalMoveGen::generate(this->position, Pieces::Black).size() == 0);
        bool blackInCheck = PsLegalMoveMaskGen::inDanger(this->position.pieces, bsf(this->position.pieces.pieceBitboards[Pieces::Black][Pieces::King]), Pieces::Black);

        bool fiftyMovesRule = (this->position.fiftyMovesCtr >= 50);
        bool threeMovesRule = (this->position.repetitionHistory.getRepetionNumber(this->position.hash) >= 3);

        bool whiteMove = this->isWhiteMove();
        bool blackMove = !whiteMove;

        if (whiteHaventGotMoves && !whiteInCheck && whiteMove)
        {
            cout << YELLOW << "[DRAW] White haven't got moves." << END << endl;

            #if LOG_TO_FILE
                log << YELLOW << "[DRAW] White haven't got moves." << END;
            #endif

            return true;
        }

        if (blackHaventGotMoves && !blackInCheck && blackMove)
        {
            cout << YELLOW << "[DRAW] Black haven't got moves." << END << endl;

            #if LOG_TO_FILE
                log << YELLOW << "[DRAW] Black haven't got moves." << END;
            #endif

            return true;
        }

        if (fiftyMovesRule)
        {
            cout << YELLOW << "[DRAW] Fifty moves rule." << END << endl;

            #if LOG_TO_FILE
                log << YELLOW << "[DRAW] Fifty moves rule." << END;
            #endif

            return true;
        }

        if (threeMovesRule)
        {
            cout << YELLOW << "[DRAW] Three moves rule." << END << endl;

            #if LOG_TO_FILE
                log << YELLOW << "[DRAW] Three moves rule." << END;
            #endif

            return true;
        }

        if (timerWhite.getElapsedSeconds() == timerBlack.getElapsedSeconds() && timerWhite.getElapsedSeconds() > timerLose)
        {
            cout << YELLOW << "[DRAW] Time is up." << END << endl;

            #if LOG_TO_FILE
                log << YELLOW << "[DRAW] Time is up." << END;
            #endif

            return true;
        }

        return false;
    }

    bool Game::gameFinished()
    {
        if (this->whiteVictory() || this->blackVictory() || this->draw())
        {
            timerWhite.reset();
            timerBlack.reset();

            return true;
        }

        return false;
    }

    bool Game::isWhiteMove() const
    {
        return (this->position.MoveCtr - floor(this->position.MoveCtr) < 1e-7);
    }

    bool Game::isBlackMove()
    {
        return !isWhiteMove();
    }

    Move Game::getMove()//
    {
        Move PlayerMove;
        string from, to;

        cout << "\nEnter move (Example: e2 e4): ";
        cin >> from >> to;

        PlayerMove.From = (from[0] - 'a') + (from[1] - '1') * 8;// в unt
        PlayerMove.To = (to[0] - 'a') + (to[1] - '1') * 8;

        return PlayerMove;
    }

    void Game::sideChoose()
    {
        cout << "Choose your side:" << endl;
        cout << "1. White" << endl;
        cout << "2. Black" << endl;

        int choice;

        do
        {
            cin >> choice;

            if (cin.fail() || choice < 1 || choice > 2)
            {
                cin.clear();
                cin.ignore(1000, '\n');

                cout << RED << "[ERROR] Invalid input. Please try again." << END << endl;
            }
        
        } while (cin.fail() || choice < 1 || choice > 2);

        if (choice == 1)
        {
            cout << WHITE << "You are playing as white." << END << endl;

            #if LOG_TO_FILE
                log << WHITE << "User is playing as white." << END;
            #endif

            playerSide = Pieces::White;
            aiSide = Pieces::Black;
        }
        else
        {
            cout << BLACK << WHITEBG << "You are playing as black." << END << endl;

            #if LOG_TO_FILE
                log << BLACK << WHITEBG << "User is playing as black." << END;
            #endif

            playerSide = Pieces::Black;
            aiSide = Pieces::White;
        }
    }

    bool Game::movePlayer(uint8_t side) // Returns true if player move is valid
    {
        this->moves = LegalMoveGen::generate(this->position, side);

        this->playerMove = getMove();
        bool moveFound = false;

        for (uint8_t i = 0; i < moves.size(); i++)
        {
            if (moves[i].From == playerMove.From && moves[i].To == playerMove.To && (moves[i].Flag != Move::Flag::PromoteToKnight && moves[i].Flag != Move::Flag::PromoteToBishop && moves[i].Flag != Move::Flag::PromoteToRook && moves[i].Flag != Move::Flag::PromoteToQueen))
            {
                move = moves[i];
                moveFound = true;

                break;
            }
            else if (moves[i].From == playerMove.From && moves[i].To == playerMove.To)
            {
                // Choose promotion piece
                cout << "\nChoose promotion piece: " << endl;
                cout << "1. Knight" << endl;
                cout << "2. Bishop" << endl;
                cout << "3. Rook" << endl;
                cout << "4. Queen" << endl;

                int promotionPiece = 0;
                cin >> promotionPiece;

                switch (promotionPiece)
                {
                    case 1:
                        move = moves[i];
                        move.Flag = Move::Flag::PromoteToKnight;
                        moveFound = true;
                    break;

                    case 2:
                        move = moves[i];
                        move.Flag = Move::Flag::PromoteToBishop;
                        moveFound = true;
                    break;

                    case 3:
                        move = moves[i];
                        move.Flag = Move::Flag::PromoteToRook;
                        moveFound = true;
                    break;

                    case 4:
                        move = moves[i];
                        move.Flag = Move::Flag::PromoteToQueen;
                        moveFound = true;
                    break;

                    default:
                        cout << RED << "\n[ERROR] Illegal move!" << END << endl;
                        continue;
                    break;
                }

                break;
            }
        }

        if (!moveFound)
        {
            cout << RED << "\n[ERROR] Illegal move!" << END << endl;
            return false;
        }

        this->position.move(move);

        if (move.DefenderType != 255)
        {
            cout << "\n";

            switch (move.DefenderType)
            {
            case 0:
                cout << YELLOW << "Pawn has been captured!" << END << endl;
                break;

            case 1:
                cout << YELLOW << "Knight has been captured!" << END << endl;
                break;

            case 2:
                cout << YELLOW << "Bishop has been captured!" << END << endl;
                break;

            case 3:
                cout << YELLOW << "Rook has been captured!" << END << endl;
                break;

            case 4:
                cout << YELLOW << "Queen has been captured!" << END << endl;
                break;

            default:
                cout << RED << "[ERROR] Unknown piece type!" << END << endl;
                break;
            }
        }

        cout << "\nMove has been made!" << endl;
        cout << Game::position << endl;

        return true;
    }

    bool Game::movePlayerForRoyalChess(uint8_t side, uint8_t playerTypeOfFigure)
    {
        this->moves = LegalMoveGen::generate(this->position, side);

        this->playerMove = getMove();
        bool moveFound = false;

        for (uint8_t i = 0; i < moves.size(); i++)
        {
            if (moves[i].From == playerMove.From && moves[i].To == playerMove.To && (moves[i].Flag != Move::Flag::PromoteToKnight && moves[i].Flag != Move::Flag::PromoteToBishop && moves[i].Flag != Move::Flag::PromoteToRook && moves[i].Flag != Move::Flag::PromoteToQueen))
            {
                move = moves[i];
                moveFound = true;

                break;
            }
            else if (moves[i].From == playerMove.From && moves[i].To == playerMove.To)
            {
                // Choose promotion piece
                cout << "\nChoose promotion piece: " << endl;
                cout << "1. Knight" << endl;
                cout << "2. Bishop" << endl;
                cout << "3. Rook" << endl;
                cout << "4. Queen" << endl;

                int promotionPiece = 0;
                cin >> promotionPiece;

                switch (promotionPiece)
                {
                    case 1:
                        move = moves[i];
                        move.Flag = Move::Flag::PromoteToKnight;
                        moveFound = true;
                    break;

                    case 2:
                        move = moves[i];
                        move.Flag = Move::Flag::PromoteToBishop;
                        moveFound = true;
                    break;

                    case 3:
                        move = moves[i];
                        move.Flag = Move::Flag::PromoteToRook;
                        moveFound = true;
                    break;

                    case 4:
                        move = moves[i];
                        move.Flag = Move::Flag::PromoteToQueen;
                        moveFound = true;
                    break;

                    default:
                        cout << RED << "\n[ERROR] Illegal move!" << END << endl;
                        continue;
                    break;
                }

                break;
            }
        }

        if(playerTypeOfFigure == 69)
        {
            return true;
        }

        if(move.AttackerType != playerTypeOfFigure)
        {
            return false;
        }

        if (!moveFound)
        {
            cout << RED << "\n[ERROR] Illegal move!" << END << endl;
            return false;
        }

        this->position.move(move);

        if (move.DefenderType != 255)
        {
            cout << "\n";

            switch (move.DefenderType)
            {
            case 0:
                cout << YELLOW << "Pawn has been captured!" << END << endl;
                break;

            case 1:
                cout << YELLOW << "Knight has been captured!" << END << endl;
                break;

            case 2:
                cout << YELLOW << "Bishop has been captured!" << END << endl;
                break;

            case 3:
                cout << YELLOW << "Rook has been captured!" << END << endl;
                break;

            case 4:
                cout << YELLOW << "Queen has been captured!" << END << endl;
                break;

            default:
                cout << RED << "[ERROR] Unknown piece type!" << END << endl;
                break;
            }
        }

        cout << "\nMove has been made!" << endl;
        cout << Game::position << endl;

        return true;
    }

    void Game::PvE()
    {
        sideChoose();

        if (playerSide == Pieces::White)
        {
            while (true)
            {

                cout << position << endl;

                #if LOG_TO_FILE
                    log << position;
                #endif

                if (!movePlayer(playerSide))
                {
                    continue;
                }

                // Check if game is finished
                if (this->gameFinished())
                {
                    cout << position << endl;

                    #if LOG_TO_FILE
                        log << position;
                    #endif

                    break;
                }

                // AI move
                move = ai.bestMove(position, aiSide, 0, 10000);
                position.move(move);

                // Check if game is finished
                if (this->gameFinished())
                {
                    cout << position << endl;

                    #if LOG_TO_FILE
                        log << position;
                    #endif

                    break;
                }
            }
        }
        else
        {
            cout << position << endl;

            #if LOG_TO_FILE
                log << position;
            #endif

            while (true)
            {

                // AI move
                move = ai.bestMove(position, aiSide, 0, 10000);
                position.move(move);

                cout << position << endl;

                #if LOG_TO_FILE
                    log << position;
                #endif

                // Check if game is finished
                if (this->gameFinished())
                {
                    cout << position << endl;

                    #if LOG_TO_FILE
                        log << position;
                    #endif

                    break;
                }

                // Player move
                while (!movePlayer(playerSide))
                {
                    continue;
                }

                // Check if game is finished
                if (this->gameFinished())
                {
                    cout << position << endl;

                    #if LOG_TO_FILE
                        log << position;
                    #endif

                    break;
                }
            }
        }
    }

    void Game::PvP()
    {
        sideChoose();

        cout << position << endl;

        #if LOG_TO_FILE
            log << position;
        #endif

        if(playerSide == Pieces::White)
        {
            while (true)
            {

                // Player 1 move
                cout << GREEN << "\nPlayer 1 move:" << END << endl;

                #if LOG_TO_FILE
                    log << GREEN << "\nPlayer 1 move:" << END;
                #endif

                while (!movePlayer(playerSide))
                {
                    continue;
                }

                // Check if game is finished
                if (this->gameFinished())
                {
                    cout << position << endl;

                    #if LOG_TO_FILE
                        log << position;
                    #endif

                    break;
                }

                // Player 2 move
                cout << YELLOW << "\nPlayer 2 move:" << END << endl;

                #if LOG_TO_FILE
                    log << YELLOW << "\nPlayer 2 move:" << END;
                #endif

                while (!movePlayer(aiSide))
                {
                    continue;
                }

                // Check if game is finished
                if (this->gameFinished())
                {
                    cout << position << endl;

                    #if LOG_TO_FILE
                        log << position;
                    #endif

                    break;
                }
            }
        }
        else
        {
            while (true)
            {

                // Player 2 move
                cout << YELLOW << "\nPlayer 2 move:" << END << endl;

                #if LOG_TO_FILE
                    log << YELLOW << "\nPlayer 2 move:" << END;
                #endif

                while (!movePlayer(aiSide))
                {
                    continue;
                }

                // Check if game is finished
                if (this->gameFinished())
                {
                    cout << position << endl;

                    #if LOG_TO_FILE
                        log << position;
                    #endif

                    break;
                }

                // Player 1 move
                cout << GREEN << "\nPlayer 1 move:" << END << endl;

                #if LOG_TO_FILE
                    log << GREEN << "\nPlayer 1 move:" << END;
                #endif

                while (!movePlayer(playerSide))
                {
                    continue;
                }

                // Check if game is finished
                if (this->gameFinished())
                {
                    cout << position << endl;

                    #if LOG_TO_FILE
                        log << position;
                    #endif

                    break;
                }
            }
        }
    }

    void Game::Gwent()// rm -f *.o 
    {
        const char* filename = "/home/danila/CCG/ProjectChess/src/DataBase/DB.db";

        sqlite3 *db;      
        handsdeck coloda_white;
        handsdeck coloda_black;

        std::cout << "Create your deck:" << endl;

        deck bd;
        
        //Реализовать заполнение бд с учетом доступных для игрока очков
     
        int point = 250; //рандомное кол-во

        std::cout << "You have a " << point << " points" << endl;

        std::cout << "Choose a figure \n 1.Queen (50 points) \n 2.Rook(20 points) \n 3.Knight(30 points) \n 4.Bishop(25 points) \n 5.Pawn(10 points)";  

        int choice;

        sqlite3_open("/home/danila/CCG/ProjectChess/src/DataBase/DB.db", &db);

        int rc = sqlite3_open("/home/danila/CCG/ProjectChess/src/DataBase/DB.db", &db);
        
        if (rc != SQLITE_OK) 
        {
            std::cout << "\n" << "Cannot open database: " << sqlite3_errmsg(db) << std::endl;
        }

        bd.clear_Bd(db);

        std::cout << std::endl << "test for bd clear:" << std::endl;

        if (bd.size_of_BD(db) == 0)
        {
            std::cout << "BD is empty " << std::endl;
        }else
        {
            std::cout << "BD is not empty " << std::endl;
        }

        std::cout << "Составление колоды для белых:" << std::endl;

        while (true) //Блок для формирования колоды
        {    
         
            cout << endl;
            
            cin >> choice;

            if((choice == 1 && point >= 50) || (choice == 2 && point >= 20) || (choice == 3 && point >= 30) || (choice == 4 && point >= 25) || (choice == 5 && point >= 10))
            {
                switch (choice)
                {
                case 1:
                    bd.appendToBD(filename, "Queen", db);
                    cout << "Queen has been added" << endl;
                    point -= 50;
                    break;

                case 2:
                    bd.appendToBD(filename, "Rook", db);
                    cout << "Rook has been added" << endl;
                    point -= 20;
                    break;

                case 3:
                    bd.appendToBD(filename, "Knight", db);
                    cout << "Knight has been added" << endl;
                    point -= 30;
                    break;

                case 4:
                    bd.appendToBD(filename, "Bishop", db);
                    cout << "Bishop has been added" << endl;
                    point -= 25;
                    break;

                case 5:
                    bd.appendToBD(filename, "Pawn", db);
                    cout << "Pawn has been added" << endl;
                    point -= 10;
                    break;

                default:
                    break;
                }
            }else
            {
                std::cout << "You have't so much points" << endl;
                break;
            }
        }
        
        sqlite3_stmt *stmt;
        const char *query = "SELECT * FROM DECK;";
        sqlite3_prepare_v2(db, query, -1, &stmt, NULL);

        std::cout << "Deck(main menu)" << std::endl;
       
        bd.print_data_base(stmt); 

        std::cout << "Deck after shuffle:" << std::endl;

        bd.shuffle(db);
    
        sqlite3_stmt *stm;
        const char *quer = "SELECT * FROM DECK;";
        sqlite3_prepare_v2(db, quer, -1, &stm, NULL);

        bd.print_data_base(stm);

        std::vector<std::string> input;
        sqlite3_stmt* s;
        std::string sql = "SELECT * FROM DECK;";
        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &s, NULL) == SQLITE_OK)
        {
            while (sqlite3_step(s) == SQLITE_ROW)
            {
                input.push_back(std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0))));
            }
            sqlite3_finalize(s);
        }

        std::cout << "Your deck: " << endl;
        
        // Создаем 5 карт с рандомными фигурами
        card obj_1(input[0], "test", " test_1"); card obj_2(input[1], "test", " test_1"); card obj_3(input[2], "test", " test_1"); card obj_4(input[3], "test", " test_1"); card obj_5(input[4], "test", " test_1");       

        // Кладем их в вектор
        coloda_white.push_b(obj_1); coloda_white.push_b(obj_2); coloda_white.push_b(obj_3); coloda_white.push_b(obj_4); coloda_white.push_b(obj_5);

        // Выводим на экран колоду для пользователя        
        coloda_white.print();        

        std::cout << endl;

        std::cout << "You can make two substitutions. Enter the card number (1-5) you want to replace. Enter 0 if you don't want to make substitutions at all." << endl;

        int t = 2;
        std::string temp = "";

       while (t != 0) // Две замены карты(белые) 
        {
            int choice;

            cin >> choice;

            if (choice == 0)
            {
                break;
            }else 
            if (choice == 1)
            {
                temp = bd.get_random_value(db);
               
                obj_1.setFigure(temp);

                coloda_white.CardChange(0, obj_1);
                
            }else 
            if (choice == 2)
            {
                temp = bd.get_random_value(db);
               
                obj_2.setFigure(temp);

                coloda_white.CardChange(1, obj_2);
                
            }else
            if (choice == 3)
            {
              temp = bd.get_random_value(db);
               
                obj_3.setFigure(temp);

                coloda_white.CardChange(2, obj_3);

            }else 
            if (choice == 4)
            {
               temp = bd.get_random_value(db);
               
                obj_4.setFigure(temp);

                coloda_white.CardChange(3, obj_4);

            }else 
            if (choice == 5)
            {
               temp = bd.get_random_value(db);
               
                obj_5.setFigure(temp);

                coloda_white.CardChange(4, obj_5);

            }else 
            if (choice > 5 || choice < 0 || double(choice))
            {
                std::cout << "Incorrect input. Please, try again" << endl;
                t++;
            }

            std::cout << "Updated deck " << endl;

            // Выводим на экран измененную колоду для пользователя
 
            coloda_white.print();

            std::cout << endl;

            t--;
        }	
        

        //Граница

        std::cout << std::endl << "Составление колоды для черных:" << std::endl;

        point = 250; //рандомное кол-во

        std::cout << "You have a " << point << " points" << endl;

        std::cout << "Choose a figure \n 1.Queen (50 points) \n 2.Rook(20 points) \n 3.Knight(30 points) \n 4.Bishop(25 points) \n 5.Pawn(10 points)";  
    
        while (true) //Блок для формирования колоды
        {    
         
            cout << endl;
            
            cin >> choice;

            if((choice == 1 && point >= 50) || (choice == 2 && point >= 20) || (choice == 3 && point >= 30) || (choice == 4 && point >= 25) || (choice == 5 && point >= 10))
            {
                switch (choice)
                {
                case 1:
                    bd.appendToBlackBD(filename, "Queen", db);
                    cout << "Queen has been added" << endl;
                    point -= 50;
                    break;

                case 2:
                    bd.appendToBlackBD(filename, "Rook", db);
                    cout << "Rook has been added" << endl;
                    point -= 20;
                    break;

                case 3:
                    bd.appendToBlackBD(filename, "Knight", db);
                    cout << "Knight has been added" << endl;
                    point -= 30;
                    break;

                case 4:
                    bd.appendToBlackBD(filename, "Bishop", db);
                    cout << "Bishop has been added" << endl;
                    point -= 25;
                    break;

                case 5:
                    bd.appendToBlackBD(filename, "Pawn", db);
                    cout << "Pawn has been added" << endl;
                    point -= 10;
                    break;

                default:
                    continue;
                    break;
                }
            }else
            {
                std::cout << "You have't so much points" << endl;
                break;
            }
        }
        
        sqlite3_stmt *stmtmt;
        const char *querrry = "SELECT * FROM BIGBLACKVADIMVLADYMTSEV;";
        sqlite3_prepare_v2(db, querrry, -1, &stmtmt, NULL);

        std::cout << "Deck(main menu)" << std::endl;
       
        bd.print_data_base(stmtmt); 

        std::cout << "Deck after shuffle:" << std::endl;

        bd.blackShuffle(db);
    
        sqlite3_stmt *stttm;
        const char *querrr = "SELECT * FROM BIGBLACKVADIMVLADYMTSEV;";
        sqlite3_prepare_v2(db, querrr, -1, &stttm, NULL);

        bd.print_data_base(stttm);

        std::vector<std::string> inputBlack;
        sqlite3_stmt* sss;
        std::string sssql = "SELECT * FROM BIGBLACKVADIMVLADYMTSEV;";
        if (sqlite3_prepare_v2(db, sssql.c_str(), -1, &sss, NULL) == SQLITE_OK)
        {
            while (sqlite3_step(sss) == SQLITE_ROW)
            {
                inputBlack.push_back(std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0))));
            }
            sqlite3_finalize(sss);
        }

        std::cout << "Your deck: " << endl;
        
        // Создаем 5 карт с рандомными фигурами
        card obj_6(inputBlack[0], "test", " test_1"); card obj_7(inputBlack[1], "test", " test_1"); card obj_8(inputBlack[2], "test", " test_1"); card obj_9(inputBlack[3], "test", " test_1"); card obj_10(inputBlack[4], "test", " test_1");       

        // Кладем их в вектор

        coloda_black.push_b(obj_6); coloda_black.push_b(obj_7); coloda_black.push_b(obj_8); coloda_black.push_b(obj_9); coloda_black.push_b(obj_10);

        // Выводим на экран колоду для пользователя        
        coloda_black.print();        

        std::cout << std::endl << "You can make two substitutions. Enter the card number (1-5) you want to replace. Enter 0 if you don't want to make substitutions at all." << std::endl;

        t = 2;
        temp = "";

       while (t != 0) // Две замены карты(черные) 
        {
            int choice;

            cin >> choice;

            if (choice == 0)
            {
                break;
            }else 
            if (choice == 1)
            {
                temp = bd.get_random_BlackValue(db);
               
                obj_6.setFigure(temp);

                coloda_black.CardChange(0, obj_6);
                
            }else 
            if (choice == 2)
            {
                temp = bd.get_random_BlackValue(db);
               
                obj_7.setFigure(temp);

                coloda_black.CardChange(1, obj_7);
                
            }else
            if (choice == 3)
            {
              temp = bd.get_random_BlackValue(db);
               
                obj_8.setFigure(temp);

                coloda_black.CardChange(2, obj_8);

            }else 
            if (choice == 4)
            {
               temp = bd.get_random_BlackValue(db);
               
                obj_9.setFigure(temp);

                coloda_black.CardChange(3, obj_9);

            }else 
            if (choice == 5)
            {
               temp = bd.get_random_BlackValue(db);
               
                obj_10.setFigure(temp);

                coloda_black.CardChange(4, obj_10);

            }else 
            if (choice > 5 || choice < 0 || double(choice))
            {
                std::cout << "Incorrect input. Please, try again" << endl;
                t++;
            }

            std::cout << "Updated deck " << endl;

            // Выводим на экран измененную колоду для пользователя
 
            coloda_black.print();

            std::cout << endl;

            t--;
        }	

        Game::position = {"....k.../......../8/8/8/8/......../....K...", 255, true, true, true, true, 1};
        
        sideChoose();

                #if LOG_TO_FILE
            log << position;
        #endif

        int counterOfMoves = 0;

        if(playerSide == Pieces::White)
        {
            int count = 4;

            while (true)
            {
                counterOfMoves++;

                count++;
                
                cout << Game::position << endl;

                // Player 1 move

                cout << GREEN << "\nPlayer 1 move:" << END << endl;

                #if LOG_TO_FILE
                    log << GREEN << "\nPlayer 1 move:" << END;
                #endif

                if(!coloda_white.checkIsEmpty())
                {
                    //Старт выставления фигуры на поле

                    char choiceOfFigure;

                    string placeOnBoard, typeOfCard;

                    while(true)
                    {
                        cout << "Choice a figure from your deck, which you want to put on the field: (Enter the first letter (ex: R, K, Q))" << endl; 

                        if(counterOfMoves >= 2 && input.size() != 0)
                        {
                            std::cout << "Карта. которая должна быть добавлена: " << input[count] << std::endl;
                            
                            card temp(input[count], "test", " test_1");

                            coloda_white.addCard(temp); 
                        }

                        coloda_white.print();

                        cout << endl;

                        cin >> choiceOfFigure;

                        cout << "Enter the position when you want to put a figure(example e2)" << endl;

                        cin >> placeOnBoard; // e4

                        if((int)placeOnBoard[1] < 5)
                        {
                            std::cout << RED << "Incorrect input, please try again" << END << std::endl; 
                            continue;
                        }

                        if(coloda_white.checkForCard(choiceOfFigure))
                        { 
                            break;
                        }   
                        else
                        { 
                            std::cout << RED << "Incorrect input, please try again" << END << std::endl; 
                            continue;
                        }
                    }
                    
                    switch (choiceOfFigure)
                    {
                    case 'P':

                        this->position.addPiece((placeOnBoard[0] - 'a') + (placeOnBoard[1] - '1') * 8, Pieces::Pawn, Pieces::White);

                        typeOfCard = "Pawn";
                        
                        break;

                    case 'R':
                        
                        this->position.addPiece((placeOnBoard[0] - 'a') + (placeOnBoard[1] - '1') * 8, Pieces::Rook, Pieces::White);

                        typeOfCard = "Rook";

                        break;
                    
                    case 'N':

                        this->position.addPiece((placeOnBoard[0] - 'a') + (placeOnBoard[1] - '1') * 8, Pieces::Knight, Pieces::White);

                        typeOfCard = "Knight";

                        break;

                    case 'B':

                        this->position.addPiece((placeOnBoard[0] - 'a') + (placeOnBoard[1] - '1') * 8, Pieces::Bishop, Pieces::White);

                        typeOfCard = "Bishop";

                        break;

                    case 'Q':

                        this->position.addPiece((placeOnBoard[0] - 'a') + (placeOnBoard[1] - '1') * 8, Pieces::Queen, Pieces::White);

                        typeOfCard = "Queen";

                        break;

                    default:
                        break;
                    }

                    coloda_white.delete_card(choiceOfFigure);  

                }
                //Конец
                
                cout << Game::position;
                
                while (!movePlayer(playerSide))
                {
                    continue;
                }

                // Check if game is finished
                if (this->gameFinished())
                {
                    cout << position << endl;

                    #if LOG_TO_FILE
                        log << position;
                    #endif

                    break;
                }

                // Player 2 move
                cout << YELLOW << "\nPlayer 2 move:" << END << endl;

                #if LOG_TO_FILE
                    log << YELLOW << "\nPlayer 2 move:" << END;
                #endif

                std::cout << Game::position << std::endl;
                
                if(!coloda_white.checkIsEmpty())
                {
                    //Старт выставления фигуры на поле

                    char choiceOfFigure;

                    string placeOnBoard;

                    while(true)
                    {
                        cout << "Choice a figure from your deck, which you want to put on the field: (Enter the first letter (ex: R, K, Q))" << endl; 

                        if(counterOfMoves >= 2 && inputBlack.size() != 0)
                        {
                            std::cout << "Карта, которая должна быть добавлена: " << inputBlack[count] << std::endl;
                            
                            card temp(inputBlack[count], "test", " test_1");

                            coloda_black.addCard(temp);  
                        }

                        coloda_black.print();

                        cout << endl;

                        cin >> choiceOfFigure;

                        cout << "Enter the position when you want to put a figure(example e2)" << endl;

                        cin >> placeOnBoard; // e4

                        if((int)placeOnBoard[1] < 5)
                        {
                            std::cout << RED << "Incorrect input, please try again" << END << std::endl; 
                            continue;
                        }

                        if(coloda_black.checkForCard(choiceOfFigure))
                        { 
                            break;
                        }   
                        else
                        { 
                            std::cout << RED << "Incorrect input, please try again" << END << std::endl; 
                            continue;
                        }
                    }

                    switch (tolower(choiceOfFigure))
                    {
                    case 'p':

                        this->position.addPiece((placeOnBoard[0] - 'a') + (placeOnBoard[1] - '1') * 8, Pieces::Pawn, Pieces::Black);
                        
                        break;

                    case 'r':
    
                        this->position.addPiece((placeOnBoard[0] - 'a') + (placeOnBoard[1] - '1') * 8, Pieces::Rook, Pieces::Black);

                        break;
                    
                    case 'n':

                        this->position.addPiece((placeOnBoard[0] - 'a') + (placeOnBoard[1] - '1') * 8, Pieces::Knight, Pieces::Black);

                        break;

                    case 'b':

                        this->position.addPiece((placeOnBoard[0] - 'a') + (placeOnBoard[1] - '1') * 8, Pieces::Bishop, Pieces::Black);

                        break;

                    case 'q':

                        this->position.addPiece((placeOnBoard[0] - 'a') + (placeOnBoard[1] - '1') * 8, Pieces::Queen, Pieces::Black);

                        break;

                    default:
                        break;
                    }

                    std::cout << Game::position << std::endl;        
                }

                while (!movePlayer(aiSide))
                {
                    
                    continue;
                }

                // Check if game is finished
                if (this->gameFinished())
                {
                    cout << Game::position << endl;

                    #if LOG_TO_FILE
                        log << position;
                    #endif

                    break;
                }
            
                count++;
            }
        }
        else
        {
            while (true)
            {

                // Player 2 move
                cout << YELLOW << "\nPlayer 2 move:" << END << endl;

                #if LOG_TO_FILE
                    log << YELLOW << "\nPlayer 2 move:" << END;
                #endif

                while (!movePlayer(aiSide))
                {
                    continue;
                }

                // Check if game is finished
                if (this->gameFinished())
                {
                    cout << position << endl;

                    #if LOG_TO_FILE
                        log << position;
                    #endif

                    break;
                }

                // Player 1 move
                cout << GREEN << "\nPlayer 1 move:" << END << endl;

                #if LOG_TO_FILE
                    log << GREEN << "\nPlayer 1 move:" << END;
                #endif

                while (!movePlayer(playerSide))
                {
                    continue;
                }

                // Check if game is finished
                if (this->gameFinished())
                {
                    cout << position << endl;

                    #if LOG_TO_FILE
                        log << position;
                    #endif

                    break;
                }
            }
        }
    
    sqlite3_close(db);
}   

    void Game::RoyalChess()
    {
        Game::position = {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR", 255, true, true, true, true, 1};

        char choice;

        int counterForJocker = 0;
    
        std::stack <uint8_t> whiteDeck;
        std::stack <uint8_t> blackDeck;

        for(int i = 0;i < 50; i++) //white deck
        {
            int ch = rand() % 5 +1;
            
            switch (ch)
            {
            case 1:
                
                whiteDeck.push(Pieces::Pawn);

                break;
            
            case 2:

                whiteDeck.push(Pieces::Rook);

                break;

            case 3:

                whiteDeck.push(Pieces::Knight);

                break;

            case 4:

                whiteDeck.push(Pieces::Bishop);

                break;

            case 5:

                whiteDeck.push(Pieces::Queen);

                break;

            case 6:

                if(counterForJocker < 2)
                {
                    whiteDeck.push(69);
                }else
                {
                    i--;

                    continue;
                }
                
                break;
            
            default:
                break;
            }

        }

        counterForJocker = 0;

        for(int i = 0;i < 50; i++) //black deck
        {
            int ch = rand() % 5 +1;
            
            switch (ch)
            {
            case 1:
                
                blackDeck.push(Pieces::Pawn);

                break;
            
            case 2:

                blackDeck.push(Pieces::Rook);

                break;

            case 3:

                blackDeck.push(Pieces::Knight);

                break;

            case 4:

                blackDeck.push(Pieces::Bishop);

                break;

            case 5:

                blackDeck.push(Pieces::Queen);

                break;

            case 6:

                if(counterForJocker < 2)
                {
                    blackDeck.push(69);
                }else
                {
                    i--;

                    continue;
                }
                
                break;
            
            default:
                break;
            }

        }

        std::vector <uint8_t> handsdeckWhite;
        std::vector <uint8_t> handsdeckBlack;

        for (int i = 0;i < 5;i++) handsdeckWhite.push_back(i);

        for (int i = 0;i < 5;i++) handsdeckBlack.push_back(i);

        sideChoose();

        cout << position << endl;

        #if LOG_TO_FILE
            log << position;
        #endif

        if(playerSide == Pieces::White)
        {
            while (true)
            {
                
                // Player 1 move
                cout << GREEN << "\nPlayer 1 move:" << END << endl;

                #if LOG_TO_FILE
                    log << GREEN << "\nPlayer 1 move:" << END;
                #endif

                std::cout << "Your deck: " << endl;

                for (int i = 0;i < 5;i++) // print deck
                {
                    switch (handsdeckWhite[i])
                    {
                    case Pieces::Pawn:
                        
                        cout << "Pawn ";

                        break;
                    case Pieces::Rook:
                        
                        cout << "Rook ";

                        break;

                    case Pieces::King:

                        cout << "King ";

                        break;

                    case Pieces::Queen:
                       
                        cout << "Queen ";

                        break;

                    case Pieces::Bishop:
                        
                        cout << "Bishop ";

                        break;

                    case Pieces::Knight:
                        
                        cout << "Knight ";

                        break;

                    case 69:

                        cout << "Jocker ";

                        break;

                    default:
                        break;
                    }
                }
                
                cout << endl;
                
                cin >> choice;

                uint8_t temp;

                switch(choice) // user choice
                {
                    case 'Q':
                        
                        temp = Pieces::Queen;

                        break;

                    case 'K':

                        temp = Pieces::King;

                        break;

                    case 'R':

                        temp = Pieces::Rook;

                        break;

                    case 'N':

                        temp = Pieces::Knight;

                        break;

                    case 'B': 

                        temp = Pieces::Bishop;

                        break;

                    case 'P':

                        temp = Pieces::Pawn;

                        break;

                    case 'J':

                        temp = 69;

                        break;
                }
                
                for (int i = 0; i < 5; i++) // replace cards
                {
                    if(temp == handsdeckWhite[i])
                    {
                        handsdeckWhite[i] = whiteDeck.top();

                        whiteDeck.pop();
                    }
                }

                while (!movePlayerForRoyalChess(playerSide, temp))// check for legal move
                {                     
                    cout << RED << "Wrong figure, try again" << END << endl;

                    continue;
                }

                if(!position.pieces.pieceBitboards[Pieces::Black][Pieces::Queen])
                {
                    // Check if game is finished
                    if (this->gameFinished())
                    {
                        cout << position << endl;

                        #if LOG_TO_FILE
                            log << position;
                        #endif

                        break;
                    } 
                }

                // Player 2 move
                cout << YELLOW << "\nPlayer 2 move:" << END << endl;

                #if LOG_TO_FILE
                    log << YELLOW << "\nPlayer 2 move:" << END;
                #endif

                std::cout << "Your deck: " << endl;

                for (int i = 0;i < 5;i++) // print deck
                {
                    switch (handsdeckBlack[i])
                    {
                    case Pieces::Pawn:
                        
                        cout << "Pawn ";

                        break;
                    case Pieces::Rook:
                        
                        cout << "Rook ";

                        break;

                    case Pieces::King:

                        cout << "King ";

                        break;

                    case Pieces::Queen:
                       
                        cout << "Queen ";

                        break;

                    case Pieces::Bishop:
                        
                        cout << "Bishop ";

                        break;

                    case Pieces::Knight:
                        
                        cout << "Knight ";

                        break;

                    case 69:

                        cout << "Jocker ";

                        break;

                    default:
                        break;
                    }
                }

                cout << endl;
                
                cin >> choice;

                switch(choice) // user choice
                {
                    case 'Q':
                        
                        temp = Pieces::Queen;

                        break;

                    case 'K':

                        temp = Pieces::King;

                        break;

                    case 'R':

                        temp = Pieces::Rook;

                        break;

                    case 'N':

                        temp = Pieces::Knight;

                        break;

                    case 'B': 

                        temp = Pieces::Bishop;

                        break;

                    case 'P':

                        temp = Pieces::Pawn;

                        break;

                    case 'J':

                        temp = 69;

                        break;
                }

                for (int i = 0; i < 5; i++) // replace cards
                {
                    if(temp == handsdeckBlack[i])
                    {
                        handsdeckBlack[i] = blackDeck.top();

                        blackDeck.pop();
                    }
                }

                while (!movePlayerForRoyalChess(aiSide, temp))// check for legal move
                {                     
                    cout << RED << "Wrong figure, try again" << END << endl;

                    continue;
                }
                
                if(!position.pieces.pieceBitboards[Pieces::White][Pieces::Queen])
                {
                    // Check if game is finished
                    if (this->gameFinished())
                    {
                        cout << position << endl;

                        #if LOG_TO_FILE
                            log << position;
                        #endif

                        break;
                    }
                }
            }
        }
        else
        {
            while (true)
            {
                uint8_t temp;

                // Player 2 move
                cout << YELLOW << "\nPlayer 2 move:" << END << endl;

                #if LOG_TO_FILE
                    log << YELLOW << "\nPlayer 2 move:" << END;
                #endif

                std::cout << "Your deck: " << endl;

                for (int i = 0;i < 5;i++) // print deck
                {
                    switch (handsdeckBlack[i])
                    {
                    case Pieces::Pawn:
                        
                        cout << "Pawn ";

                        break;
                    case Pieces::Rook:
                        
                        cout << "Rook ";

                        break;

                    case Pieces::King:

                        cout << "King ";

                        break;

                    case Pieces::Queen:
                       
                        cout << "Queen ";

                        break;

                    case Pieces::Bishop:
                        
                        cout << "Bishop ";

                        break;

                    case Pieces::Knight:
                        
                        cout << "Knight ";

                        break;

                    case 69:

                        cout << "Jocker ";

                        break;

                    default:
                        break;
                    }
                }

                cout << endl;
                
                cin >> choice;

                switch(choice) // user choice
                {
                    case 'Q':
                        
                        temp = Pieces::Queen;

                        break;

                    case 'K':

                        temp = Pieces::King;

                        break;

                    case 'R':

                        temp = Pieces::Rook;

                        break;

                    case 'N':

                        temp = Pieces::Knight;

                        break;

                    case 'B': 

                        temp = Pieces::Bishop;

                        break;

                    case 'P':

                        temp = Pieces::Pawn;

                        break;

                    case 'J':

                        temp = 69;

                        break;
                }

                for (int i = 0; i < 5; i++) // replace cards
                {
                    if(temp == handsdeckBlack[i])
                    {
                        handsdeckBlack[i] = blackDeck.top();

                        blackDeck.pop();
                    }
                }

                while (!movePlayerForRoyalChess(aiSide, temp))// check for legal move
                {                     
                    cout << RED << "Wrong figure, try again" << END << endl;

                    continue;
                }
                
                if(!position.pieces.pieceBitboards[Pieces::White][Pieces::Queen])
                {
                    // Check if game is finished
                    if (this->gameFinished())
                    {
                        cout << position << endl;

                        #if LOG_TO_FILE
                            log << position;
                        #endif

                        break;
                    }
                }
                
                // Player 1 move
                cout << GREEN << "\nPlayer 1 move:" << END << endl;

                #if LOG_TO_FILE
                    log << GREEN << "\nPlayer 1 move:" << END;
                #endif

                std::cout << "Your deck: " << endl;

                for (int i = 0;i < 5;i++) // print deck
                {
                    switch (handsdeckWhite[i])
                    {
                    case Pieces::Pawn:
                        
                        cout << "Pawn ";

                        break;
                    case Pieces::Rook:
                        
                        cout << "Rook ";

                        break;

                    case Pieces::King:

                        cout << "King ";

                        break;

                    case Pieces::Queen:
                       
                        cout << "Queen ";

                        break;

                    case Pieces::Bishop:
                        
                        cout << "Bishop ";

                        break;

                    case Pieces::Knight:
                        
                        cout << "Knight ";

                        break;

                    case 69:

                        cout << "Jocker ";

                        break;

                    default:
                        break;
                    }
                }
                
                cout << endl;
                
                cin >> choice;

                switch(choice) // user choice
                {
                    case 'Q':
                        
                        temp = Pieces::Queen;

                        break;

                    case 'K':

                        temp = Pieces::King;

                        break;

                    case 'R':

                        temp = Pieces::Rook;

                        break;

                    case 'N':

                        temp = Pieces::Knight;

                        break;

                    case 'B': 

                        temp = Pieces::Bishop;

                        break;

                    case 'P':

                        temp = Pieces::Pawn;

                        break;

                    case 'J':

                        temp = 69;

                        break;
                }
                
                for (int i = 0; i < 5; i++) // replace cards
                {
                    if(temp == handsdeckWhite[i])
                    {
                        handsdeckWhite[i] = whiteDeck.top();

                        whiteDeck.pop();
                    }
                }

                while (!movePlayerForRoyalChess(playerSide, temp))// check for legal move
                {                     
                    cout << RED << "Wrong figure, try again" << END << endl;

                    continue;
                }

                if(!position.pieces.pieceBitboards[Pieces::Black][Pieces::Queen])
                {
                    // Check if game is finished
                    if (this->gameFinished())
                    {
                        cout << position << endl;

                        #if LOG_TO_FILE
                            log << position;
                        #endif

                        break;
                    } 
                }
            }
        }        

    }

    void Game::EvE()
    {
        playerSide = Pieces::White;
        aiSide = Pieces::Black;

        while (true)
        {
            cout << position << endl;

            #if LOG_TO_FILE
                log << position;
            #endif

            // AI 1 move
            move = ai.bestMove(position, playerSide, 0, 10000);
            position.move(move);

            // Check if game is finished
            if (this->gameFinished())
            {
                cout << position << endl;

                #if LOG_TO_FILE
                    log << position;
                #endif

                break;
            }

            cout << position << endl;

            #if LOG_TO_FILE
                log << position;
            #endif

            // AI 2 move
            move = ai.bestMove(position, aiSide, 0, 10000);
            position.move(move);

            // Check if game is finished
            if (this->gameFinished())
            {
                cout << position << endl;

                #if LOG_TO_FILE
                                log << position;
                #endif

                break;
            }
        }
    }

    void Game::PvPTime()
    {
        //Start game in which each side has 10 minutes to make a move use class Timer
        //If one side doesn't make a move in time, he loses the game
        //If both sides don't make a move in time, draw

        sideChoose();

        cout << position << endl;

        #if LOG_TO_FILE
            log << position;
        #endif

        //Start timer for white
        timerWhite.start();

        if(playerSide == Pieces::White)
        {
            while (true)
            {
                // Player 1 move
                cout << GREEN << "\nPlayer 1 move:" << END << endl;

                //Output time left in format mm:ss
                cout << "Time left (min:sec): " << (timerLose - timerWhite.getElapsedSeconds()) / 60 << ":" << (60 - (timerWhite.getElapsedSeconds() - timerWhite.getElapsedMinutes() * 60)) % 60 << endl;

                #if LOG_TO_FILE
                    log << GREEN << "\nPlayer 1 move:" << END;
                #endif

                while (!movePlayer(playerSide))
                {
                    continue;
                }

                // Check if game is finished
                if (this->gameFinished())
                {
                    cout << position << endl;

                    #if LOG_TO_FILE
                        log << position;
                    #endif

                    break;
                }

                //Stop timer for white
                timerWhite.stop();

                //Start timer for black
                timerBlack.start();

                // Player 2 move
                cout << YELLOW << "\nPlayer 2 move:" << END << endl;

                //Output time left in format mm:ss
                cout << "Time left (min:sec): " << (timerLose - timerBlack.getElapsedSeconds()) / 60 << ":" << (60 - (timerBlack.getElapsedSeconds() - timerBlack.getElapsedMinutes() * 60)) % 60 << endl;

                #if LOG_TO_FILE
                    log << YELLOW << "\nPlayer 2 move:" << END;
                #endif

                while (!movePlayer(aiSide))
                {
                    continue;
                }

                // Check if game is finished
                if (this->gameFinished())
                {
                    cout << position << endl;

                    #if LOG_TO_FILE
                        log << position;
                    #endif

                    break;
                }

                //Stop timer for black
                timerBlack.stop();

                //Start timer for white
                timerWhite.start();
            }
        } else
        {
            while (true)
            {
                // Player 1 move
                cout << GREEN << "\nPlayer 2 move:" << END << endl;

                //Output time left in format mm:ss
                cout << "Time left (min:sec): " << (timerLose - timerWhite.getElapsedSeconds()) / 60 << ":" << (60 - (timerWhite.getElapsedSeconds() - timerWhite.getElapsedMinutes() * 60)) % 60 << endl;

                #if LOG_TO_FILE
                    log << GREEN << "\nPlayer 2 move:" << END;
                #endif

                while (!movePlayer(aiSide))
                {
                    continue;
                }

                // Check if game is finished
                if (this->gameFinished())
                {
                    cout << position << endl;

                    #if LOG_TO_FILE
                        log << position;
                    #endif

                    break;
                }

                //Stop timer for white
                timerWhite.stop();

                //Start timer for black
                timerBlack.start();

                // Player 1 move
                cout << YELLOW << "\nPlayer 1 move:" << END << endl;

                //Output time left in format mm:ss
                cout << "Time left (min:sec): " << (timerLose - timerBlack.getElapsedSeconds()) / 60 << ":" << (60 - (timerBlack.getElapsedSeconds() - timerBlack.getElapsedMinutes() * 60)) % 60 << endl;

                #if LOG_TO_FILE
                    log << YELLOW << "\nPlayer 1 move:" << END;
                #endif

                while (!movePlayer(playerSide))
                {
                    continue;
                }

                // Check if game is finished
                if (this->gameFinished())
                {
                    cout << position << endl;

                    #if LOG_TO_FILE
                        log << position;
                    #endif

                    break;
                }

                //Stop timer for black
                timerBlack.stop();

                //Start timer for white
                timerWhite.start();
            }
        }
    }
    
    void Game::chooseLimitedTimeMode()
    {
        //Choose time mode for game (Blitz, Rapid, Classical)
        cout << "Choose time mode: " << endl;
        cout << "1. Blitz (5 min)" << endl;
        cout << "2. Rapid (10 min)" << endl;
        cout << "3. Classical (15 min)" << endl;

        int timeMode = 0;
        cin >> timeMode;

        switch (timeMode)
        {
            case 1:
                #if LOG_TO_FILE
                    log << "Time mode: Blitz (5 min)";
                #endif

                timerLose = 5 * 60; //5 minutes
                PvPTime();
            break;

            case 2:
                #if LOG_TO_FILE
                    log << "Time mode: Rapid (10 min)";
                #endif

                timerLose = 10 * 60; //10 minutes
                PvPTime();
            break;

            case 3:
                #if LOG_TO_FILE
                    log << "Time mode: Classical (15 min)";
                #endif

                timerLose = 15 * 60; //15 minutes
                PvPTime();
            break;

            default:
                cout << "Wrong input!" << endl;
                chooseLimitedTimeMode();
            break;
        }
    }

    

    void Game::chooseGameMode()
    {
        // Choose game mode
        cout << "Choose game mode: " << endl;
        cout << "1. Player vs Player" << endl;
        cout << "2. Player vs AI" << endl;
        cout << "3. AI vs AI" << endl;
        cout << "4. Player vs Player (10 min limit)" << endl;
        cout << "5. Gwent" << endl; // Gwent mode
        cout << "6. Royal Chess" << endl; // Royal Chess mode
        cout << "7. Tests" << endl;
        int gameMode = 0;
        cin >> gameMode;

        switch (gameMode)
        {
            case 1:
                #if LOG_TO_FILE
                    log << "Game mode: Player vs Player";
                #endif

                PvP();
            break;

            case 2:
                #if LOG_TO_FILE
                    log << "Game mode: Player vs AI";
                #endif

                PvE();
            break;

            case 3:
                #if LOG_TO_FILE
                    log << "Game mode: AI vs AI";
                #endif
                EvE();
            break;

            case 4:
                #if LOG_TO_FILE
                    log << "Game mode: Player vs Player with time";
                #endif

                timerWhite.reset();
                timerBlack.reset();

                chooseLimitedTimeMode();
            break;

            case 5:
                #if LOG_TO_FILE
                    log << "Game mode: Player vs AI";
                #endif

                Gwent();
            break;

            case 6:
                #if LOG_TO_FILE
                    log << "Game mode: Player vs Player with time";
                #endif

                RoyalChess();
            break;

            default:
                cout << RED << "\n[ERROR] Illegal game mode!\n" << END << endl;
                this->chooseGameMode();
            break;
        }
    }