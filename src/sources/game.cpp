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

    Move Game::getMove()
    {
        Move PlayerMove;
        string from, to;

        cout << "\nEnter move (Example: e2 e4): ";
        cin >> from >> to;

        PlayerMove.From = (from[0] - 'a') + (from[1] - '1') * 8;
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
        cout << position << endl;

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

    int RandFigure() // Генерирует индекс для фигуры, которую мы вызовем в колоду
    {
        int count = 0;

        count = rand() % 6;

        return count;
    }

    void Game::Gwent()// rm -f *.o 
    {
        const char* filename = "/home/danila/CCG/ProjectChess/src/DataBase/DB.db";

        sqlite3 *db;      
        handsdeck coloda;
        
        std::cout << "Create your deck:" << endl;

        deck bd;
        
        //Реализовать заполнение бд с учетом доступных для игрока очков
     
        int point = 200; //рандомное кол-во

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

        std::cout << "Составление колоды:" << std::endl;

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
        std::string sql = "SELECT * FROM DECK LIMIT 7;";
        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &s, NULL) == SQLITE_OK)
        {
            while (sqlite3_step(s) == SQLITE_ROW)
            {
                input.push_back(std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0))));
            }
            sqlite3_finalize(s);
        }
        //Реализовать рaндомное взятие кард из бд

        // for (int i = 0;i < input.size(); i++)
        // {
        //     cout << input[i] << " ";
        // }

        std::cout << "Your deck: " << endl;

        card obj_1(input[0], "test", " test_1");// Создаем 5 карт с рандомными фигурами 
        card obj_2(input[1], "test", " test_1");
        card obj_3(input[2], "test", " test_1");
        card obj_4(input[3], "test", " test_1");
        card obj_5(input[4], "test", " test_1");
       
    
        coloda.push_b(obj_1); // Кладем их в вектор
        coloda.push_b(obj_2);
        coloda.push_b(obj_3);
        coloda.push_b(obj_4);
        coloda.push_b(obj_5);


        // Выводим на экран колоду для пользователя
        
        coloda.print();        

        std::cout << endl;

        std::cout << "You can make two substitutions. Enter the card number (1-5) you want to replace. Enter 0 if you don't want to make substitutions at all." << endl;

        int t = 2;
        std::string temp = "sdfdsf ";

        std::cout << "test for rand from BD" << std::endl;

        std::cout << bd.get_random_value(db) << std::endl;

        while (t != 0) // Две замены карты (to do)
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

                coloda.CardChange(0, obj_1);
                
            }else 
            if (choice == 2)
            {
                temp = bd.get_random_value(db);
               
                obj_2.setFigure(temp);

                coloda.CardChange(1, obj_2);
                
            }else
            if (choice == 3)
            {
              temp = bd.get_random_value(db);
               
                obj_3.setFigure(temp);

                coloda.CardChange(2, obj_3);

            }else 
            if (choice == 4)
            {
               temp = bd.get_random_value(db);
               
                obj_4.setFigure(temp);

                coloda.CardChange(3, obj_4);

            }else 
            if (choice == 5)
            {
               temp = bd.get_random_value(db);
               
                obj_5.setFigure(temp);

                coloda.CardChange(4, obj_5);

            }else 
            if (choice > 5 || choice < 0 || double(choice))
            {
                std::cout << "Incorrect input. Please, try again" << endl;
                t++;
            }

            std::cout << "Updated deck " << endl;

            // Выводим на экран измененную колоду для пользователя
 
            coloda.print();

            std::cout << endl;

            t--;
        }	

    sqlite3_close(db);
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
            default:
                cout << RED << "\n[ERROR] Illegal game mode!\n" << END << endl;
                this->chooseGameMode();
            break;
        }
    }