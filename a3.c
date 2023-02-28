#include <stdio.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define NUME "RESP_PIPE_41792" // pipe
#define NUME2 "REQ_PIPE_41792"

int main(void)
{
    int fd = -1, fd2 = -1;
    int nr=7;
    if (mkfifo(NUME, 0600) != 0)
    {
        printf("ERROR");
        printf("\ncannot create the response pipe | cannot open the request pipe"); 
        return 1; // crearea pipe 
    }
    fd = open(NUME2, O_RDONLY);
    if (fd == -1)
    {
        printf("ERROR");
        printf("\ncannot create the response pipe | cannot open the request pipe"); 
        return 1;
    } // deschidere pipe ptr citire
    fd2 = open(NUME, O_WRONLY);
    if (fd2 == -1)
    {
        printf("ERROR");
        printf("\ncannot create the response pipe | cannot open the request pipe"); 
        return 1;
    }// deschidere pipe ptr scriere
    if(write(fd2, &nr, 1)!=1|| write(fd2,"CONNECT",7)!=7)
    {
        printf("ERROR");
        printf("\ncannot create the response pipe | cannot open the request pipe"); 
        return 1; // connectare la tester
    }
    else 
    {
        printf("SUCCESS\n"); //success conectare
    }
    while(1)
    {
        int nr2=0;
        read(fd,&nr2,1);
        char buffer[nr2+1];// citire comanda
        read(fd,buffer,sizeof(char)*nr2);
        buffer[nr2]='\0';
        if(strcmp(buffer,"PING")==0) // comanda = ping
        {
            char lungime=4;
            int nr3=41792;
            write(fd2,&lungime,1);
            write(fd2,buffer,4);//scriere ping
            write(fd2,&lungime,1);
            strcpy(buffer,"PONG");//scriere pong
            write(fd2,buffer,4);
            write(fd2, &nr3,sizeof(int));// scriere 41792
            break;
        }
        else if(strcmp(buffer,"CREATE_SHM")==0)  //comanda = create_SHM
        {
            int fd3= shm_open("/a5ym8gJ", O_CREAT | O_RDWR, 0664); // crearae zona de memorie partajata
            unsigned int dim;
            read(fd,&dim,sizeof(unsigned int));
            ftruncate(fd3, dim); // dimensiune memroie
            char * buffer;
            buffer=(char*)mmap(NULL,dim,PROT_READ |PROT_WRITE, MAP_SHARED,fd3,0); // mapare memorie
            if(fd3>0 && buffer!=(void*)-1)// verificare reusita operatie
            {
                char s[12]="CREATE_SHM"; // transmitere success
                int nr4=10;
                write(fd2, &nr4,1);
                write(fd2,s,10);
                nr4=7;
                strcpy(s,"SUCCESS");
                write(fd2,&nr4,1);
                write(fd2,s,7);
            }
            else
            {
                char s[12]="CREATE_SHM";
                int nr4=10; // transmitere fail ;
                write(fd2, &nr4,1);
                write(fd2,s,10);
                nr4=5;
                strcpy(s,"ERROR");
                write(fd2,&nr4,1);
                write(fd2,s,5);
            }

        }
        else // EXIT;
        {
            close(fd);
            close(fd2);
            unlink(NUME);
            break;
        }
    }
   return 0;
}