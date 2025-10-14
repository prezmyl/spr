#include <array>
#include <iostream>

enum class Color { WHITE, BLACK };
enum class PieceType { KING, QUEEN, ROOK, BISHOP, KNIGHT, PAWN, EMPTY };
struct Position { int row; int col; };


class Element {
private:
    Color c;
    PieceType pt;
public:
    Element(Color c, PieceType pt) : c(c), pt(pt) {}
    Element() : c(Color::WHITE), pt(PieceType::EMPTY) {} //barva nema vyznam u prazdneho polizka
    static Element empty() { return Element(); }

    Color getColor() const {return c;}
    PieceType getPieceType() const {return pt;}
    bool isOccupied() const { return pt != PieceType::EMPTY; }

};


class Board {
private:
    int size;
    std::array<std::array<Element,8>,8> board;
public:
    Board(int size);
    bool inBound(Position p) const;
    int getSize() const {return this->size;}

    const Element& at(Position p) const { return board[p.row][p.col];}
    Element& at(Position p) {return board[p.row][p.col];}

    void set(Position p, PieceType pt, Color c) {board[p.row][p.col] = Element(c, pt);}
    void setEmpty(Position p) { board[p.row][p.col] = Element::empty(); }

    // Position findKing(Color c) const; NEpotrebuju -> primo pri vstupu. uzstrim nxn pruchod

    bool isKingInCheck(Color c, Position king_position) const;
};



Board::Board(int s) : size(s) {
}

bool Board::inBound(Position p) const{
    return p.row >= 0 && p.row < this->size && p.col >= 0 && p.col < this->size;
}


class Attack {
public:
    static bool pawnThreat (const Board& b, Position k, Color opp);
    static bool knightThreat(const Board& b, Position k, Color opp);
    static bool rayThreat   (const Board& b, Position k, int dr, int dc,
                             Color opp, PieceType t1, PieceType t2);
};

bool Attack::pawnThreat(const Board& b, Position k, Color opp) {
    int dr = (opp == Color::WHITE) ? 1 : -1;
    Position dc1 = {k.row + dr, k.col - 1};
    Position dc2 = {k.row + dr, k.col + 1};

    if (b.inBound(dc1)) {
        const Element& e = b.at(dc1);
        if (e.isOccupied() && e.getColor() == opp && e.getPieceType()==PieceType::PAWN)
            return true;
    }
    if (b.inBound(dc2)) {
        const Element& e = b.at(dc2);
        if (e.isOccupied() && e.getColor() == opp && e.getPieceType()==PieceType::PAWN)
            return true;
    }
    return false;
}

bool Attack::knightThreat(const Board& b, Position k, Color opp) {
    //8 moznych relativnich pozic pro Kone
    static const int L[8][2] = {
        {+2,+1},{+2,-1},{-2,+1},
        {-2,-1},{+1,+2},{+1,-2},{-1,+2},{-1,-2}
    };

    for (auto const& d : L) {
        Position p = {k.row + d[0], k.col + d[1]};
        if (!b.inBound(p)) continue;
        const Element& e = b.at(p);
        if (e.isOccupied() && e.getColor()==opp && e.getPieceType()==PieceType::KNIGHT) return true;
    }
    return false;
}

bool Attack::rayThreat(const Board& b, Position k, int dr, int dc, Color opp, PieceType t1, PieceType t2) {
    Position p = { k.row + dr, k.col + dc };
    while (b.inBound(p)) {
        const Element& e = b.at(p);
        if (e.isOccupied()) {
            if (e.getColor() == opp) {
                const PieceType pt = e.getPieceType();
                if (pt == t1 || pt == t2) return true;
            }
            return false;
        }
        p.row += dr; p.col += dc;
    }
    return false;
}


// reseni z pohledu krale -> kontroluju mozne hrozby vzhledem k druhum figurek
// O(1) + O(1) + O(n) = O(n) pro kotroly
bool Board::isKingInCheck(Color c, Position king_position) const {
    const Color opp = (c==Color::WHITE ? Color::BLACK : Color::WHITE);

    if (Attack::pawnThreat( *this, king_position, opp)) return true;
    if (Attack::knightThreat(*this, king_position, opp)) return true;


    static const int D[4][2] = {{+1,+1},{+1,-1},{-1,+1},{-1,-1}};
    static const int O[4][2] = {{+1,0},{-1,0},{0,+1},{0,-1}};
    // dagonals: strelec/dama
    for (auto const& d : D) {
        if (Attack::rayThreat(*this, king_position, d[0], d[1], opp, PieceType::BISHOP, PieceType::QUEEN)) return true;
    }
    //ortoganals: vez/dama
    for (auto const& d : O) {
        if (Attack::rayThreat(*this, king_position, d[0], d[1], opp, PieceType::ROOK, PieceType::QUEEN)) return true;
    }

    return false;
}

class Game {
private:
    Position whiteKing;
    Position blackKing;
    int boardSize;

    bool readInput(std::istream& is, Board& b, bool& allDots);
public:
    Game(int bs);
    void run(std::istream &is, std::ostream &os);

};
Game::Game(int bs) : boardSize(bs),  whiteKing({-1, -1}), blackKing({-1, -1}) {
}

// O(n) + O(n) = O(n^2) pro nacitani dat
bool Game::readInput(std::istream &is, Board &b, bool &allDots) {
    std::string line;
    int row = 0;
    allDots = true;
    whiteKing = {-1,-1};
    blackKing = {-1,-1};

    while (std::getline(is,line) && line.empty()) {/* skip oddelovace*/}
    if (!is) return false;

    for (; row < boardSize; row++) {
        if (row > 0) {
            if (!std::getline(is, line)) return false;
        }
        if ((int)line.size() < boardSize) return false;

        for (int col = 0; col < boardSize; col++) {
            char c = line[col];
            Position p = {row, col};
            if (c != '.') allDots = false;
            switch (c) {
                case '.': b.setEmpty(p); break;
                case 'K': b.set(p, PieceType::KING,   Color::WHITE); this->whiteKing = p; break;
                case 'Q': b.set(p, PieceType::QUEEN,  Color::WHITE); break;
                case 'R': b.set(p, PieceType::ROOK,   Color::WHITE); break;
                case 'B': b.set(p, PieceType::BISHOP, Color::WHITE); break;
                case 'N': b.set(p, PieceType::KNIGHT, Color::WHITE); break;
                case 'P': b.set(p, PieceType::PAWN,   Color::WHITE); break;
                case 'k': b.set(p, PieceType::KING,   Color::BLACK); this->blackKing = p; break;
                case 'q': b.set(p, PieceType::QUEEN,  Color::BLACK); break;
                case 'r': b.set(p, PieceType::ROOK,   Color::BLACK); break;
                case 'b': b.set(p, PieceType::BISHOP, Color::BLACK); break;
                case 'n': b.set(p, PieceType::KNIGHT, Color::BLACK); break;
                case 'p': b.set(p, PieceType::PAWN,   Color::BLACK); break;
                default: break;
            }
        }
    }

    return true;
}



void Game::run(std::istream &is, std::ostream &os) {
    int gameNum = 1;
    for (;;) {
        Board b(boardSize);
        bool allDots = true;

        if (!readInput(is, b, allDots)) return;
        if (allDots) return;

        const bool whiteInCheck = b.isKingInCheck(Color::WHITE, whiteKing);
        const bool blackInCheck = b.isKingInCheck(Color::BLACK, blackKing);

        if (whiteInCheck)
            os << "Game #" << gameNum++ << ": white king is in check.\n";
        else if (blackInCheck)
            os << "Game #" << gameNum++ << ": black king is in check.\n";
        else
            os << "Game #" << gameNum++ << ": no king is in check.\n";

    }
}




int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    const int size = 8;
    Game game(size);
    game.run(std::cin, std::cout);

    return 0;
}
