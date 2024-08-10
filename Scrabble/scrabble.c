#include "util/print_board.h"
#include "util/scrabble.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
void TASK0(char board[BOARD_SIZE][BOARD_SIZE])
{
    int i, j;
    for (i = 0; i < BOARD_SIZE; i++)
    {
        for (j = 0; j < BOARD_SIZE; j++)
        {
            board[i][j] = '.';
        }
    }
}
void TASK1(char board[BOARD_SIZE][BOARD_SIZE])
{
    TASK0(board);
    int i, j, N, lungime, coordy, coordx, dir;
    char *p, *h;
    char Num[2];
    char Num2[3];
    fgets(Num, 3, stdin);
    fgets(Num2, 4, stdin);
    h = strtok(Num2, "\n");
    N = atoi(h);
    char input[30];
    for (i = 0; i < N; i++)
    {
        fgets(input, 30, stdin);
        p = strtok(input, " ");
        //y1 = p;
        coordy = atoi(p);
        p = strtok(NULL, " ");
        //x1 = p;
        coordx = atoi(p);
        p = strtok(NULL, " ");
        //d1 = p;
        dir = atoi(p);
        p = strtok(NULL, "\n");
        lungime = strlen(p);
        if (dir == 1)
        {
            for (j = 0; j < lungime && coordy < BOARD_SIZE ; j++)
            {
                board[coordy][coordx] = p[j];
                coordy++;
            }
        }
        else
        {
            for (j = 0; j < lungime && coordx < BOARD_SIZE; j++)
            {
                board[coordy][coordx] = p[j];
                coordx++;
            }
        }
    }
    print_board(board);
}
int scor(int m)
{
    int score[25];
    score[0] = 1;
    score[1] = 3;
    score[2] = 3;
    score[3] = 2;
    score[4] = 1;
    score[5] = 4;
    score[6] = 2;
    score[7] = 4;
    score[8] = 1;
    score[9] = 8;
    score[10] = 5;
    score[11] = 1;
    score[12] = 3;
    score[13] = 1;
    score[14] = 1;
    score[15] = 3;
    score[16] = 10;
    score[17] = 1;
    score[18] = 1;
    score[19] = 1;
    score[20] = 1;
    score[21] = 4;
    score[22] = 4;
    score[23] = 8;
    score[24] = 4;
    score[25] = 10;
    return score[m];
}
void TASK2(int *task2w, int *task2p)
{
    int player1 = 0, player2 = 0;
    int i, k, N, lungime, Litera;
    char *p, *h;
    char Num[2];
    char Nlinii[3];
    fgets(Num, 3, stdin);
    fgets(Nlinii, 4, stdin);
    h = strtok(Nlinii, "\n");
    N = atoi(h);
    char input[30];
    for (i = 0; i < N; i++)
    {
        fgets(input, 30, stdin);
        p = strtok(input, " ");
        p = strtok(NULL, " ");
        p = strtok(NULL, " ");
        p = strtok(NULL, "\n");
        lungime = strlen(p);
        if (i % 2 == 0)
        {
            for (k = 0; k < lungime; k++)
            {
                Litera = *(p + k) - 'A';
                player1 = player1 + scor(Litera);
            }

        }
        else if (i % 2 == 1)
        {
            for (k = 0; k < lungime; k++)
            {
                Litera = *(p + k) - 'A';
                player2 = player2 + scor(Litera);
            }
        }
    }
    *task2w = player1;
    *task2p = player2;
}
void TASK3(int *task3w, int *task3p)
{
    int player1 = 0, player2 = 0, rezultat[20], rez1 = 0, rez2 = 0;
    int i, j, k, N, lungime, Litera, x, y, d, xcopy, ycopy;
    char *p, *h, *XX, *YY;
    char Num[2];
    char Nlinii[4];
    char String1[3];
    char String2[3];
    fgets(Num, 3, stdin);
    fgets(String1, 4, stdin);
    XX = strtok(String1, "\n");
    fgets(String2, 4, stdin);
    YY = strtok(String2, "\n");
    fgets(Nlinii, 4, stdin);
    h = strtok(Nlinii, "\n");
    N = atoi(h);
    char input[30];
    for (i = 0; i < N; i++)
    {
        fgets(input, 30, stdin);
        p = strtok(input, " ");
        y = atoi(p);
        p = strtok(NULL, " ");
        x = atoi(p);
        p = strtok(NULL, " ");
        d = atoi(p);
        p = strtok(NULL, "\n");
        lungime = strlen(p);
        xcopy = x;
        ycopy = y;
        if (i % 2 == 0)
        {
            player1 = 0;
            for (k = 0; k < lungime; k++)
            {
                Litera = *(p + k) - 'A';
                player1 = player1 + scor(Litera);
            }
            if (d == 0)
            {
                x = xcopy;
                y = ycopy;
                for (j = 0; j < lungime - 1; j++)
                {
                    if ((*(p + j) == *XX) && (*(p + j + 1) == *(XX + 1)))
                    {
                        for (k = 0; k < lungime; k++)
                        {
                            if (bonus_board[y][x] == 1)
                            {
                                player1 = player1 * 2;
                            }
                            ++x;
                        }
                    }
                }
                x = xcopy;
                if ((*(p + lungime - 2) == *YY) && (*(p + lungime - 1) == *(YY + 1)))
                {
                    for (k = 0; k < lungime; k++)
                    {

                        if (bonus_board[y][x] == 2)
                        {
                            player1 = player1 * 3;
                        }
                        ++x;
                    }
                }
            }
            if (d == 1)
            {
                x = xcopy;
                y = ycopy;
                for (j = 0; j < lungime; j++)
                {
                    if ((*(p + j) == *XX) && (*(p + j + 1) == *(XX + 1)))
                    {
                        for (k = 0; k < lungime; k++)
                        {
                            if (bonus_board[y][x] == 1)
                            {
                                player1 = player1 * 2;
                            }
                            ++y;
                        }
                    }
                }
                y = ycopy;
                if ((*(p + lungime - 2) == *YY) && (*(p + lungime - 1) == *(YY + 1)))
                {
                    for (k = 0; k < lungime; k++)
                    {
                        if (bonus_board[y][x] == 2)
                        {
                            player1 = player1 * 3;
                        }
                        ++y;
                    }
                }
            }
            rezultat[i] = player1;
        }
        if (i % 2 == 1)
        {
            player2 = 0;
            for (k = 0; k < lungime; k++)
            {
                Litera = *(p + k) - 'A';
                player2 = player2 + scor(Litera);
            }
            if (d == 0)
            {
                x = xcopy;
                y = ycopy;
                for (j = 0; j < lungime - 1; j++)
                {
                    if ((*(p + j) == *XX) && (*(p + j + 1) == *(XX + 1)))
                    {
                        for (k = 0; k < lungime; k++)
                        {
                            if (bonus_board[y][x] == 1)
                            {
                                player2 = player2 * 2;
                            }
                            ++x;
                        }
                    }
                }
                x = xcopy;
                if (*(p + (lungime - 2)) == *YY && *(p + (lungime - 1)) == *(YY + 1))
                {
                    for (k = 0; k < lungime; k++)
                    {
                        if (bonus_board[y][x] == 2)
                        {
                            player2 = player2 * 3;
                        }
                        ++x;
                    }
                }
            }
            if (d == 1)
            {
                x = xcopy;
                y = ycopy;
                for (j = 0; j < lungime - 1; j++)
                {
                    // printf("%c, %c | %c, %c\n", *(p + j), *XX, *(p + j + 1), *(XX + 1) );
                    if ((*(p + j) == *XX) && (*(p + j + 1) == *(XX + 1)))
                    {
                        for (k = 0; k < lungime; k++)
                        {
                            if (bonus_board[y][x] == 1)
                            {
                                player2 = player2 * 2;
                            }
                            ++y;
                        }
                    }
                }
                y = ycopy;
                if ((*(p + lungime - 2) == *YY) && (*(p + lungime - 1) == *(YY + 1)))
                {
                    for (k = 0; k < lungime; k++)
                    {
                        if (bonus_board[y][x] == 2)
                        {
                            player2 = player2 * 3;
                        }
                        ++y;
                    }
                }
            }
            rezultat[i] = player2;
        }
    }
    for (i = 0; i < N; i++)
        {
            if (i % 2 == 0)
            {
                rez1 = rez1 + rezultat[i];
            }
            else if (i % 2 == 1)
            {
                rez2 = rez2 + rezultat[i];
            }
        }
        *task3w = rez1;
        *task3p = rez2;

}
void TASK4(char board[BOARD_SIZE][BOARD_SIZE]){
    TASK0(board);
    int i, j, N, lungime, x, y, d;
    char *p, *h;// *XX, *YY;
    char Num[2];
    char Num2[4];
    char S1[3];
    char S2[3];
    fgets(Num, 3, stdin);
    fgets(S1, 4, stdin);
    //XX = strtok(S1, "\n");
    fgets(S2, 4, stdin);
    //YY = strtok(S2, "\n");
    fgets(Num2, 4, stdin);
    h = strtok(Num2, "\n");
    N = atoi(h);
    char input[30];
    for (i = 0; i < N; i++)
    {
        fgets(input, 30, stdin);
        p = strtok(input, " ");
        //y1 = p;
        y = atoi(p);
        p = strtok(NULL, " ");
        //x1 = p;
        x = atoi(p);
        p = strtok(NULL, " ");
        //d1 = p;
        d = atoi(p);
        p = strtok(NULL, "\n");
        for (int ii = 0; ii < 100; ii++)
        {
            if (strcmp(words[ii], p) == 0)//sunt la fel
            {
                strcpy(words[ii], "");
            }
        }
        lungime = strlen(p);
        if (d == 1)
        {
            for (j = 0; j < lungime && y < BOARD_SIZE; j++)
            {
                board[y][x] = p[j];
                y++;
            }
        }
        else
        {
            for (j = 0; j < lungime && x < BOARD_SIZE; j++)
            {
                board[y][x] = p[j];
                x++;
            }
        }
    }
    int ok = 0;
    for (int index = 0; index < 100 && ok == 0; index++)
    {
        for (i = 0; i < BOARD_SIZE && ok == 0; i++)
        {
            for (j = 0; j < BOARD_SIZE && ok == 0; j++)
            {
                if (board[i][j] == words[index][0])
                {
                    ok = 1;
                    int lungime2 = strlen(words[index]);
                    if (j + lungime2 <= BOARD_SIZE)
                    {
                        int index_j = j + 1;
                        int cnt = 1;
                        while (cnt <= lungime2 - 1)
                        {
                            if (board[i][index_j] != '.')
                            {
                                ok = 0;
                                break;
                            }
                            cnt++;
                            index_j++;
                        }
                        if (ok == 1)
                        {
                            index_j = j + 1;
                            cnt = 1;
                            while (cnt <= lungime2 - 1)
                            {
                                board[i][index_j] = words[index][cnt];
                                cnt++;
                                index_j++;
                            }
                        }
                    }
                    else ok = 0;
                    if (ok == 0)
                    {
                        ok = 1;
                        if (i + lungime2 <= BOARD_SIZE)
                        {
                            
                            int index_i = i + 1;
                            int cnt = 1;
                            while (cnt <= lungime2 - 1)
                            {
                                if (board[index_i][j] != '.')
                                {
                                    ok = 0;
                                    break;
                                }
                                cnt++;
                                index_i++;
                            }
                            if (ok == 1)
                            {
                                index_i = i + 1;
                                cnt = 1;
                                while (cnt <= lungime2 - 1)
                                {
                                    board[index_i][j] = words[index][cnt];
                                    cnt++;
                                    index_i++;
                                }
                            }
                        }
                        else ok = 0;
                    }
                }
            }
        }
    }
    print_board(board);
}
int main()
{
    int task2w, task2p, task3w, task3p;
    char board[BOARD_SIZE][BOARD_SIZE];
    char input[1];
    fgets(input, 2, stdin);
    if(input[0] == '0'){
    TASK0(board);
    print_board(board);
    }
    else if(input[0] == '1'){
    TASK1(board);
    }
    else if(input[0] == '2'){
    TASK2(&task2w, &task2p);
    printf("Player 1: %d Points\n", task2w);
    printf("Player 2: %d Points\n", task2p);
    }
    else if(input[0] == '3'){
    TASK3(&task3w, &task3p);
    printf("Player 1: %d Points\n", task3w);
    printf("Player 2: %d Points\n", task3p);
    }
    else if(input[0] == '4'){
    TASK4(board);    
    }
}