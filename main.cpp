// Minesweeper Command Line
//
//  Rev Date    Name
//  0   012814  Benjamin J. Spivey, Ph.D.
//  1   020214  Benjamin J. Spivey, Ph.D.
//
//  Note:
//  This program is built to run on Windows 2000 or later as it uses the console resize functions



// Needed for the GetConsoleWindow() function for Windows 2000 or higher
#define _WIN32_WINNT 0x0500

#include <iostream>
#include <string>
#include <array>
#include <map>
#include <time.h>     // Used for initializing the random number generator
#include <limits>     // Used for clearing the cin buffer
#include <windows.h>  // Windows API header, used for resizing the console

//Classes

class Tile {
    int row, col;  // Assigns tile position
    int index;     // Assigns number of adjacent mines
    bool mine, hidden, marker;

    public:
        Tile() : row(0), col(0), index(0) {};
        Tile(const int &rowAssignment, const int &colAssignment) {
            row = rowAssignment;
            col = colAssignment;
            index = 0;
        }
        Tile(const Tile &o) {
            row = o.row;
            col = o.col;
            index = o.index;
        }
        int getRow() const{
            return row;
        }
        int getCol() const {
            return col;
        }
        void setMine(const bool mineAssignment) {
            mine = mineAssignment;
        }
        bool getMine() const {
            return mine;
        }
        void upIndex() {
            index++;
        }
        int getIndex() const {
            return index;
        }
        void setHidden(const bool hiddenAssignment) {
            hidden = hiddenAssignment;
        }
        bool getHidden() const {
            return hidden;
        }
        void setMarker(const bool markerAssignment) {
            marker = markerAssignment;
        }
        bool getMarker() const {
            return marker;
        }

};


class Board {
const static int numRows = 7;
const static int numCols = numRows;
int numMines; // Count number of unmarked mines

// Array key is used to facilitate element access with row & col user input
typedef std::array<int, 2> tilePosition;
std::map<tilePosition, Tile > boardTiles;

public:

    Board() {
            int row, col;
            srand (time(NULL));
            numMines = 0;

            //Generate tiles with random mine placement
            for (int i = 0; i < numRows; i++) {
                for (int j = 0; j < numCols; j++) {

                    //Create a new tile
                    row = i+1;
                    col = j+1;

                    Tile newTile(row,col);
                    boardTiles[{row,col}] = newTile;
                    boardTiles[{row,col}].setHidden(true);
                    boardTiles[{row,col}].setMarker(false);

                    //Drop mines
                    int mineWeight = rand() % 10;
                    if (mineWeight > 7) {
                        boardTiles[{row,col}].setMine(true);
                        numMines++;
                    }
                    else
                        boardTiles[{row,col}].setMine(false);
                }
            }
            // Count adjacent mines for each tile - the tile indices
            for (int i = 0; i < numRows; i++) {
                for (int j = 0; j < numCols; j++) {
                    row = i+1;
                    col = j+1;
                    int colM = col - 1;
                    int rowM = row - 1;
                    int colP = col + 1;
                    int rowP = row + 1;

                    //Check each tile for adjacent mines
                    if ( (col > 1) && (boardTiles[{row,colM}].getMine() == true)) //Left
                        boardTiles[{row,col}].upIndex();
                    if ( (row > 1) && (boardTiles[{rowM,col}].getMine() == true)) //Upper
                        boardTiles[{row,col}].upIndex();
                    if ( (col > 1) && (row > 1) && (boardTiles[{rowM,colM}].getMine() == true)) //Upper-left
                        boardTiles[{row,col}].upIndex();
                    if ( (row > 1) && (col < numCols) && (boardTiles[{rowM,colP}].getMine() == true)) //Upper-right
                        boardTiles[{row,col}].upIndex();
                    if ( (col < numCols) && (boardTiles[{row,colP}].getMine() == true)) //Right
                        boardTiles[{row,col}].upIndex();
                    if ( (row < numRows) && (boardTiles[{rowP,col}].getMine() == true)) //Lower
                        boardTiles[{row,col}].upIndex();
                    if ( (row < numRows) && (col > 1) && (boardTiles[{rowP,colM}].getMine() == true)) //Lower-left
                        boardTiles[{row,col}].upIndex();
                    if ( (row < numRows) && (col < numCols) && (boardTiles[{rowP,colP}].getMine() == true)) //Lower-right
                        boardTiles[{row,col}].upIndex();
                    }
                }

            // Automatically reveal a tile if it has a neighbor with index == 0
            // Each tile is set with hidden == true by default earlier in the constructor
            for (int i = 0; i < numRows; i++) {
                for (int j = 0; j < numCols; j++) {
                    row = i+1;
                    col = j+1;
                    int colM = col - 1;
                    int rowM = row - 1;
                    int colP = col + 1;
                    int rowP = row + 1;

                    // Check each tile for adjacent tiles with index == 0
                    // If any adjacent tiles have index == 0, the tile is revealed
                    if ( (col > 1) && (boardTiles[{row,colM}].getIndex() == 0)) //Left
                        boardTiles[{row,col}].setHidden(false);
                    if ( (row > 1) && (boardTiles[{rowM,col}].getIndex() == 0)) //Upper
                        boardTiles[{row,col}].setHidden(false);
                    if ( (col > 1) && (row > 1) && (boardTiles[{rowM,colM}].getIndex() == 0)) //Upper-left
                        boardTiles[{row,col}].setHidden(false);
                    if ( (row > 1) && (col < numCols) && (boardTiles[{rowM,colP}].getIndex() == 0)) //Upper-right
                        boardTiles[{row,col}].setHidden(false);
                    if ( (col < numCols) && (boardTiles[{row,colP}].getIndex() == 0)) //Right
                        boardTiles[{row,col}].setHidden(false);
                    if ( (row < numRows) && (boardTiles[{rowP,col}].getIndex() == 0)) //Lower
                        boardTiles[{row,col}].setHidden(false);
                    if ( (row < numRows) && (col > 1) && (boardTiles[{rowP,colM}].getIndex() == 0)) //Lower-left
                        boardTiles[{row,col}].setHidden(false);
                    if ( (row < numRows) && (col < numCols) && (boardTiles[{rowP,colP}].getIndex() == 0)) //Lower-right
                        boardTiles[{row,col}].setHidden(false);
                    }
                }

             }


    void printDividerLine() {
        for (int j = 0; j < numCols; j++) {
            std::cout << "*---";

            if (j == (numCols - 1))
                std::cout <<"*";
        }
        std::cout << std::endl;
    }

    void printTileLine(int row) {
        int col;
        for (int j = 0; j < numCols; j++) {
            col = j + 1;
            std::cout << "|" + std::to_string(row) + std::to_string(col) + " ";

            if (j == (numCols - 1))
                std::cout << "|";
        }
        std::cout << std::endl;
    }

    void printCenterLine (int row) {
        int col, index;
        bool hidden, mine, marker;

        for (int j = 0; j < numCols; j++) {
            col = j + 1;
            hidden = boardTiles[{row,col}].getHidden();
            mine = boardTiles[{row,col}].getMine();
            index = boardTiles[{row,col}].getIndex();
            marker = boardTiles[{row,col}].getMarker();
            std::cout << "| ";

            if (hidden && (index > 0 || mine) && !marker)
                std::cout << "H ";
            else if (!hidden && mine)
                std::cout << "M ";
            else if (!hidden && index > 0)
                std::cout << std::to_string(index) + " ";
            else if (marker)
                std::cout << "X ";
            else
                std::cout << "  ";

            if (j == (numCols - 1))
                std::cout << "|";
        }
        std::cout << std::endl;
    }

    void printBottomLine() {
        for (int j = 0; j < numCols; j++) {
            std::cout << "|   ";

            if (j == (numCols - 1))
                std::cout << "|";
        }
        std::cout << std::endl;
    }

    void printBoard() {
        for (int i = 0; i < numRows; i++) {
            int row = i + 1;
            printDividerLine();
            printTileLine(row);
            printCenterLine(row);
            printBottomLine();

            if (i == (numRows - 1))
                printDividerLine();
        }
    }

    void printHeader() {
        std::cout << std::endl;
        printDividerLine();
        std::cout << "* " << "MineSweeper Command Line v1.1" << std::endl;
        std::cout << "* " << std::endl;
        std::cout << "* " << "  Benjamin J. Spivey, Ph.D." << std::endl;
        std::cout << "* " << "  1/25/14" << std::endl;
        std::cout << "* " << std::endl;
        printDividerLine();
        std::cout << std::endl;
    }

    void setBoardHidden(const int row, const int col, bool hidden) {
        boardTiles[{row,col}].setHidden(hidden);
    }

    bool getBoardHidden(const int row, const int col) {
        return boardTiles[{row,col}].getHidden();
    }

    bool getBoardMine(const int row, const int col) {
        return boardTiles[{row,col}].getMine();
    }

    void decrementNumMines() {
        numMines--;
    }

    int getNumMines() const {
        return numMines;
    }

    void setBoardMarker(const int row, const int col, bool marker) {
        boardTiles[{row,col}].setMarker(marker);
    }

    bool checkBoardVictory () {
        int row, col;
        bool victory = false;

            if (numMines == 0)
                victory = true;
            else
                victory = false;

        return victory;
    }


};

// Functions

std::array<int, 2> tilePositionIO () {
        std::array<int, 2> tilePosition;
        std::cout << "Choose a tile." << std::endl;
        std::cout << "Enter the row: " << std::endl;
        std::cin  >> tilePosition[0];
        std::cout << "Enter the column: " << std::endl;
        std::cin  >> tilePosition[1];
        return tilePosition;
}

// Main

int main() {

    // Enlarge the windows console
    HWND handle = GetConsoleWindow();
    RECT r;
    GetWindowRect(handle, &r);
    MoveWindow(handle, r.left, r.top, 800, 600, TRUE);

    std::string userString;
    bool gameOver = false;
    bool victory  = false;
    std::array<int, 2> tilePosition;
    int tileRow, tileCol;

    Board newGame;
    newGame.printHeader();

    // Start game
    std::cout << "Would you like to start a new game? Enter Y (Yes) or N (No): " << std::endl;
    std::cin >> userString;
    if (userString == "N" || userString == "No" || userString == "n")
        return 0;
    else if (userString == "Y" || userString == "Yes" || userString == "y") {
        std::cout << std::endl;
        std::cout << "Number of Mines are = " << newGame.getNumMines();
        std::cout << std::endl;
        newGame.printBoard();
    }
    else {
        std::cout << std::endl;
        std::cout << "You entered an invalid response. Good bye!";
        std::cout << std::endl;
        return 1;
    }

    while (!gameOver) {

        //Start round by asking to reveal a tile or place a marker
        std::cout << "Choose to reveal a tile (T) or place a marker (M): " << std::endl;
        std::cin  >> userString;

        if ((userString == "T") || (userString == "t")) {
            // Ask user to reveal a tile
            tilePosition = tilePositionIO();
            tileRow = tilePosition[0];
            tileCol = tilePosition[1];

            // Modify the board
            newGame.setBoardHidden(tileRow, tileCol, false);
        }
        else if ((userString == "M") || (userString == "m")) {
            // Ask user to mark a tile
            tilePosition = tilePositionIO();
            tileRow = tilePosition[0];
            tileCol = tilePosition[1];

            // Modify the board
            newGame.setBoardMarker(tileRow, tileCol, true);

            // Decrement number of mines if user marks a mine location
            if (newGame.getBoardMine(tileRow, tileCol))
                newGame.decrementNumMines();
        }

        // Show the new board
         newGame.printBoard();

        // Win the game if you place markers on all mines
        victory = newGame.checkBoardVictory();
        if (victory) {
            std::cout << "You placed markers on all mines! You won the game!!" << std::endl;
            std::cout << std::endl;

            std::cout << "Press the Enter key to exit ..." << std::endl;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin.get();
            return 0;
        }
        // Game over if you land on a mine
        else if (!newGame.getBoardHidden(tileRow, tileCol) && newGame.getBoardMine(tileRow, tileCol)) {
            std::cout << "You stepped on a mine! Game Over!!!" << std::endl;
            std::cout << std::endl;
            gameOver = true;

            std::cout << "Press the Enter key to exit ..." << std::endl;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin.get();
            return 0;

        }

    }


	return 0;
 }


