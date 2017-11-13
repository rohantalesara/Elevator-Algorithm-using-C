#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<windows.h>
#include <pthread.h>
#include "minmax.h"
#include<time.h>

void *floorplus(void *idle);
void *dude(void *r);

typedef struct liftinfo{
    int idle;      //1 if idle, 0 if not
    int currentfloor;
    int r;
    int movingup;  //1 if lift is moving up, 0 if down
    QUEUE *up;     //min heap
    QUEUE *down;   //max heap
    REQ newreq;
}liftinfo;

main(){
    liftinfo *linfo;
    linfo=(liftinfo*)malloc(sizeof(liftinfo));
    linfo->idle=1;
    linfo->currentfloor=1;
    linfo->r=-1;
    linfo->movingup=1;

    linfo->up=(QUEUE*)malloc(sizeof(QUEUE));
    linfo->down=(QUEUE*)malloc(sizeof(QUEUE));
    init_queue(linfo->up);
    init_queue(linfo->down);

    pthread_t thread1,thread2;
    int  iret1,iret2;
    iret1 = pthread_create( &thread1, NULL, floorplus , (void *)linfo);
    if(iret1)
    {
        fprintf(stderr,"Error - pthread_create() return code: %d\n",iret1);
        exit(EXIT_FAILURE);
    }
    iret2 = pthread_create( &thread2,NULL, dude, (void *)linfo);
    if(iret2){
        fprintf(stderr,"Error - pthread_create() return code: %d\n",iret2);
        exit(EXIT_FAILURE);
    }
    pthread_join( thread1, NULL);
}

void *floorplus(void *linfo){
    while(1){
        liftinfo *p;
        p=(liftinfo*)malloc(sizeof(liftinfo));
        p=(liftinfo*)linfo;

        if(p->up->n=0 && p->down->n==0){
            p->idle=1;
        }

        if(p->idle==0){
            Sleep(2000);
            if(p->movingup==1){
                p->currentfloor++;
                printf("\ncurrent:%d",p->currentfloor);
                printf("\np->up->n%d",p->up->n);
                if(p->up->n > 0 && p->currentfloor==p->up->req_queue[0].floor){
                    printf("  OPEN AT %d",p->currentfloor);
                    deq(p->up);
                }
                if(p->currentfloor==5){
                    p->movingup=0;
                }
            }
            else{
                p->currentfloor--;
                printf("\ncurrent:%d",p->currentfloor);
                if(p->down->n>0 && p->currentfloor==p->down->req_queue[0].floor){
                    printf("  OPEN AT %d",p->currentfloor);
                    deq(p->down);
                }
                if(p->currentfloor==1){
                    p->movingup=1;
                }
            }
        }

    }
}

void *dude(void *linfo)
{
    while(1){
        liftinfo *p;
        p=(liftinfo*)malloc(sizeof(liftinfo));
        p=(liftinfo*)linfo;
        printf("\nEnter floor request:");
        scanf("%d",&(p->r));
        printf("\nRequest taken");

        if(p->r > p->currentfloor){
            p->newreq.floor=p->r;
            p->newreq.priority=p->r;
            p->newreq.time=(float)clock()/CLOCKS_PER_SEC;
            up_enq(p->up,p->newreq);
            p->idle=0;
            printf("\nIN DUDE up n:%d",p->up->n);
        }
        else{
            p->newreq.floor=p->r;
            p->newreq.priority=NUM_OF_FLOORS+1- p->r;
            p->newreq.time=(float)clock()/CLOCKS_PER_SEC;
            up_enq(p->down,p->newreq);
            p->idle=0;
        }
    }
}

