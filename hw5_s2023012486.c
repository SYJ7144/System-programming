#include<stdio.h>
#include<unistd.h>
#include<signal.h>
#include<time.h>



int leftTime;

void signal_handler( int signo )
{
   printf("Left Time:");
}
void signal_handler2(int signo){
    printf("Entering orbit...");
}
void sigInt_handler(int signo){
    alarm(0);
    printf("\nEarly Seperation! (SIGINT)\n");
    leftTime=0;
    
}
void sigQuit_handler(int signo){
    alarm(0);
    printf("\nEarly Seperation! (SIGQUIT)\n");
    leftTime=0;
}

int main(){
    
    signal(SIGALRM, signal_handler);
    signal(SIGINT,SIG_IGN);
    signal(SIGQUIT,SIG_IGN);
    int T;
    
    
    printf("Firing latency (s): ");
    scanf("%d",&T);
    leftTime=T;
    
    while(leftTime>0)
    {
        alarm(1);
        pause();
        alarm(1);
        if(leftTime>0){
            printf("   %d s\n",leftTime);
        }
        
        leftTime--;
    }
    printf("###############################\n");
    printf("             Fire!\n");
    printf("###############################\n");
    
    signal(SIGINT,sigInt_handler);
    leftTime=T;
    while(leftTime>0)
    {
        alarm(1);
        pause();
        alarm(1);
        if(leftTime>0){
            printf("   %d s\n",leftTime);
        }
        leftTime--;
    }
    printf("###############################\n");
    printf("Satellite 1st engine separation\n");
    printf("###############################\n");
    
    signal(SIGQUIT,sigQuit_handler);
    leftTime=T;
    while(leftTime>0)
    {
        alarm(1);
        pause();
        alarm(1);
        if(leftTime>0){
            printf("   %d s\n",leftTime);
        }
        leftTime--;
    }
    printf("###############################\n");
    printf("Satellite 2nd engine separation\n");
    printf("###############################\n");

    signal(SIGINT,SIG_IGN);
    signal(SIGQUIT,SIG_IGN);
    signal(SIGALRM, signal_handler2);
    leftTime=3;
    while(leftTime>0)
    {
        alarm(1);
        pause();
        alarm(1);
        if(leftTime>0){
            printf("   %d \n",leftTime);
        }
        leftTime--;
    }
    printf("###############################\n");
    printf("   Satellite orbital entry\n");
    printf("###############################\n");
    alarm(0);
    int speed;
    int distance;
    int ExpectedTime;
    printf("Speed of satellites (km/s): ");
    scanf("%d",&speed);
    printf("Distance to travel (km): ");
    scanf("%d",&distance);
    printf("\n\n");
    ExpectedTime=(int)(distance/speed);
    if((distance/speed)!=0)
        printf("Expected Time (s): %d\n",ExpectedTime);
    else{
        fprintf(stderr,"DIVID BY ZERO ERROR!\n");
        printf("The satellite maintains its current position.\n");
    }
        
    


}
