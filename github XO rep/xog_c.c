#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//defining for the graphics and size
#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 600
#define SIZE 3
#define CELL_SIZE (WINDOW_WIDTH / SIZE)

//here are the prototypes for the functions of this code
void xoboard(SDL_Renderer *renderer, char board[SIZE][SIZE]);
void handleEvents(SDL_Event *event, char board[SIZE][SIZE], int *gameMode, int *currentPlayer, int *gameOver);
int showMenu();
void aiturn(char board[SIZE][SIZE]);
int check_winner(int n, int m, char mat[][m]);
int is_draw(char board[SIZE][SIZE]);
void drawCircle(SDL_Renderer *renderer, int x, int y, int radius);


int main()
{
    int gameMode = showMenu();  //1 for Player vs AI, 2 for AI vs AI, shows in the terminal before opening window
    
    //initializing the SDL library
    SDL_Init(SDL_INIT_VIDEO);
    //SDL window opens a window to visualize the board and the game, titled "Tic Tac Toe"
    SDL_Window *window = SDL_CreateWindow("Tic Tac Toe", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    //creating the renderer used to draw graphics
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    char board[SIZE][SIZE] = { {'-', '-', '-'}, {'-', '-', '-'}, {'-', '-', '-'} };
    int currentPlayer = 0;  // 0 for player's turn, 1 for AI's turn
    int gameOver = 0;

    while (!gameOver)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            handleEvents(&event, board, &gameMode, &currentPlayer, &gameOver);
        }
        xoboard(renderer, board);
        SDL_Delay(100);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

//asks for game mode  before opening window
int showMenu()
{
    int choice;
    printf("Select game mode:\n");
    printf("1. Player vs AI\n");
    printf("2. Player vs Player\n");
    printf("Enter your choice (1 or 2): ");
    scanf("%d", &choice);
    return (choice == 2) ? 2 : 1; //returns default to player vs AI if invalid input
}

void xoboard(SDL_Renderer *renderer, char board[SIZE][SIZE])
{
    //decided to go with a my melody/coquette color palette
    SDL_SetRenderDrawColor(renderer, 255, 182, 193, 255); // baby pink color for background
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); //white lines for grid
    for (int i = 1; i < SIZE; ++i)
    {
        SDL_RenderDrawLine(renderer, i * CELL_SIZE, 0, i * CELL_SIZE, WINDOW_HEIGHT);
        SDL_RenderDrawLine(renderer, 0, i * CELL_SIZE, WINDOW_WIDTH, i * CELL_SIZE);
    }

    //board content
    for (int i = 0; i < SIZE; ++i)
    {
        for (int j = 0; j < SIZE; ++j)
        {
            if (board[i][j] == 'X') 
            {
                // drawing X
                SDL_RenderDrawLine(renderer, j * CELL_SIZE + 10, i * CELL_SIZE + 10, (j + 1) * CELL_SIZE - 10, (i + 1) * CELL_SIZE - 10);
                SDL_RenderDrawLine(renderer, (j + 1) * CELL_SIZE - 10, i * CELL_SIZE + 10, j * CELL_SIZE + 10, (i + 1) * CELL_SIZE - 10);
            }
            else if (board[i][j] == 'O')
            {
                // drawing O (circle)
                drawCircle(renderer, j * CELL_SIZE + CELL_SIZE / 2, i * CELL_SIZE + CELL_SIZE / 2, CELL_SIZE / 3);
            }
        }
    }

    SDL_RenderPresent(renderer);
}

//function to draw a circle for O in SDL
void drawCircle(SDL_Renderer *renderer, int x, int y, int radius)
{
    int offsetX, offsetY, d;
    offsetX = 0;
    offsetY = radius;
    d = radius - 1;
    while (offsetY >= offsetX)
    {
        SDL_RenderDrawPoint(renderer, x + offsetX, y + offsetY);
        SDL_RenderDrawPoint(renderer, x + offsetY, y + offsetX);
        SDL_RenderDrawPoint(renderer, x - offsetX, y + offsetY);
        SDL_RenderDrawPoint(renderer, x - offsetY, y + offsetX);
        SDL_RenderDrawPoint(renderer, x + offsetX, y - offsetY);
        SDL_RenderDrawPoint(renderer, x + offsetY, y - offsetX);
        SDL_RenderDrawPoint(renderer, x - offsetX, y - offsetY);
        SDL_RenderDrawPoint(renderer, x - offsetY, y - offsetX);

        if (d >= 2 * offsetX)
        {
            d -= 2 * offsetX + 1;
            offsetX += 1;
        }
        else if (d < 2 * (radius - offsetY))
        {
            d += 2 * offsetY - 1;
            offsetY -= 1;
        }
        else
        {
            d += 2 * (offsetY - offsetX - 1);
            offsetY -= 1;
            offsetX += 1;
        }
    }
}

//function  to check the winner
int check_winner(int n, int m, char mat[][m])
{
    //if three cells x/o in one row horizontally are the same, x/o wins
    for (int i = 0; i < n; i++)
    {
        if (mat[i][0] == 'X' && mat[i][1] == 'X' && mat[i][2] == 'X') return 'X';
        if (mat[i][0] == 'O' && mat[i][1] == 'O' && mat[i][2] == 'O') return 'O';
    }
    //if three cells x/o in one row vertically are the same, x/o wins
    for (int j = 0; j < m; j++)
    {
        if (mat[0][j] == 'X' && mat[1][j] == 'X' && mat[2][j] == 'X') return 'X';
        if (mat[0][j] == 'O' && mat[1][j] == 'O' && mat[2][j] == 'O') return 'O';
    }
    //if three cells x/o in one row diagonally are the same, x/o wins
    if (mat[0][0] == 'X' && mat[1][1] == 'X' && mat[2][2] == 'X') return 'X';
    if (mat[0][0] == 'O' && mat[1][1] == 'O' && mat[2][2] == 'O') return 'O';
    if (mat[0][2] == 'X' && mat[1][1] == 'X' && mat[2][0] == 'X') return 'X';
    if (mat[0][2] == 'O' && mat[1][1] == 'O' && mat[2][0] == 'O') return 'O';
    
    //if none, it returns a D for draw
    return 'D';
}

//checking draw
int is_draw(char board[SIZE][SIZE])
{
    for (int i = 0; i < SIZE; ++i)
    {
        for (int j = 0; j < SIZE; ++j)
        {
            if (board[i][j] == '-')
            {
                return 0;  //returns 0 if there are still moves left
            }
        }
    }
    return 1;  //if no moves left, it's a draw, returns 1
}

//function for the ai's turn to play
void aiturn(char board[SIZE][SIZE])
{
    printf("AI is thinking...\n");
    SDL_Delay(1000); //a delay while ai is thinking

    //block user's moves
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++) 
        {
            if (board[i][j] == '-')
            {
                board[i][j] = 'X';  //as if it's user's move
                if (check_winner(SIZE, SIZE, board) == 'X')
                {
                    board[i][j] = 'O';  //if it is there, block move
                    printf("AI blocks at (%d, %d)\n", i, j);
                    return;
                }
                board[i][j] = '-';  //if not, then undo the block move
            }
        }
    }

    //if no blocking move is needed, ai makes a random move
    srand(time(NULL));
    int row, col;
    do
    {
        row = rand() % SIZE;
        col = rand() % SIZE;
    }
    while (board[row][col] != '-');
    board[row][col] = 'O';
    printf("AI randomly placed at (%d, %d)\n", row, col);
}

//function for event handeling
void handleEvents(SDL_Event *event, char board[SIZE][SIZE], int *gameMode, int *currentPlayer, int *gameOver)
{
    //if user closes window, game ends
    if (event->type == SDL_QUIT)
    {
        *gameOver = 1;
    }
    //handles mouse clicks
    else if (event->type == SDL_MOUSEBUTTONDOWN && !*gameOver)
    {
        int x, y;
        SDL_GetMouseState(&x, &y);
        int row = y / CELL_SIZE;
        int col = x / CELL_SIZE;

        if (row >= 0 && row < SIZE && col >= 0 && col < SIZE && board[row][col] == '-')
        {
            if (*gameMode == 1)  //player vs AI
            {
                if (*currentPlayer == 0)  //player's turn
                {
                    board[row][col] = 'X';  //player move
                    if (check_winner(SIZE, SIZE, board) == 'X')
                    {
                        *gameOver = 1;
                        printf("Player wins!\n");
                    }
                    else if (is_draw(board))
                    {
                        *gameOver = 1;
                        printf("It's a draw!\n");
                    }
                    else
                    {
                        *currentPlayer = 1;  //switches to AI
                        aiturn(board);
                        if (check_winner(SIZE, SIZE, board) == 'O')
                        {
                            *gameOver = 1;
                            printf("AI wins!\n");
                        }
                        else if (is_draw(board))
                        {
                            *gameOver = 1;
                            printf("It's a draw!\n");
                        }
                        else
                        {
                            *currentPlayer = 0;  //switches back to player
                        }
                    }
                }
            }
            else if (*gameMode == 2)  //player vs player
            {
                if (*currentPlayer == 0)  //player 1's turn
                {
                    board[row][col] = 'X';  //player 1 move
                    if (check_winner(SIZE, SIZE, board) == 'X')
                    {
                        *gameOver = 1;
                        printf("Player X wins!\n");
                    }
                    else if (is_draw(board))
                    {
                        *gameOver = 1;
                        printf("It's a draw!\n");
                    }
                    else
                    {
                        *currentPlayer = 1;  //goes to player 2
                    }
                }
                else if (*currentPlayer == 1)  //player 2's turn
                {
                    board[row][col] = 'O';  //player 2 move
                    if (check_winner(SIZE, SIZE, board) == 'O')
                    {
                        *gameOver = 1;
                        printf("Player O wins!\n");
                    }
                    else if (is_draw(board))
                    {
                        *gameOver = 1;
                        printf("It's a draw!\n");
                    }
                    else
                    {
                        *currentPlayer = 0;  //switches back to player 1
                    }
                }
            }
        }
    }
}