/*
 * =====================================================================================
 *
 *       Filename:  main.cc
 *    Description:  smart pointers 
 *
 *        Version:  1.0
 *        Created:  01/22/2018 10:55:25
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Wenting Tan (wt), wentingt@umich.edu
 *   Organization:  
 *
 * =====================================================================================
 */

#include <iostream>
#include <array>
#include <cassert>
#include <memory>
#include <vector>

//the following are UBUNTU/LINUX ONLY terminal color codes.
#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */


const int NUM_ROW = 8;
const int NUM_COLUMN = 8;

enum Type {
	empty,
	king,
	rook,
	bishop,
	queen,
	knight,
	pawn 
};

std::ostream& operator<<(std::ostream& os, Type type) {
	switch (type) {
		case empty:
			os << "empty";
			break;
		case king: 
			os << "king"; 
			break;
		case rook: 
			os << "rook"; 
			break;
		case bishop: 
			os << "bishop"; 
			break;
		case queen: 
			os << "queen"; 
			break;
		case knight: 
			os << "knight"; 
			break;
		case pawn: 
			os << "pawn"; 
			break;
		default:
			os.setstate(std::ios_base::failbit);
	}
	return os;
}

enum Color {
	none,
	black,
	white
};

std::ostream& operator<<(std::ostream& os, Color color) {
	switch(color) {
		case none:
			os << "none";
			break;
		case black:
			os << "black";
			break;
		case white:
			os << "white";
			break;
		default:
			os.setstate(std::ios_base::failbit);
	}
	return os;
}

class Square;
class Piece;
class Piece {
	public:
	Type type;
	Color color;
	// all sqaures controlled by this piece
  	std::vector<std::shared_ptr<Square>> squares;

	public:
	// constructor
	Piece() : type(empty), color(none) {}

	Piece(Type type, Color color) {
		this->type = type;
		this->color = color;
	}	

	void print() {
		if (type == empty) {
			std::cout << type;
		} else {
			std::cout << (color == white ? BOLDCYAN : BOLDMAGENTA) << type << RESET;
		}
	}

	void addSquare(std::shared_ptr<Square> pSquare) {
		squares.push_back(pSquare);
	}
};


class Square: public std::enable_shared_from_this<Square> {
	public:
	// uptr: one piece can only be in one sqaure
	std::unique_ptr<Piece> pPiece; // has piece, not shaded

	public:
	// constructor
	Square() : pPiece(std::make_unique<Piece>()) {}

	std::shared_ptr<Square> getptr() {
		return shared_from_this();	
	}

	void setPiece(std::unique_ptr<Piece> pPiece) {
		this->pPiece = std::move(pPiece); // transfer ownership
	}

	std::unique_ptr<Piece> getPiece() {
		std::unique_ptr<Piece> pPiece_ = std::move(pPiece);
		pPiece_->squares.clear();
		this->pPiece = std::make_unique<Piece>(empty, none);
		return std::move(pPiece_);
	}

	Type getPieceType() {
		return pPiece->type;
	}
	Color getPieceColor() {
		return pPiece->color;
	}

	void print() {
		pPiece->print();
	}

	void addSquare(std::shared_ptr<Square> pSquare) {
		pPiece->addSquare(pSquare);
	}

};

class Board {
	std::array<std::array<std::shared_ptr<Square>, NUM_COLUMN>, NUM_ROW> pSquares;

	public:

	Board() {
		for (int i = 0; i < NUM_ROW; i += 1) {
			for (int j = 0; j < NUM_COLUMN; j += 1) {
				assert(pSquares[i][j] == nullptr);
				pSquares[i][j] = std::make_shared<Square>();
			}
		}

	}

	void init() {
		init_(black);
		init_(white);
	}

	void init_(Color color) {
		// pawn
		int row = (color == white ? 2 : 7) - 1;
		for (int i = 0; i < NUM_COLUMN; i += 1) {
			pSquares[row][i]->setPiece(std::make_unique<Piece>(pawn, color));
		}
		// rook
		color == white? (row--) : (row++);
		int col = 0;
		int cor = NUM_COLUMN - 1;
		pSquares[row][col]->setPiece(std::make_unique<Piece>(rook, color));
		pSquares[row][cor]->setPiece(std::make_unique<Piece>(rook, color));
		// knight
		col++;
		cor--;
		pSquares[row][col]->setPiece(std::make_unique<Piece>(knight, color));
		pSquares[row][cor]->setPiece(std::make_unique<Piece>(knight, color));
		// bishop
		col++;
		cor--;
		pSquares[row][col]->setPiece(std::make_unique<Piece>(bishop, color));
		pSquares[row][cor]->setPiece(std::make_unique<Piece>(bishop, color));
		// queen
		col++;
		pSquares[row][col]->setPiece(std::make_unique<Piece>(queen, color));
		// king
		cor--;
		assert(col + 1 == cor);
		pSquares[row][cor]->setPiece(std::make_unique<Piece>(king, color));
	}

	void mark(int row, int col, Type type) {
	
	}

	void mark() {
		pSquares[0][0]->addSquare(pSquares[0][1]->getptr());
		pSquares[0][0]->addSquare(pSquares[1][0]->getptr());

		pSquares[0][1]->addSquare(pSquares[1][2]->getptr());
		pSquares[0][1]->addSquare(pSquares[1][0]->getptr());
	}

	void print() {
		for (int i = 0; i < NUM_ROW; i += 1) {
			int row = NUM_ROW - i - 1;
			for (int j = 0; j < NUM_COLUMN; j += 1) {
			 	pSquares[row][j]->print();

				std::cout << pSquares[row][j].use_count() - 1;

				std::cout << '\t';
			}
			std::cout << '\n';
		}

	}

	void move(int srow, int scol, int erow, int ecol) {
		// pSquares[erow][ecol]->setPiece(pSquares[srow][scol]->getPiece());
		pSquares[erow][ecol]->setPiece(std::make_unique<Piece>(
					pSquares[srow][scol]->getPieceType(),
					pSquares[srow][scol]->getPieceColor()));
		pSquares[srow][scol]->setPiece(std::make_unique<Piece>());
	}

};

int main () {
	Board board;
	board.init();
	board.print();

	board.mark();
	board.print();

	board.move(0,0,4,4);
	board.print();

	return 0;
}


// Todo:
// g++ main.cc -o chess -std=c++11
// 177: when piece move, clear vector
