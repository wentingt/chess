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
	Type type;
	Color color;

	public: // *todo* delete
	// what is shared? 
	// the controlled (squares) are sharing the power of the controller (piece).
  	std::vector<std::shared_ptr<Square>> squares;

	public:
	// constructor
	Piece() : type(empty), color(none) {}

	Piece(Type type, Color color) : type(type), color(color) {}

	Type getType() {
		return type;
	}
	Color getColor() {
		return color;
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
	void clearSquares() {
		squares.clear();
	}
/*
	std::pair<int, int> getCount() {
		int cblack = 0;
		int cwhite = 0;

		for (auto square: squares) {
			// forward declaration of 'Square'
			switch(square->getPieceColor()) { 
				case black:
					cblack++;
					break;
				case white:
					cwhite++;
					break;
				default: 
				 	std::cout << "empty" << std::endl;	
					break;
			}
		}
		
		return std::make_pair(cblack, cwhite);
	}
*/
};

class Square: public std::enable_shared_from_this<Square> {
	public: // *todo* delete
	// what is unique? 
	// one piece can only be in one sqaure
	std::unique_ptr<Piece> pPiece; 

	public:
	// constructor
	Square() : pPiece(std::make_unique<Piece>()) {}

	std::shared_ptr<Square> getptr() {
		return shared_from_this();	
	}

	// like put down this piece from hand
	// never set an empty piece
	void setPiece(std::unique_ptr<Piece> pPiece) {
		this->pPiece = std::move(pPiece); // transfer ownership
	}

	// like hold this piece in hand, and put down an empty piece
	std::unique_ptr<Piece> getPiece() {
		std::unique_ptr<Piece> pPiece_ = std::move(pPiece);
		this->pPiece = std::make_unique<Piece>(empty, none);
		return std::move(pPiece_);
	}

	Type getPieceType() {
		return pPiece->getType();
	}
	Color getPieceColor() {
		return pPiece->getColor();
	}

	void print() {
		pPiece->print();
	}

	void addSquare(std::shared_ptr<Square> pSquare) {
		pPiece->addSquare(pSquare);
	}

	void clearSquares() {
		pPiece->clearSquares();
	}
/*
	std::pair<int, int> getCount() {
		return pPiece->getCount();
	}
*/
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

	void move(int srow, int scol, int erow, int ecol) {
		// cannot be in the same square
		assert(!(srow == erow && scol == ecol));

		pSquares[erow][ecol]->setPiece(pSquares[srow][scol]->getPiece());
	}

	void print() {
		mark();

		for (int i = 0; i < NUM_COLUMN; i += 1) {
			std::cout << '\t'  << (char) ('A' + i) << '\t';
		}
		std::cout << '\n';

		for (int i = 0; i < NUM_ROW; i += 1) {
			int row = NUM_ROW - i - 1;
			std::cout << row << '\t';

			for (int j = 0; j < NUM_COLUMN; j += 1) {
			 	pSquares[row][j]->print();
				std::cout << '\t';

				// number of square this piece on this square control
				std::cout << pSquares[row][j].use_count() - 1;
				std::cout << ",";

				// count of balck and white peices that contol this square
				std::cout << MAGENTA << getCount(pSquares[row][j]).first << RESET;
				std::cout << ",";
				std::cout << CYAN << getCount(pSquares[row][j]).second << RESET;
				std::cout << '\t';
			}
				
			std::cout << row << '\n';
		}

		for (int i = 0; i < NUM_COLUMN; i += 1) {
			std::cout << '\t'  << (char) ('A' + i) << '\t';
		}
		std::cout << '\n';

	}

	std::pair<int, int> getCount(std::shared_ptr<Square> pSquare) {
		int cblack = 0;
		int cwhite = 0;

		// rely on public member variables *todo*
		for (auto square: pSquare->pPiece->squares) {
			switch(square->getPieceColor()) {
				case black:
					cblack++;
					break;
				case white:
					cwhite++;
					break;
				default: 
				 	std::cout << "empty" << std::endl;	
					break;
			}
		}
		
		return std::make_pair(cblack, cwhite);
	}

	private:
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

	void clear() {
		for (int i = 0; i < NUM_ROW; i += 1) {
			for (int j = 0; j < NUM_COLUMN; j += 1) {
				pSquares[i][j]->clearSquares();
			}
		}
	}

	void mark() {
		clear();
		for (int i = 0; i < NUM_ROW; i += 1) {
			for (int j = 0; j < NUM_COLUMN; j += 1) {
				mark(i, j);
			}
		}
	}

	void mark(int row, int col) {
		switch (pSquares[row][col]->getPieceType()) {
			case rook: 
				markRF(row, col);
				mark(pSquares[row][col]);
				break;
			case bishop: 
				markD(row, col);
				mark(pSquares[row][col]);
				break;
			case queen: 
			case king: 
				markRF(row, col);
				markD(row, col);
				mark(pSquares[row][col]);
				break;
			case knight: 
				markL(row, col);
				mark(pSquares[row][col]);
				break;
			case pawn: 
				// special case *todo*
				mark(pSquares[row][col]);
				break;
			default: // empty
				break;
		}
	}

	// mark diagnol
	void markD(int row, int col) {
		for (int i = 1; i < std::min(row + 1, col + 1); i += 1) {
			if (!mark(pSquares[row][col], pSquares[row - i][col - i])) break;
		}
		for (int i = 1; i < std::min(NUM_ROW - row, NUM_COLUMN - col); i += 1) {
			if (!mark(pSquares[row][col], pSquares[row + i][col + i])) break;
		}
		for (int i = 1; i < std::min(NUM_ROW - row, col + 1); i += 1) {
			if (!mark(pSquares[row][col], pSquares[row + i][col - i])) break;
		}
		for (int i = 1; i < std::min(row + 1, NUM_COLUMN - col); i += 1) {
			if (!mark(pSquares[row][col], pSquares[row - i][col + i])) break;
		}
	}

	// mark rank and file
	void markRF(int row, int col) {
		for (int i = 1; i < row + 1; i += 1) {
			if (!mark(pSquares[row][col], pSquares[row - i][col])) break;
		}
		for (int i = 1; i < NUM_ROW - row; i += 1) {
			if (!mark(pSquares[row][col], pSquares[row + i][col])) break;
		}
		for (int i = 1; i < col + 1; i += 1) {
			if (!mark(pSquares[row][col], pSquares[row][col - i])) break;
		}
		for (int i = 1; i < NUM_COLUMN - col; i += 1) {
			if (!mark(pSquares[row][col], pSquares[row][col + i])) break;
		}
	}

	// mark L side
	void markL(int row, int col) {
		if (row > 1 && col > 0) {
			mark(pSquares[row][col], pSquares[row - 2][col - 1]);
		}
		if (row > 1 && col < NUM_COLUMN - 1) {
			mark(pSquares[row][col], pSquares[row - 2][col + 1]);
		}
		if (row > 0 && col > 1) {
			mark(pSquares[row][col], pSquares[row - 1][col - 2]);
		}
		if (row > 0 && col < NUM_COLUMN - 2) {
			mark(pSquares[row][col], pSquares[row - 1][col + 2]);
		}
		if (row < NUM_ROW - 1 && col > 1) {
			mark(pSquares[row][col], pSquares[row + 1][col - 2]);
		}
		if (row < NUM_ROW - 1 && col < NUM_COLUMN - 2) {
			mark(pSquares[row][col], pSquares[row + 1][col + 2]);
		}
		if (row < NUM_ROW - 2 && col > 0) {
			mark(pSquares[row][col], pSquares[row + 2][col - 1]);
		}
		if (row < NUM_ROW - 2 && col < NUM_COLUMN - 1) {
			mark(pSquares[row][col], pSquares[row + 2][col + 1]);
		}
	}

	// return false if should not continute marking
	bool mark(std::shared_ptr<Square> pSquare, std::shared_ptr<Square> mpSquare) {
		mpSquare->addSquare(pSquare->getptr());

		if (mpSquare->getPieceType() == empty) {
			if (pSquare->getPieceType() == king /*|| 
					pSquare->getPieceType() == knight*/) {
				return false;
			}
			return true;
		}
		return false;
	}

	// mark self
	void mark(std::shared_ptr<Square> pSquare) {
		pSquare->addSquare(pSquare->getptr());
	}

};

int main () {
	Board board;
	board.init();
	board.print();

	// simple test
	board.move(0,0,4,4);
	board.print();
	
	return 0;
}

/* todo */
// pawn basic, en passant, promotion
// castling
// check
