#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <curses.h>
#include <signal.h>
#include <sys/time.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define BLANK " "

char symbol[1]; 
int row;
int col;
int xdir;
int ydir;
int delay, ndelay;
int colR, toprowR, botrowR;
int top_row, bot_row;
int l_edge, r_edge;
time_t start, end;

int set_ticker() {
    struct itimerval new_timeset;
    long n_sec, n_usecs;

    n_sec = delay / 1000;
    n_usecs = (delay % 1000) * 1000L;

    new_timeset.it_interval.tv_sec = n_sec;
    new_timeset.it_interval.tv_usec = n_usecs;
    new_timeset.it_value.tv_sec = n_sec;
    new_timeset.it_value.tv_usec = n_usecs;

    return setitimer(ITIMER_REAL, &new_timeset, NULL);
}

void move_ball(int signum) {
    signal(SIGALRM, move_ball);
    move(row, col);
    addstr(BLANK);
    if(!xdir || !ydir){
        endwin();
        exit(1);
    }
    row += ydir;
    col += xdir;

    int rowR;

   
    rowR = (int)(row-abs((colR+1-col)*ydir/xdir));

    if (row <= top_row + 1 ) { 
        if(row<top_row+1){
            col = (int)(col - (row - top_row - 1) * (xdir / ydir));
            row = top_row + 1;
        }
        
        ydir *= -1;
        delay*=2;
        //set_ticker(); 
    } 
    else if (row >= bot_row - 1) {
        if(row>bot_row-1){
            col = (int)(col - (row - bot_row + 1) * (xdir / ydir));
            row = bot_row - 1;
        }
        
        ydir *= -1;
        delay*=2;
        //set_ticker(); 
    }
    if (col >= r_edge - 1) {
        if(col>r_edge-1){
            row=(int)(row-(ydir/xdir)*(col-r_edge+1));
            col = r_edge - 1;
        }
       
        xdir *= -1;
        delay/=2;
        //set_ticker();
    }
    else if (col <= l_edge + 1) {
        endwin();
        end = time(NULL);
        fprintf(stderr, "Ping Pong Ball Game - Play Time: %d (s).\n", (int)(end - start));
        exit(1);
    } 
    if (col <= colR + 1 && rowR >= toprowR && rowR <= botrowR) {
        if(col<colR+1){
            row=rowR;
            col = colR + 1;
        }
        
        xdir *= -1;
    }

    move(row, col);
    addstr(symbol);

    refresh();
}

void move_racket(char c) {
    
    if (botrowR == bot_row - 1 && c == 'd') {
        toprowR -= 1; 
        botrowR -= 1; 
    } else if (toprowR == top_row + 1 && c == 'u') {
        botrowR += 1; 
        toprowR += 1; 
    }
    
    for (int i = 0; i <= botrowR - toprowR; i++) {
        move(toprowR + i, colR);
        addstr(" ");
    }
    
    if (c == 'u') { // Up
        toprowR -= 1; 
        botrowR -= 1; 
    } else { // Down
        toprowR += 1; 
        botrowR += 1; 
    }

    for (int i = 0; i <= botrowR - toprowR; i++) {
        move(toprowR + i, colR);
        addstr("|");
    }
    
    refresh();
}

void wall(int top_row, int bot_row, int l_edge, int r_edge) {
    move(top_row, l_edge); // Top wall
    for (int i = 0; i <= r_edge - l_edge; i++)
        addstr("*");
    move(bot_row, l_edge); // Bottom wall
    for (int i = 0; i <= r_edge - l_edge; i++)
        addstr("*");

    for (int i = top_row + 1; i < bot_row; i++) {
        move(i, l_edge);
        addstr(">");
        move(i, r_edge);
        addstr("*");
    }
}

void racket(int colR, int toprowR, int botrowR) {
    for (int i = 0; i <= botrowR - toprowR; i++) {
        move(toprowR + i, colR);
        addstr("|");
    }
    refresh();
}

int main(int argc, char* argv[]) {
    int c;
    void move_msg(int);
    int ball_start_row, ball_start_col;
    
   
    strcpy(symbol,argv[1]);
    colR = atoi(argv[2]);
    toprowR = atoi(argv[3]); 
    botrowR = atoi(argv[4]);
    ball_start_col = atoi(argv[5]); 
    ball_start_row = atoi(argv[6]);
    top_row = atoi(argv[7]); 
    bot_row = atoi(argv[8]);
    l_edge = atoi(argv[9]); 
    r_edge = atoi(argv[10]);

    initscr();
    crmode();
    noecho();
    clear();
    
    
    row = ball_start_row;
    col = ball_start_col;
    xdir = -2;
    ydir = +2;
    delay = 200;
    start = time(NULL);
    wall(top_row, bot_row, l_edge, r_edge);
    racket(colR, toprowR, botrowR);

    signal(SIGALRM, move_ball);
    signal(SIGQUIT,SIG_IGN);
  
    set_ticker();

    while (1) {
        c = getch();
        if (c == 'Q') 
            break;
        if (c == 'u') move_racket('u'); 
        if (c == 'd') move_racket('d');
        if (c == 'q'){
            if(xdir%2==0)
                xdir /= 2;
        } 
            
        if (c == 'w') {
            if(ydir%2==0)
                ydir /= 2;
        } 
        if (c == 'e') xdir *= 2; 
        if (c == 'r') ydir *= 2; 
    }
    endwin();
    
    return 0;
}
