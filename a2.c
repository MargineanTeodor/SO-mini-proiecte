#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "a2_helper.h"
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>
typedef struct{
    int nume;
}TH3;
sem_t* sem25=NULL, *sem32=NULL,*sem31=NULL, * sem35= NULL, *sem4 = NULL;
void* cerinta3(void* param)
{
    TH3* nume= (TH3 *)param;
    if(nume->nume==5)
    {
        sem_wait(sem35);
        info(BEGIN,3,nume->nume);
        info(END,3,nume->nume);
        sem_post(sem31);
    }
    else
    {
        if(nume->nume==1)
        {
            info(BEGIN,3,nume->nume);
            sem_post(sem35);
            sem_wait(sem31);
            info(END,3,nume->nume);
        }
        else if(nume->nume==2)
        {
            sem_wait(sem32);
            info(BEGIN,3,nume->nume);
            info(END,3,nume->nume);
            sem_post(sem25);
        }
            else
            {
                info(BEGIN,3,nume->nume);
                info(END,3,nume->nume);
            }
    }    
    return param;
}
void* cerinta4(void* param)
{
    TH3* nume= (TH3 *)param;
    sem_wait(sem4);
    info(BEGIN,5,nume->nume);
    info(END,5,nume->nume);
    sem_post(sem4);
    return param;
}
void* cerinta5(void* param)
{
    TH3* nume= (TH3 *)param;
    if(nume->nume == 1)
    {
        info(BEGIN,2,nume->nume);
        info(END,2,nume->nume);  
        sem_post(sem32);
    }
    else 
        if(nume->nume==5)
        {
            sem_wait(sem25);
            info(BEGIN,2,nume->nume);
            info(END,2,nume->nume); 
        }
        else 
        {
            info(BEGIN,2,nume->nume);
            info(END,2,nume->nume); 
        }
    
    return param;
}
int main(){
    init();

    info(BEGIN, 1, 0);
    pid_t pid2=-1,pid3=-1,pid4=-1,pid5=-1,pid6=-1,pid7=-1;
    pthread_t tid3[5];
    pthread_t tid2[5];
    pthread_t tid[40];
    TH3 x[5];
    TH3 y[5];
    TH3 z[40];
    sem25= sem_open("sem25",O_CREAT,0644,0); 
    sem4= sem_open("sem4",O_CREAT,0644,6); 
    sem32= sem_open("sem32",O_CREAT,0644,0); 
    sem31= sem_open("sem31",O_CREAT,0644,0); 
    sem35= sem_open("sem35",O_CREAT,0644,0); 
    int i;
    int status2,status3,status4,status5,status6,status7;
    pid2=fork();
    if(pid2==0)
    {
        info(BEGIN,2,0);
        pid4=fork();
        if(pid4==0)
        {
            info(BEGIN,4,0);
            pid5=fork();
            if(pid5==0)
            {
                info(BEGIN,5,0);
                for(i=1;i<=39;i++)
                {
                    z[i].nume=i; 
                    pthread_create(&tid[i],NULL,cerinta4,&z[i]);
                }
                for(i=1;i<=39;i++)
                {
                    pthread_join(tid[i],NULL);
                }
                info(END,5,0);
                exit(5);
            }
            wait(&status5);
            info(END,4,0);
            exit(4);
        }
        wait(&status4);
        pid6=fork();
        if(pid6==0)
        {
            info(BEGIN,6,0);
            info(END,6,0);
            exit(6);
        }
        wait(&status6);
        pid7=fork();
        if(pid7==0)
        {
            info(BEGIN,7,0);
            info(END,7,0);
            exit(7);
        }
        wait(&status7);
        pid3=fork();
        if(pid3==0)
        {
            info(BEGIN,3,0);   
            
            for(i=1;i<=5;i++)
            {
                x[i].nume=i; 
                pthread_create(&tid3[i],NULL,cerinta3,&x[i]);
            }
            for(i=1;i<=5;i++)
            {
                pthread_join(tid3[i],NULL);
            }
            info(END,3,0); 
            exit(3);
        }
       
        for(i=1;i<=5;i++)
        {
            y[i].nume=i; 
            pthread_create(&tid2[i],NULL,cerinta5,&y[i]);
        }
        for(i=1;i<=5;i++)
        {
            pthread_join(tid2[i],NULL);
        } 
        wait(&status3);
        info(END,2,0);
        exit(2);
    }
    else
    {
        wait(&status2);
        info(END, 1, 0);
    }
    sem_unlink("sem25");
        sem_unlink("sem32");
        sem_unlink("sem31");
        sem_unlink("sem35");
    sem_unlink("sem4");
    return 0;
}
