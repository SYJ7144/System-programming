#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>

struct flaginfo{
    int fl_value;
    char *fl_name;
};

struct flaginfo input_flags[];
struct flaginfo output_flags[];
struct flaginfo local_flags[];


void flip_A(struct termios *ttyp);
void flip_flags(struct termios *ttyp, struct flaginfo thebitnames[],char str[]);
void showbaud(int thespeed);
void show_some_flags(struct termios *ttyp);
void show_flagset(int thevalue,struct flaginfo thebitnames[]);

int main(int argc, char *argv[]){
    struct termios ttyinfo;
    if(tcgetattr(0,&ttyinfo)==-1){
        perror("cannot get params about stdin");
        exit(1);
    }
    
    showbaud(cfgetospeed(&ttyinfo));
    printf("erase = ^%c;kill = ^%c;\n",ttyinfo.c_cc[VERASE]-1+'A',ttyinfo.c_cc[VKILL]-1+'A');

    if (argc > 1) {
        
        
        if (strcmp(argv[1], "-FI") == 0) {
            for(int i=2;i<argc;i++)
                flip_flags(&ttyinfo, input_flags,argv[i]);
        } else if (strcmp(argv[1], "-FO") == 0) {
            flip_flags(&ttyinfo, output_flags,"olcuc");
        } else if (strcmp(argv[1], "-FL") == 0) {
            for(int i=2;i<argc;i++)
                flip_flags(&ttyinfo, local_flags,argv[i]);
        } else if (strcmp(argv[1], "-A") == 0) {
            flip_A(&ttyinfo);
        } 
    }
    
    
    show_some_flags(&ttyinfo);
}



void showbaud(int thespeed){
    printf("speed ");
    switch(thespeed){
        case B300:      printf("300 baud;\n"); break;
        case B600:      printf("600 baud;\n"); break;
        case B1200:     printf("1200 baud;\n"); break;
        case B1800:     printf("1800 baud;\n"); break;
        case B2400:     printf("2400 baud;\n"); break;
        case B4800:     printf("4800 baud;\n"); break;
        case B9600:     printf("9600 baud;\n"); break;
        default   :     printf("Fast baud;\n"); break;
    }
}
    
 
struct flaginfo input_flags[]={
    IGNBRK , "ignbrk",
    BRKINT , "brkint",
    IGNPAR , "ignpar",
    PARMRK , "parmrk",
    INPCK  , "inpck",
    ISTRIP , "istrip",
    INLCR  , "inlcr",
    IGNCR  , "igncr",
    ICRNL  , "icrnl",
    IXON   , "ixon",
    IXOFF  , "ixoff",
    0      , NULL};

struct flaginfo output_flags[] = {
      OLCUC, "olcuc",
      0, NULL};

struct flaginfo local_flags[]={
    ISIG   , "isig",
    ICANON , "icanon",
    ECHO   , "echo",
    ECHOE  , "echoe",
    ECHOK  , "echok",
    0      , NULL};

void flip_A(struct termios *ttyp){
    for(int i=0; input_flags[i].fl_name;i++)
    {
        if(ttyp->c_iflag & input_flags[i].fl_value){
                ttyp->c_iflag &= ~input_flags[i].fl_value;
        }else{
                ttyp->c_iflag|=input_flags[i].fl_value;
        }
    }
    for(int i=0; output_flags[i].fl_name;i++)
    {
        if(ttyp->c_oflag & output_flags[i].fl_value){
                ttyp->c_oflag &= ~output_flags[i].fl_value;
        }else{
                ttyp->c_oflag|=output_flags[i].fl_value;
        }
    }
    for(int i=0; local_flags[i].fl_name;i++)
    {
        if(ttyp->c_lflag & local_flags[i].fl_value){
                ttyp->c_lflag &= ~local_flags[i].fl_value;
        }else{
                ttyp->c_lflag|=local_flags[i].fl_value;
        }
    }
    if(tcsetattr(0,TCSANOW,ttyp)==-1){
                perror("tcsetattr");
                exit(2);
            }
    
}void flip_flags(struct termios *ttyp, struct flaginfo thebitnames[], char str[]) {
    int i;
    if (thebitnames == input_flags) {
        for (i = 0; input_flags[i].fl_name; i++) {
            if(strcmp(input_flags[i].fl_name,str)==0){
                if(ttyp->c_iflag & input_flags[i].fl_value){
                    ttyp->c_iflag &= ~input_flags[i].fl_value;
                } else {
                    ttyp->c_iflag |= input_flags[i].fl_value;
                }
                break;
            }
        }
    } else if (thebitnames == output_flags) {
        for (i = 0; output_flags[i].fl_name; i++) {
            if(strcmp(output_flags[i].fl_name,str)==0){
                if(ttyp->c_oflag & output_flags[i].fl_value){
                    ttyp->c_oflag &= ~output_flags[i].fl_value;
                } else {
                    ttyp->c_oflag |= output_flags[i].fl_value;
                }
                break;
            }
        }
    } else if (thebitnames == local_flags) {
        for (i = 0; local_flags[i].fl_name; i++) {
            if(strcmp(local_flags[i].fl_name,str)==0){
                if(ttyp->c_lflag & local_flags[i].fl_value){
                    ttyp->c_lflag &= ~local_flags[i].fl_value;
                } else {
                    ttyp->c_lflag |= local_flags[i].fl_value;
                }
                break;
            }
        }
    }
    if(tcsetattr(0,TCSANOW,ttyp)==-1){
                perror("tcsetattr");
                exit(2);
            }
    
}

void show_some_flags(struct termios *ttyp){
    show_flagset(ttyp->c_iflag,input_flags);
    show_flagset(ttyp->c_oflag,output_flags);
    show_flagset(ttyp->c_lflag,local_flags);
}

void show_flagset(int thevalue, struct flaginfo thebitnames[]){
    int i;
    for(i=0; thebitnames[i].fl_value;i++){
        if(thevalue & thebitnames[i].fl_value){
            printf("%s ",thebitnames[i].fl_name);
        }else{
            printf("-%s ",thebitnames[i].fl_name);
        }
    }
    printf("\n");
}



