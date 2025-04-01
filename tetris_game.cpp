#include <iostream>
#include <vector>
#include <conio.h>    
#include <windows.h> 
#include <ctime>
#include <string>
#include <cstdlib>
using namespace std;

//-----------------------------------------------------------------------------------
//                              GLOBAL CONSTANTS (Updated Grid Size)
//-----------------------------------------------------------------------------------
const int WIDTH = 15;  
const int HEIGHT = 20;  
char grid[HEIGHT][WIDTH];
int score = 0;
int highScore = 0;     
int fallSpeed = 500;


HANDLE hConsole;

// Buffer for the previous frame to minimize redraws
char prevGrid[HEIGHT][WIDTH];
int prevScore = -1;
int prevHighScore = -1;

//-----------------------------------------------------------------------------------
//                              TETROMINO SHAPES
//-----------------------------------------------------------------------------------
vector<vector<vector<int>>> tetrominoes = {
    // I shape (ID = 1)
    {{1, 1, 1, 1}},
    // O shape (ID = 2)
    {{2, 2}, {2, 2}},
    // T shape (ID = 3)
    {{0, 3, 0}, {3, 3, 3}},
    // S shape (ID = 4)
    {{0, 4, 4}, {4, 4, 0}},
    // Z shape (ID = 5)
    {{5, 5, 0}, {0, 5, 5}},
    // L shape (ID = 6)
    {{6, 0, 0}, {6, 6, 6}},
    // J shape (ID = 7)
    {{0, 0, 7}, {7, 7, 7}}
};

class Tetromino {
    public:
    vector<vector<int>> shape;
    int x, y;
};

Tetromino currentTetromino;

//-----------------------------------------------------------------------------------
//                              UTILITY FUNCTIONS
//-----------------------------------------------------------------------------------
void initializeGrid() {
    for (int i = 0; i < HEIGHT; i++)
        for (int j = 0; j < WIDTH; j++)
            grid[i][j] = prevGrid[i][j] = '.';
}

// Sets the console cursor position (to update the screen in place)
void setCursorPosition(int x, int y) {
    COORD pos = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(hConsole, pos);
}

// Maps a cell's value ('1'..'7') to a specific console color.
void setColorForCell(char c) {
    switch (c) {
        case '1': 
            SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            break;
        case '2': 
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
            break;
        case '3': 
            SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            break;
        case '4': 
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            break;
        case '5': 
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
            break;
        case '6': 
            SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
            break;
        case '7':
            SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
            break;
        default:  
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            break;
    }
}

//-----------------------------------------------------------------------------------
//                              TITLE SCREEN
//-----------------------------------------------------------------------------------
string showTitleScreen() {
    system("cls");

    // Set up a creative welcome screen using Unicode box-drawing characters and multiple colors.
    // Top border in bright white
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    cout << "==========================================\n";

    // Title line with a different color (e.g., yellow)
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    cout << "|            WELCOME TO TETRIS           |\n";

    // Decorative separator in bright white
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    cout << "==========================================\n";

    // Tagline in light green
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    cout << "|   Prepare for the ultimate challenge!  |\n";

    // Bottom border in bright white
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    cout << "==========================================\n\n\n";

    // Prompt for user name in default bright white
    cout << "Enter your name: ";
    string userName;
    getline(cin, userName);
    if(userName.empty())
        userName = "Player";

    cout << "\nHello, " << userName << "!\n";
    cout << "Press any key to start the game...\n";
    _getch();
    system("cls");
    return userName;
}

//-----------------------------------------------------------------------------------
//                              RENDERING
//-----------------------------------------------------------------------------------
void printGrid(Tetromino &currentTetromino, bool gameOver = false) {
    // Create a temporary buffer from the grid
    char temp[HEIGHT][WIDTH];
    for (int i = 0; i < HEIGHT; i++)
        for (int j = 0; j < WIDTH; j++)
            temp[i][j] = grid[i][j];        

    // Overlay the active tetromino onto the buffer
    for (int i = 0; i < currentTetromino.shape.size(); i++) {
        for (int j = 0; j < currentTetromino.shape[i].size(); j++) {
            if (currentTetromino.shape[i][j])
                temp[currentTetromino.y + i][currentTetromino.x + j] = char('0' + currentTetromino.shape[i][j]);
        }
    }

    // Update the screen in place
    setCursorPosition(0, 0);

    // Only redraw the borders if it's the first frame
    if (prevScore == -1) {
        // Top border
        for (int i = 0; i < WIDTH + 2; i++) cout << "#";
        cout << "\n";
    }

    // Print each row with side borders, only updating changed cells
    for (int i = 0; i < HEIGHT; i++) {
        bool rowChanged = false;
        for (int j = 0; j < WIDTH; j++) {
            if (temp[i][j] != prevGrid[i][j]) {
                rowChanged = true;
                break;
            }
        }
        
        if (rowChanged || prevScore == -1) {
            setCursorPosition(0, i + 1); // +1 for top border
            cout << "|";
            for (int j = 0; j < WIDTH; j++) {
                if (temp[i][j] == '.') {
                    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                    cout << '.';
                } else {
                    setColorForCell(temp[i][j]);
                    cout << '#';
                }
                prevGrid[i][j] = temp[i][j];
            }
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
            cout << "|";
        }
    }
    
    // Only redraw the bottom border if it's the first frame
    if (prevScore == -1) {
        setCursorPosition(0, HEIGHT + 1);
        for (int i = 0; i < WIDTH + 2; i++) cout << "#";
        cout << "\n";
    }

    // Display score and high score only if they changed
    if (score != prevScore || highScore != prevHighScore || prevScore == -1) {
        setCursorPosition(0, HEIGHT + 2);
        cout << "Score: " << score << "    |    High Score: " << highScore << "    \n"; 
        prevScore = score;
        prevHighScore = highScore;
    }
    // Display game over message if needed
    if (gameOver) {
        int centerX = 1;
        setCursorPosition(centerX, HEIGHT / 2 - 1);
        SetConsoleTextAttribute(hConsole, BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        cout << "===============";
        
        setCursorPosition(centerX, HEIGHT / 2);
        SetConsoleTextAttribute(hConsole, BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_INTENSITY);
        cout << "  GAME OVER!   ";
        
        setCursorPosition(centerX, HEIGHT / 2 + 1);
        SetConsoleTextAttribute(hConsole, BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        cout << "===============";
        
        // Reset to normal colors
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    }
}

//-----------------------------------------------------------------------------------
//                              GAME LOGIC
//-----------------------------------------------------------------------------------
bool canMove(int dx, int dy, const vector<vector<int>> &shape) {
    for (int i = 0; i < shape.size(); i++) {
        for (int j = 0; j < shape[i].size(); j++) {
            if(shape[i][j]){
                int newX = currentTetromino.x + j + dx;
                int newY = currentTetromino.y + i + dy;
                if(newX < 0 || newX >= WIDTH || newY < 0 || newY >= HEIGHT)
                    return false;
                if(grid[newY][newX] != '.')
                    return false;
            }
        }
    }
    return true;
}

void rotateTetromino(){
    int rows = currentTetromino.shape.size();
    int cols = currentTetromino.shape[0].size();
    vector<vector<int>> rotated(cols, vector<int>(rows, 0));
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            rotated[j][rows - 1 - i] = currentTetromino.shape[i][j];
    if (canMove(0, 0, rotated))
        currentTetromino.shape = rotated;
}

void placeTetromino(){
    for (int i = 0; i < currentTetromino.shape.size(); i++)
        for (int j = 0; j < currentTetromino.shape[i].size(); j++)
            if (currentTetromino.shape[i][j])
                grid[currentTetromino.y + i][currentTetromino.x + j] = char('0' + currentTetromino.shape[i][j]);
} 

void spawnTetromino() {
    currentTetromino.shape = tetrominoes[rand() % tetrominoes.size()];
    currentTetromino.x = WIDTH / 2 - currentTetromino.shape[0].size() / 2;
    currentTetromino.y = 0;
    while (_kbhit()) _getch();
    // Let runGameSession() decide game over.
}
void clearLines() {
    for(int i = HEIGHT - 1; i >= 0; i--){
        bool full = true;
        for(int j = 0; j < WIDTH; j++){
            if (grid[i][j] == '.') {
                full = false;
                break;
            }
        }
        if (full) {
            for (int row = i; row > 0; row--)
                for (int col = 0; col < WIDTH; col++) {
                    grid[row][col] = grid[row - 1][col];
                    prevGrid[row][col] = '.'; // Force redraw of these rows
                } 
            for (int col = 0; col < WIDTH; col++) {
                grid[0][col] = '.';
                prevGrid[0][col] = '.';
            }
            score += 100;
            if(fallSpeed >=100) fallSpeed-=75;
            i++; // Re-check the same row after shifting down.
        }
    }
}

void handleInput() {
    while (_kbhit()) {
        int ch = _getch();
        if (ch == 224) {  // Arrow keys
            int arrow = _getch();
            if (arrow == 75 && canMove(-1, 0, currentTetromino.shape))
                currentTetromino.x--;
            else if (arrow == 77 && canMove(1, 0, currentTetromino.shape))
                currentTetromino.x++;
            else if (arrow == 80 && canMove(0, 1, currentTetromino.shape))
                currentTetromino.y++;
            else if (arrow == 72)
                rotateTetromino();
        } else {
            // WASD controls and Spacebar
            if (ch == 'a' || ch == 'A') {
                if (canMove(-1, 0, currentTetromino.shape))
                    currentTetromino.x--;
            } else if (ch == 'd' || ch == 'D') {
                if (canMove(1, 0, currentTetromino.shape))
                    currentTetromino.x++;
            } else if (ch == 's' || ch == 'S') {
                if (canMove(0, 1, currentTetromino.shape))
                    currentTetromino.y++;
            } else if (ch == 'w' || ch == 'W') {
                rotateTetromino();
            } else if (ch == 32) { // Spacebar for hard drop
                while (canMove(0, 1, currentTetromino.shape))
                    currentTetromino.y++;
            }
        }
    }
    Sleep(50);
}

int runGameSession() {
    system("cls"); // Clear screen for a fresh session.
    initializeGrid();
    score = 0;
    prevScore = -1; // Reset to force full redraw
    spawnTetromino();
    if (!canMove(0, 0, currentTetromino.shape)) {
        printGrid(currentTetromino, true); // Show game over immediately
        return score;
    }
    DWORD lastFallTime = GetTickCount();
     // Milliseconds per fall step

    while(true){
        printGrid(currentTetromino);
        handleInput();
        DWORD now = GetTickCount();
        if(now - lastFallTime >= fallSpeed){
            if (canMove(0, 1, currentTetromino.shape))
                currentTetromino.y++;
            else {
                placeTetromino();
                clearLines();
                spawnTetromino();
                if (!canMove(0, 0, currentTetromino.shape)) {
                    printGrid(currentTetromino, true); // Show game over with final board
                    return score;
                }
            }
            lastFallTime = now;
        }
        Sleep(30);
    }
}
//-----------------------------------------------------------------------------------
//                              MAIN
//-----------------------------------------------------------------------------------
int main(){
    srand((unsigned)time(0));
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    
    // Hide the cursor to reduce flickering
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
    
    string userName = showTitleScreen();
    bool exitGame = false;
    while (!exitGame) {
        int finalScore = runGameSession();
        if (finalScore > highScore)
            highScore = finalScore;
        
        // Position cursor below the game board
        setCursorPosition(0, HEIGHT + 4);
        
        // Show colored game over information
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        cout << "Game Over, " << userName << "!\n";
        SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        cout << "Your Score: " << finalScore << "\n";
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        cout << "High Score: " << highScore << "\n\n";
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        cout << "Press [R] to Restart or [X] to Exit.\n";
        
        bool validChoice = false;
        while (!validChoice) {
            if (_kbhit()) {
                char ch = _getch();
                if (ch == 'R' || ch == 'r')
                    validChoice = true;
                else if (ch == 'X' || ch == 'x') {
                    validChoice = true;
                    exitGame = true;
                }
            }
            Sleep(50);
        }
    }
    
    // Restore cursor visibility
    cursorInfo.bVisible = true;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
    
    system("cls");
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    cout << "Thanks for playing TETRIS, " << userName << "!\n";
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    cout << "Final High Score: " << highScore << "\n";
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    system("pause");
    return 0;
}
