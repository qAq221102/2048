#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <math.h>
#include <time.h>
void setconsole();
// default the console
// set the console in the mid of screen
// set width: 350, height: 450
// set the title: 2048
// set background: black, foreground: light yellow
void setposition();
void mode();
// choose mode
void chforeground();
// argument:
//      str: the str for print
//      num: for the number(2^nth) on the board, if num=1 means for end()
void score();
// argument:
//      cur_hstscore: current score
// read the huighest score in the file
// write the new highest score into the file
void setobject();
// argument:
//      board: 11*11 board included bounds
//      movesuc: 0 -> didnt move, 1 -> move successfully
// if movesuc=1 set a new number
// set the number on the board
void seteffect();
// argument:
//      board: 11*11 board included bounds
void showboard();
// argument:
//      board: 11*11 board included bounds
//      cur_hstscore: current score
// print the board & state
int move();
// argument:
//      board: 11*11 board included bounds
//      col: current col
//      row: current row
//      arrow: current arrow
//      ori_suc: original suc state
//      move_arr: 1*4 arr, carry the distance to move depends on arrow
//      merged_arr: 1*2 arr, record merged position
//      fixed_arr: 1*2 arr, record transmitted position
//      cur_hstscore: current score
// move num and handle effects
// return 0 -> still, 1 -> moved
void suc_showboard();
// argument:
//      board: 11*11 board included bounds
//      cur_hstscore: current score
//      suc: current moved-successful state
// after moved successful, print the board & state
int ewsn();
// argument:
//      board: 11*11 board included bounds
//      movesuc: 0 -> didnt move, 1 -> move successfully
//      cur_hstscore: current score
// detect the move & do the direction
// return 0 -> still play, 1 -> clicked "esc"
int judge();
// argument:
//      board: 11*11 board included bounds
//      win: 0 -> no 2048, 1 -> 2048
// check if cant move anymore or have 2048
// return 0-> still movable, 1 ->cant move
void easymode();
// argument:
//      board: 11*11 board included bounds
//      movable: 0 -> still movable, 1 -> cant move
// clear one block if unmovale
void end();
// argument:
//      board: 11*11 board included bounds
//      movable: 0 -> still movable, 1 -> cant move
//      win: 0 -> no 2048, 1 -> 2048
//      esc: 0-> still play, 1 -> clicked "esc"
// show win ending or game over(cant move or esc)
void endcartoon(); // ending cartoon
int modekey;       // 0: NORMAL, 1: BRICK, 3: SWIRL, 5: WORMHOLE, 7: EASY, 9: MIX
int main()
{
    srand(time(NULL));
    setconsole();
    modekey = 0;
    int board[11][11] = {0}; // inclided fences
    int movesuc = 0;         // 0: first time, 1: move successfully
    int movable = 0;         // 0: is still movable, 1: cant move
    int win = 0;             // 0: no 2048, 1: have 2048
    int cur_hstscore = 0;    // current score
    int esc = 0;             // 0: still, 1: give up
    mode();
    while (movable == 0 && esc == 0)
    {
        setobject(board, movesuc);
        showboard(board, cur_hstscore);
        esc = ewsn(board, &movesuc, &cur_hstscore);
        movable = judge(board, &win);
        easymode(board, &movable, win);
        end(board, &movable, &win, &esc);
    }
}
void setconsole()
{
    int scrwid = GetSystemMetrics(SM_CXSCREEN);
    int scrhei = GetSystemMetrics(SM_CYSCREEN);
    int cnswid = 400;                 // console wid
    int cnshei = 600;                 // console hei
    int left = (scrwid - cnswid) / 2; // left coordinate
    int top = (scrhei - cnshei) / 2;  // top coordinate
    MoveWindow(GetConsoleWindow(), left, top, cnswid, cnshei, TRUE);
    SetConsoleTitleW(L"2048"); // LPCWSTR
    system("color 0e");        // default background: black, foreground: light yellow
}
void setposition(int wid, int het)
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD crdsp;
    crdsp.X = wid;
    crdsp.Y = het;
    SetConsoleCursorPosition(handle, crdsp);
}
void mode()
{
    int arrow;
    COORD coord;
    coord.X = 13;
    coord.Y = 0;
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cci;
    GetConsoleCursorInfo(handle, &cci);
    cci.bVisible = FALSE;
    SetConsoleCursorInfo(handle, &cci);
    while (1)
    {
        setposition(0, 0);
        printf("| NORMAL   |   \n");
        printf("| SPECIAL v|   \n");
        if (coord.Y > 0) // open SPECIAL list
        {
            COORD crdspecial;
            crdspecial.X = 0;
            crdspecial.Y = 2;
            SetConsoleCursorPosition(handle, crdspecial);
            printf(" -BRICK    |   \n");
            printf(" -SWIRL    |   \n");
            printf(" -WORMHOLE |   \n");
            printf(" -EASY     |   \n");
            printf(" -MIX      |   \n");
        }
        else // clear the list
        {
            setposition(0, 0);
            printf("| NORMAL   |   \n");
            printf("| SPECIAL v|   \n");
            printf("               \n");
            printf("               \n");
            printf("               \n");
            printf("               \n");
            printf("               \n");
        }
        if (_kbhit()) // get a arrow
        {
            arrow = getch();
            if (arrow == 224)
            {
                arrow = getch();
                if (arrow == 72 || arrow == 80) // up or down
                {
                    switch (arrow) // do the arrow
                    {
                    case 72: // up
                        if (coord.Y == 0)
                            coord.Y = 0;
                        else
                            coord.Y--;
                        break;
                    case 80: // down
                        if (coord.Y == 6)
                            coord.Y = 6;
                        else
                            coord.Y++;
                        break;
                    }
                }
            }
            else if (arrow == 13) // enter
            {
                if (coord.Y == 1)
                    continue;
                else if (coord.Y == 2) // brick
                    modekey = 1;
                else if (coord.Y == 3) // swirl
                    modekey = 3;
                else if (coord.Y == 4) // wormhole
                    modekey = 5;
                else if (coord.Y == 5) // EASY
                    modekey = 7;
                else if (coord.Y == 6) // mix
                    modekey = 9;
                system("cls");
                return;
            }
        }
        SetConsoleCursorPosition(handle, coord);
        SetConsoleTextAttribute(handle, 7);
        printf("<<");
        Sleep(100);
        SetConsoleCursorPosition(handle, coord);
        printf("  ");
        Sleep(100);
        SetConsoleTextAttribute(handle, 14);
    }
}
void chforeground(const char *str, int num)
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE); // get handle
    int nthpower = log2(num);                        // check the degree of the num
    int numcolor = 0;                                // choose a color for the num
    int background = 0;                              // background: black
    if (nthpower == 0)                               // for win & game over
    {
        SetConsoleTextAttribute(handle, 4 | background); // foreground: red
        printf("%s", str);
    }
    else
    {
        for (numcolor = 1; numcolor <= nthpower; numcolor++) // set different color
        {
            if (numcolor == nthpower && numcolor < 6)
            {
                numcolor += 8;
                break;
            }
            else if (numcolor == nthpower && numcolor > 6)
            {
                numcolor -= 6;
                break;
            }
            else if (numcolor == nthpower && numcolor == 6)
            {
                break;
            }
        }
        SetConsoleTextAttribute(handle, numcolor | background); // change num color
        printf(" %4s ", str);
    }
    SetConsoleTextAttribute(handle, 14 | background); // default the console
}
void score(int cur_hstscore)
{
    FILE *file = fopen("highest score.txt", "r+");
    int hstscore;
    fscanf(file, "%d", &hstscore);
    printf("Current score: %d\n", cur_hstscore);
    if (cur_hstscore > hstscore) // write the new highest score into file
    {
        hstscore = cur_hstscore;
        fseek(file, 0, SEEK_SET);
        fprintf(file, "%d", cur_hstscore);
    }
    printf("The Highest Score: %d\n", hstscore);
    fclose(file);
}
void setobject(int (*board)[11], int movesuc)
{
    int col, row, ct;
    int rd_col, rd_row, rd_num;
    int rd_data[5] = {1, 3, 5, 7, 9};
    for (col = 1, ct = 0; col < 11; col += 2) // if the board is clear
    {
        for (row = 1; row < 11; row += 2)
        {
            if (board[col][row] == 0) // if board is all blank
            {
                ct++; // max is 25
            }
            if (movesuc == 1 && modekey % 2 == 1 &&
                (board[col][row] == 1 || board[col][row] == 3 || board[col][row] == 5))
            // clear the effect
            {
                board[col][row] = 0;
            }
        }
    }
    if (ct == 25) // initial
    {
        int n = 0;
        while (n < 2) // set the num
        {
            rd_num = pow(2, rand() % 2 + 1); // rand num will be 2^1 or 2^2
            rd_col = rd_data[rand() % 5];    // rand col
            rd_row = rd_data[rand() % 5];    // rand row
            if (board[rd_col][rd_row] == 0)
            {
                board[rd_col][rd_row] = rd_num;
                n++;
            }
        }
        seteffect(board);
    }
    else if (movesuc == 1) // after move successfully
    {
        int n = 0;
        int col, row;
        for (col = 1; col <= 9 && n == 0; col += 2)
        {
            for (row = 1; row <= 9 && n == 0; row += 2)
            {
                if (board[col][row] == 0)
                    n = 1;
            }
        }
        while (n == 1) // set the num, at least one blank
        {
            rd_num = pow(2, rand() % 2 + 1); // rand num will be 2^1 or 2^2
            rd_col = rd_data[rand() % 5];    // rand col
            rd_row = rd_data[rand() % 5];    // rand row
            if (board[rd_col][rd_row] == 0)
            {
                board[rd_col][rd_row] = rd_num; // set the number
                seteffect(board);
                break;
            }
        }
    }
}
void seteffect(int (*board)[11])
{
    int n = 0; // for count and effect number
    int rd_col = 0, rd_row = 0;
    int rd_data[5] = {1, 3, 5, 7, 9};
    while ((modekey == 1 || modekey == 9) && n < 1) // set BRICK
    {
        rd_col = rd_data[rand() % 5]; // rand col
        rd_row = rd_data[rand() % 5]; // rand row
        if (board[rd_col][rd_row] == 0)
        {
            board[rd_col][rd_row] = 1;
            n++;
        }
    }
    n = 0;
    while ((modekey == 3 || modekey == 9) && n < 1) // set SWIRL
    {
        rd_col = rd_data[rand() % 5]; // rand col
        rd_row = rd_data[rand() % 5]; // rand row
        if (board[rd_col][rd_row] == 0)
        {
            board[rd_col][rd_row] = 3;
            n++;
        }
    }
    n = 0;                                          // for count
    while ((modekey == 5 || modekey == 9) && n < 2) // set WORMHOLES
    {
        rd_col = rd_data[rand() % 5]; // rand col
        rd_row = rd_data[rand() % 5]; // rand row
        if (board[rd_col][rd_row] == 0)
        {
            board[rd_col][rd_row] = 5;
            n++;
        }
    }
}
void showboard(int (*board)[11], int cur_hstscore)
{
    setposition(0, 0); // easier to cover ori-board
    score(cur_hstscore);
    int col, row;
    for (col = 0; col < 11; col++)
    {
        for (row = 0; row < 11; row++)
        {
            if (col % 2 == 0 && row % 2 == 0) // the intersects
            {
                if (col % 2 == 0 && row == 0)
                {
                    printf("\n%c", ' ');
                }
                else if (col % 2 == 0 && row == 10)
                {
                    printf("%c\n", ' ');
                }
                else
                {
                    printf("%c", ' ');
                }
            }
            else if (col % 2 == 0 && (row % 4 == 1 || row % 4 == 3)) // horizon lines
            {
                printf("%s", "  --  ");
            }
            else if (col % 2 == 1 && row % 2 == 0) // verticle lines
            {
                printf("%s", "|");
            }
            else if ((modekey == 1 || modekey == 9) && board[col][row] == 1) // brick
            {
                printf("%2s%s%2s", " ", "##", " ");
            }
            else if ((modekey == 3 || modekey == 9) && board[col][row] == 3) // swirl
            {
                printf("%2s%s%2s", " ", "@)", " ");
            }
            else if ((modekey == 5 || modekey == 9) && board[col][row] == 5) // wormhole
            {
                printf("%2s%s%2s", " ", "OO", " ");
            }
            else if (board[col][row] % 2 == 0 && board[col][row] != 0) // the num
            {
                char str[5];
                sprintf(str, "%d", board[col][row]);
                chforeground(str, board[col][row]);
            }
            else
            {
                printf("%6c", ' ');
            }
        }
        printf("\n");
    }
}
int move(int (*board)[11], int col, int row, int arrow, int ori_suc,
         int *move_arr, int *merged_arr, int *fixed_arr, int *cur_hstscore)
{
    int ver_move[5] = {0}; // fifth value for special effect case
    int hor_move[5] = {0}; // fifth value for special effect case
    int move_index = 0;
    if (arrow == 72 || arrow == 80) // set vertical move distance
    {
        for (move_index = 0; move_index < 4; move_index++)
            ver_move[move_index] = move_arr[move_index];
    }
    if (arrow == 75 || arrow == 77) // set horizon move distance
    {
        for (move_index = 0; move_index < 4; move_index++)
            hor_move[move_index] = move_arr[move_index];
    }
    int newcol = col + ver_move[0]; // new check
    int newrow = row + hor_move[0];
    if (board[newcol][newrow] == 0 || board[newcol][newrow] == board[col][row])
        move_index = 0;
    int ct_index = 0;
    for (ct_index = 1; ct_index < 4; ct_index++) // count the distance to move
    {
        // new check
        newcol = col + ver_move[ct_index];
        newrow = row + hor_move[ct_index];
        if ((board[newcol][newrow] == 0 || board[newcol][newrow] == board[col][row]) &&
            (newcol >= 1 && newcol <= 9 && newrow >= 1 && newrow <= 9))
        {
            if (newcol == merged_arr[0] && newrow == merged_arr[1])
                break;
            if (move_index == ct_index - 1)
                move_index = ct_index;
        }
    }
    // deal wite effect, if didnt move then current mvoe_index is 4, in arr is 0
    newcol = col + ver_move[move_index];
    newrow = row + hor_move[move_index];
    int eft_newcol = newcol + ver_move[0];
    int eft_newrow = newrow + hor_move[0];
    if (eft_newcol >= 1 && eft_newcol <= 9 && eft_newrow >= 1 && eft_newrow <= 9)
    {
        if (board[eft_newcol][eft_newrow] == 3) // bump into SWIRL
        {
            board[col][row] = 0;               // clear the num
            board[eft_newcol][eft_newrow] = 0; // clear the SWIRL
            return 1;                          // moved is success
        }
        if (board[eft_newcol][eft_newrow] == 5) // bump into WORMHOLE
        {
            int wormcol, wormrow;
            for (wormcol = 1; wormcol <= 9; wormcol += 2) // find another WORMHOLE
            {
                for (wormrow = 1; wormrow <= 9; wormrow += 2)
                {
                    if (wormcol == eft_newcol && wormrow == eft_newrow) // skip the ori-wormhole
                        continue;
                    else if (board[wormcol][wormrow] == 5)
                    {
                        board[eft_newcol][eft_newrow] = 0;         // clear the wormhole
                        board[wormcol][wormrow] = board[col][row]; // new wormhole, deliver the num
                        fixed_arr[0] = wormcol;                    // record the transmitted num
                        fixed_arr[1] = wormrow;
                        board[col][row] = 0; // clear the original num
                        return 1;
                    }
                }
            }
        }
    }
    // if didnt move, move_index is still 4
    if (move_index < 4) // if moved means suc
    {
        if (board[newcol][newrow] == board[col][row]) // two values are equal, get score
        {
            *cur_hstscore = *cur_hstscore + board[col][row] * 2;
            merged_arr[0] = newcol;
            merged_arr[1] = newrow;
        }
        board[newcol][newrow] += board[col][row];
        board[col][row] = 0;
        return 1; // moved
    }
    if (ori_suc == 1) // already success
        return 1;
    return 0; // didnt move
}
void suc_showboard(int (*board)[11], int cur_hstscore, int suc)
{
    if (suc == 1) // print at most 4 times
    {
        showboard(board, cur_hstscore);
        Sleep(125);
    }
}
int ewsn(int (*board)[11], int *movesuc, int *cur_hstscore)
{
    int col = 0, row = 0;
    int arrow = 0;
    int suc = 0;
    *movesuc = 0;
    while (1) // let program into loop
    {
        if (_kbhit())
        {
            arrow = getch();
            if (arrow == 224 || arrow == 0) // arrow key
            {
                arrow = getch();
                break;
            }
            else if (arrow == 27)
                return 1;
        }
        COORD crd00 = {0, 0};
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), crd00);
    }
    while (_kbhit()) // clear kb input buffer
        getch();
    int fixed_arr[] = {0, 0};                        // for fix the transmitted num
    int merged_arr[] = {0, 0};                       // for prevent chain merge bug
    for (col = 3; col <= 9 && arrow == 72; col += 2) // up
    {
        for (row = 1; row <= 9; row += 2)
        {
            if (board[col][row] % 2 == 0 && board[col][row] != 0) // locate the num
            {
                if (col == fixed_arr[0] && row == fixed_arr[1]) // let transmitted num fixed
                    continue;
                int ver_move[] = {-2, -4, -6, -8};
                suc = move(board, col, row, arrow, suc, ver_move, merged_arr, fixed_arr, cur_hstscore);
            }
        }
        *movesuc = suc;
        suc_showboard(board, *cur_hstscore, suc);
    }
    for (col = 7; col >= 1 && arrow == 80; col -= 2) // down
    {
        for (row = 1; row <= 9; row += 2)
        {
            if (board[col][row] % 2 == 0 && board[col][row] != 0) // locate the num
            {
                if (col == fixed_arr[0] && row == fixed_arr[1]) // let transmitted num fixed
                    continue;
                int ver_move[] = {2, 4, 6, 8};
                suc = move(board, col, row, arrow, suc, ver_move, merged_arr, fixed_arr, cur_hstscore);
            }
        }
        *movesuc = suc;
        suc_showboard(board, *cur_hstscore, suc);
    }
    for (row = 3; row <= 9 && arrow == 75; row += 2) // left
    {
        for (col = 1; col <= 9; col += 2)
        {
            if (board[col][row] % 2 == 0 && board[col][row] != 0) // locate the num
            {
                if (col == fixed_arr[0] && row == fixed_arr[1]) // let transmitted num fixed
                    continue;
                int hor_move[] = {-2, -4, -6, -8};
                suc = move(board, col, row, arrow, suc, hor_move, merged_arr, fixed_arr, cur_hstscore);
            }
        }
        *movesuc = suc;
        suc_showboard(board, *cur_hstscore, suc);
    }
    for (row = 7; row >= 1 && arrow == 77; row -= 2) // right
    {
        for (col = 1; col <= 9; col += 2)
        {
            if (board[col][row] % 2 == 0 && board[col][row] != 0) // locate the num
            {
                if (col == fixed_arr[0] && row == fixed_arr[1]) // let transmitted num fixed
                    continue;
                int hor_move[] = {2, 4, 6, 8};
                suc = move(board, col, row, arrow, suc, hor_move, merged_arr, fixed_arr, cur_hstscore);
            }
        }
        *movesuc = suc;
        suc_showboard(board, *cur_hstscore, suc);
    }
    return 0; // didnt esc
}
int judge(int (*board)[11], int *win)
{
    int col = 0, row = 0, y = 0, x = 0;
    for (col = 1; col <= 9; col += 2) // check 2048
    {
        for (row = 1; row <= 9; row += 2)
        {
            if (board[col][row] == 2048)
            {
                *win = 1;
                return 1; // unmovable
            }
        }
    }
    for (col = 1; col <= 9; col += 2)
    {
        for (row = 1; row <= 9; row += 2)
        {
            for (y = -2; y <= 2; y += 4) // up & down
            {
                if (col + y >= 1 && col + y <= 9)
                {
                    if (board[col + y][row] == 0 || board[col + y][row] == 3 /*swirl*/ ||
                        board[col + y][row] == 5 /*wormhole*/ || board[col + y][row] == board[col][row])
                    {
                        return 0; // movable
                    }
                }
            }
            for (x = -2; x <= 2; x += 4) // left & right
            {
                if (row + x >= 1 && row + x <= 9)
                {

                    if (board[col][row + x] == 0 || board[col][row + x] == 3 /*swirl*/ ||
                        board[col][row + x] == 5 /*wormholes*/ || board[col][row + x] == board[col][row])
                    {
                        return 0; // movable
                    }
                }
            }
        }
    }
    return 1; // unmovable
}
void easymode(int (*board)[11], int *movable, int win)
{
    if ((modekey == 7 || modekey == 9) && *movable == 1 && win == 0)
    {
        int col, row;
        int max = board[1][1];
        int max_col = 1, max_row = 1;
        int index[] = {1, 3, 5, 7, 9};
        for (col = 1; col <= 9; col += 2) // find maximum's position
        {
            for (row = 1; row <= 9; row += 2)
            {
                if (board[col][row] >= max)
                {
                    max = board[col][row];
                    max_col = col;
                    max_row = row;
                }
            }
        }
        int rd_col = 0, rd_row = 0;
        while (1) // clear a number, skip maximum
        {
            rd_col = index[rand() % 5];
            rd_row = index[rand() % 5];
            if ((rd_col == max_col && rd_row == max_row) && board[rd_col][rd_row] % 2 == 0)
            {
                continue;
            }
            board[rd_col][rd_row] = 0;
            *movable = 0;
            break;
        }
    }
}
void end(int (*board)[11], int *movable, int *win, int *esc)
{
    if (*win == 1 || *movable == 1 || *esc == 1)
    {
        COORD crdend = {0, 25};
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), crdend);
        if (*win == 1) // 2048
        {
            chforeground("!> |CONGRATULATION| <!\n", 1);
            chforeground("you achieve 2048\n", 1);
        }
        else if (*movable == 1 || *esc == 1) // cant move or esc
        {
            chforeground("- QAQ |GAME OVER| QAQ -\n", 1);
            chforeground("you cant move anymore\n", 1);
        }
        printf("----------------\n");
        printf("QUIT   |");
        CONSOLE_SCREEN_BUFFER_INFO csbi; // get cursor position
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
        int cur_x = csbi.dwCursorPosition.X;
        int cur_y = csbi.dwCursorPosition.Y;
        // POINT basecursor;
        // GetCursorPos(&basecursor); // get cursor position
        printf("\n");
        printf("REPLAY |");
        COORD endcrd = {cur_x + 2, cur_y};
        while (1)
        {
            if (_kbhit()) // detect the arrow
            {
                int choose = getch();
                if (choose == 224)
                {
                    choose = getch();
                    int y = 0;
                    if (choose == 72) // up
                    {
                        y = -1;
                        if (endcrd.Y == cur_y)
                            y = 0;
                    }
                    if (choose == 80) // down
                    {
                        y = 1;
                        if (endcrd.Y == cur_y + 1)
                            y = 0;
                    }
                    endcrd.Y += y;
                }
                else if (choose == 13) // enter
                {
                    if (endcrd.Y == cur_y) // QUIT
                        endcartoon();
                    else if (endcrd.Y == cur_y + 1) // REPLAY
                    {
                        int col, row;
                        for (col = 1; col <= 9; col += 2)
                        {
                            for (row = 1; row <= 11; row += 2)
                            {
                                board[col][row] = 0;
                            }
                        }
                        modekey = 0;
                        *win = 0;
                        *movable = 0;
                        *esc = 0;
                        system("cls");
                        mode(); // rechoose the mode
                        return;
                    }
                }
            }
            while (_kbhit()) // clear kb input buffer
                getch();
            setposition(endcrd.X, endcrd.Y);
            printf("<<");
            Sleep(100);
            setposition(endcrd.X, endcrd.Y);
            printf("  ");
            Sleep(100);
        }
    }
}
void endcartoon()
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleScreenBufferInfo(handle, &csbi);
    GetConsoleCursorInfo(handle, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(handle, &cursorInfo); // set cursor unvisible
    int col = 0;
    int row = 0;
    int right = csbi.srWindow.Right;
    int bottom = csbi.srWindow.Bottom;
    SetConsoleTextAttribute(handle, 7);
    for (col = 0; col <= bottom / 2; col++)
    {
        for (row = 0; row <= right; row++)
        {
            COORD crdcartoon;
            crdcartoon.X = row;
            crdcartoon.Y = col;
            SetConsoleCursorPosition(handle, crdcartoon);
            printf(" ");
        }
        for (row = 0; row <= right; row++)
        {
            COORD crdcartoon;
            crdcartoon.X = row;
            crdcartoon.Y = bottom - col;
            SetConsoleCursorPosition(handle, crdcartoon);
            printf(" ");
        }
        Sleep(50);
    }
    exit(0);
}