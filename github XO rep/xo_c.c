#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 3

void printBoard(char board[SIZE][SIZE]);
int checkWin(char board[SIZE][SIZE], char player);
int isBoardFull(char board[SIZE][SIZE]);
void aiMove(char board[SIZE][SIZE]);

void print_my2darr(int n, int m, char mat[][m]){
    printf("    ");
    for (int j = 0; j < m; j++) {
        printf("%d      ", j);
    }
    printf("\n\n");

    for(int i = 0; i < n; i++){
        printf("%d   ", i);
        for(int j = 0; j < m; j++){
            printf("%c      ", mat[i][j]);
        }
        printf("\n\n");
    }
}

char check_winner(int n, int m, char mat[][m]){
  for(int i = 0; i < n; i++){
    if(mat[i][0] == 'X' && mat[i][1] == 'X' && mat[i][2] == 'X'){
      return 'X';
    }
    if(mat[i][0] == 'O' && mat[i][1] == 'O' && mat[i][2] == 'O'){
      return 'O';
    }
  }
  for(int j = 0; j < m; j++){
    if(mat[0][j] == 'X' && mat[1][j] == 'X' && mat[2][j] == 'X'){
      return 'X';
    }
    if(mat[0][j] == 'O' && mat[1][j] == 'O' && mat[2][j] == 'O'){
      return 'O';
    }
  }
    if(mat[0][0] == 'X' && mat[1][1] == 'X' && mat[2][2] == 'X'){
        return 'X';
    }
    if(mat[0][0] == 'O' && mat[1][1] == 'O' && mat[2][2] == 'O'){
        return 'O';
    }
    if(mat[0][2] == 'X' && mat[1][1] == 'X' && mat[2][0] == 'X'){
        return 'X';
    }
    if(mat[0][2] == 'O' && mat[1][1] == 'O' && mat[2][0] == 'O'){
        return 'O';
    }
    
    return 'D';
}

void aiMove(char board[SIZE][SIZE]) {
    printf("Computer's turn...\n");

    //checking the ai possible chances of winning
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] ==  '-') {
                board[i][j] = 'O'; //put o
                if (check_winner(SIZE, SIZE, board) == 'O') {
                    printf("AI places at (%d, %d)\n", i, j);
                    return; //if can win, return
                }
                board[i][j] = '-';  //then undo
            }
        }
    }

    //blocking user moves
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] == '-') {
                board[i][j] = 'X';  //tries user's next move
                if (check_winner(SIZE, SIZE, board) == 'X') {
                    board[i][j] = 'O';  // Block move
                    printf("AI blocks at (%d, %d)\n", i, j);
                    return; //again, if can win,return
                }
                board[i][j] = '-';  //then undo
            }
        }
    }

    //if cant win block from first time, then just random move
    srand(time(NULL));
    int row, col;
    do {
        row = rand() % SIZE;
        col = rand() % SIZE;
    } while (board[row][col] != '-');
    board[row][col] = 'O';
    printf("Randomly placed at (%d, %d)\n", row, col);
}

int main() {
    char mat[SIZE][SIZE] = {{'-', '-', '-'}, {'-', '-', '-'}, {'-', '-', '-'}};
    int row, col, gameMode;

    printf("Enter 1 for Player vs Player, 2 for Player vs AI: ");
    scanf("%d", &gameMode);

    for (int moves = 0; moves < 9; moves++) {
        print_my2darr(3, 3, mat);

        if (moves % 2 == 0) {
            // Player X's move
            printf("Player X enter the row: ");
            scanf("%d", &row);
            printf("Player X enter the column: ");
            scanf("%d", &col);

            if (row >= 0 && row < SIZE && col >= 0 && col < SIZE && mat[row][col] == '-') {
                mat[row][col] = 'X';
            } else {
                printf("invalid input, try again\n");
                moves--;
                continue;
            }

            //checking if user (x) wins
            char result = check_winner(SIZE, SIZE, mat);
            if (result == 'X') {
                printf("Player X wins!\n");
                print_my2darr(SIZE, SIZE, mat);
                return 0;
            }

            //ai turn (for user vs ai)
            if (gameMode == 2) {
                aiMove(mat);
                print_my2darr(SIZE, SIZE, mat);  //print board after AI move

                // Check if AI won
                result = check_winner(SIZE, SIZE, mat);
                if (result == 'O') {
                    printf("AI wins!\n");
                    print_my2darr(SIZE, SIZE, mat);
                    return 0;
                }
            }
        } else if (gameMode == 1) {
            // user O's move (for Player vs Player mode)
            printf("Player O enter the row: ");
            scanf("%d", &row);
            printf("Player O enter the column: ");
            scanf("%d", &col);

            if (row >= 0 && row < SIZE && col >= 0 && col < SIZE && mat[row][col] == '-') {
                mat[row][col] = 'O';
            } else {
                printf("invalid input, try again\n");
                moves--;
                continue;
            }

            //check if user O won
            char result = check_winner(SIZE, SIZE, mat);
            if (result == 'O') {
                printf("Player O wins!\n");
                print_my2darr(SIZE, SIZE, mat);
                return 0;
            }
        }
    }

    printf("It's a draw\n");
    print_my2darr(SIZE, SIZE, mat);
    return 0;
}