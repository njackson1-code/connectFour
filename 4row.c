#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>
//#include "level_2.h"

int colRow[] = {0,0,0,0,0,0,0};
int colRowB[] = {0,0,0,0,0,0,0};

char** create_game(){
    char** column = malloc(sizeof(char**)*7);

    for (int i = 0; i < 7; i ++){
        char *row = malloc(sizeof(char*) * 8);
        for (int r = 0; r < 6; r ++){
            row[r] = 'o';
        }
        
        column[i] = &row[0];
    }

    return column;
}

void print_game(char** game){
    printf("_______________\n");
    for (int r = 0; r < 6; r ++){
        printf("|");
        for (int c = 0; c < 7; c ++){
            if (c == 6){
                printf("%c", game[c][r]);
            }
            else {
                printf("%c ", game[c][r]);
            }
        }
        printf("|\n");
    }
    printf("---------------\n");
    printf("|             |\n");
    //printf("_______________\n");
}


bool makeMove(int column, char** game, char color){
    int row = colRow[column-1];
    if (5-row<0){
        return false;
    }
    game[column-1][5-row] = color;
    colRow[column-1] = colRow[column-1] + 1;
    return true;
}

bool threeRow(char** game, int col, int colChange, int row, int rowChange, char color, int* count){
    if (col < 0 || col > 6 || row < 0 || row > 5){
        return false;
    }
    //printf("col - %d; row - %d\n", col,row);
    //printf("%c\n", game[col][row]);
    if (game[col][row] != color){
        return false;
    }
    
    *count = *count + 1;
    threeRow(game,col+colChange, colChange, row+rowChange, rowChange, color,count);

    return false;

}

int threeRowCaller(char** game, int intArray[], int* rowCol){
    char color = 'B';
    int colMax = 0;
    for (int c = 0; c < 7; c++){
        int col = c;
        int row = 5-colRow[col];
        if (row < 0){
            continue;
        }
        int num = 1;

        
        int* count = &num;


        //diagonal increasing
        threeRow(game,col+1,1,row+1,1,color,count);
        //diagonal increasing
        threeRow(game,col-1,-1,row-1,-1,color,count);
        int diCount = *count;

        num = 1;
        *count = num;
        //diagonal increasing
        threeRow(game,col-1,-1,row+1,1,color,count);
        //diagonal increasing
        threeRow(game,col+1,1,row-1,-1,color,count);
        int ddCount = *count;

        num = 1;
        *count = num;
        //vertical increasing
        threeRow(game,col,0,row+1,1,color,count);
        //vertical decreasing
        threeRow(game,col,0,row-1,-1,color,count);
        int vCount = *count;


        num = 1;
        *count = num;
        //horizontal increasing
        threeRow(game,col+1,1,row,0,color,count);
        //horizontal decreasing
        threeRow(game,col-1,-1,row,0,color,count);
        int hCount = *count;
 


        //num = max(hCount, vCount, diCount, ddCount);
        int maxCheck[] = {hCount, vCount, diCount, ddCount};
        int max = 0;
        for (int i = 0; i <= 3; i ++){
            if (maxCheck[i]>max){
                max = maxCheck[i];
            }
        }
        if (max > colMax){
            colMax = max;
        }
        intArray[c] = max;
       
        
    }
    
    //int colMax = 0;
    //int newCol = 0;
    int count = 0;
    int newAr[7];
    for (int col=0; col<=6; col ++){
        int row = 5-colRow[col];
        printf("COL: %d val: %d\n", col, intArray[col]);
        if (intArray[col]>=colMax && row >=0){
            colMax = intArray[col];
            //newCol = col;
            newAr[count] = col;
            count ++;
        }
    }

    srand(time(NULL));
    int newCol = newAr[rand()%count];
    
    printf("newcol %d\n", newCol);
    return newCol;
}


bool gameOver(char** game, int col, int colChange, int row, int rowChange, char color, int* count){
    if (col < 0 || col > 6 || row < 0 || row > 5){
        return false;
    }
    if (game[col][row] != color){
        return false;
    }
    *count = *count + 1;
    gameOver(game,col+colChange, colChange, row+rowChange, rowChange, color,count);

    return false;
}


bool gameOverCaller(char** game, int col, char color){
    int row = 0;
    for (int i = 0; i <= 5; i++){
        if (game[col][i] != 'o'){
            row = i;
            break;
        }         
    }
      
    
    int num = 1;
    int* count = &num;
    //diagonal increasing
    gameOver(game,col+1,1,row+1,1,color,count);
    //diagonal increasing
    gameOver(game,col-1,-1,row-1,-1,color,count);
    if (*count >= 4){
        return true;
    }


    *count = 1;
    //diagonal increasing
    gameOver(game,col-1,-1,row+1,1,color,count);
    //diagonal increasing
    gameOver(game,col+1,1,row-1,-1,color,count);
    if (*count >= 4){
        return true;
    }


    *count = 1;
    //vertical increasing
    gameOver(game,col,0,row+1,1,color,count);
    //vertical decreasing
    gameOver(game,col,0,row-1,-1,color,count);
    printf("%d\n",*count);
    if (*count >= 4){
        return true;
    }
    *count = 1;


    //horizontal increasing
    gameOver(game,col+1,1,row,0,color,count);
    //horizontal decreasing
    gameOver(game,col-1,-1,row,0,color,count);
    if (*count >= 4){
        return true;
    }

    return false;
}

//void bestColumn(char** game,int* goodColumns);

int computerMove(char** game, int level){
    if (level == 1){
        srand(time(NULL));
        int col = rand()%6+1;
        makeMove(col, game, 'B');
        return col;
    }

    else if (level == 2){
        int* goodColumns = malloc(sizeof(int)*7);
        int intArray[7];
        for (int i = 0; i < 7; i ++){
            intArray[i] = 0;
        }
        int goodColumn = threeRowCaller(game, intArray, colRow);
        printf("%d\n", goodColumn);
        //int col = goodColumns[0] + 1;
        makeMove(goodColumn+1, game, 'B');
        return goodColumn;
    }

    else if (level == 3){
        //cuts off your options
        //knows possible wins patterns
    }

    return 0;
}


int main(int argc, char const *argv[])
{
    char** game = create_game();
    print_game(game);
    
    int spot = 0;
    bool notDone = true;

    printf("How difficult should the computer be? (1-Easy, 2-Intermediate, 3-Difficult)\n");
    char diffLevel[3];
    fgets(diffLevel,3,stdin);
    int level = diffLevel[0] - '0';


    printf("Begin game!\n");
    while (true){
        printf("Your turn\n");
        printf("Which column do you want to place it? ");
        while(true){
        char colInput[3];
        fgets(colInput,3,stdin);
        bool con = makeMove(colInput[0] - '0',game,'W');
        if (!con){
            printf("Column full! \nPlease enter value column - ");
            continue;
        }
        print_game(game);
        if (gameOverCaller(game,colInput[0] - '0'-1,'W')){
            printf("Game over! You win!\n");
            return 0;
        }
        break;
        }

        printf("\n\n");
        printf("Computer turn\n");
        sleep(1);
        spot = computerMove(game, level);
        print_game(game);
        if (gameOverCaller(game,spot,'B')){
            printf("Game over! Computer wins!\n");
            return 0;
        }
        printf("Next Turn\n");
        printf("---------\n");
    }


    return 0;
}


