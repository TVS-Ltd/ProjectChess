#include <string>
using namespace std;
class card
{
    private:
        char figure;
        string icon;
        string piece;
    public:
    // Constructor
    card(char fig, string icn, string pie);
    
    // Getters and setters
    char getFigure();
    void setFigure(char fig);

    string getIcon();
    void setIcon(string icn);

    string getPiece();
    void setPiece(string pie);
};
