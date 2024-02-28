#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/msg.h>
#include <sys/wait.h>
#define shmSIZE 4096
#define pointSIZE 16
#define BUFFERSIZE 40
const char * msgqid = "houseparty"; 
const char * shmpath = "house";
const char * pointpath = "point";
const char * readqueuepath = "/tmp/read";
const char * writequeuepath = "/tmp/write";
const char * eofpipepath = "/tmp/eof";  
int child;
void panic()
{
    printf("\npanicking!!\n");
    kill(child, SIGKILL);
    exit(-1);
}

int main()
{
    //creating child and parent process that conversate in the same file
    mkfifo(readqueuepath, 0666);
    mkfifo(writequeuepath, 0666);
    mkfifo(eofpipepath, 0666);
    int shmfd = shm_open(shmpath, O_RDWR|O_CREAT, 0777); //initialize the shm segment
    int pointfd = shm_open(pointpath, O_RDWR|O_CREAT, 0777); //initialize the point segment
    if (shmfd<0)
    {
        perror("creation error :");
        return 0;
    }
    if (ftruncate(shmfd, shmSIZE)<0) //resize the shm
    {
        perror("ftruncate error: ");
        return 0;
    }
    if (ftruncate(pointfd, pointSIZE)<0) //resize the shm
    {
        perror("ftruncate error: ");
        return 0;
    }
    void *point = mmap(NULL, sizeof(shmpath), PROT_EXEC|PROT_WRITE|PROT_EXEC, MAP_SHARED, shmfd, 0);
    void *point_blanc = point;
    void *and_point = mmap(NULL, sizeof(pointpath), PROT_EXEC|PROT_WRITE|PROT_EXEC, MAP_SHARED, pointfd, 0);
    if (point==MAP_FAILED)
    {
        perror("mmap: ");
        return 0;
    }
    if (and_point==MAP_FAILED)
    {
        perror("mmap: ");
        return 0;
    }
    bzero(point, shmSIZE);
    int forker=fork(); //set up the dining table
    if (forker==0)
    {
        //child goes here
        int fdw = open(writequeuepath, O_RDONLY);
        int fdr = open(readqueuepath, O_WRONLY);
        int fde = open(eofpipepath, O_RDONLY);
        FILE* fptr = fopen("play.txt", "r");  
        char wbuff[10];
        read(fdw, wbuff, sizeof(wbuff)); //waiting for cue
                // exit(-1);
        point=(void*)*(long*)and_point;
        if (strcmp(wbuff, "yourTurnS")==0)
        {
        char last_literal = '\0';
            while(1)
            {
                char literal = getc(fptr);
                if (feof(fptr))
                {
                    break;
                }
                if (literal==':' && last_literal=='2')
                {
                    //this is the start of out portion
                    char innerlast_literal='\0';
                    printf("Anya: ");
                    sprintf(point, "Anya: ");
                    point+=strlen("Anya: ");
                    while (1)
                    {
                        char innerliteral = getc(fptr);
                        point+=1;
                        if (feof(fptr))
                        {
                            break;
                        }
                        if (innerliteral==':' && innerlast_literal=='1')
                        {
                            //our part has ended and we must return back to normalcy
                            char rbuff[10]="yourTurn";
                            *(long*)and_point=(long)point;
                            write(fdr, rbuff, sizeof(rbuff));
                            read(fdw, rbuff, sizeof(rbuff));
                            point=(void*)*(long*)and_point;
                            if (strcmp(rbuff, "yourTurnS")==0)
                            {
                                break;
                            }
                            else
                            {
                                if (strcmp(rbuff, "Done"))
                                break;
                            else
                            exit(0);
                            }
                        }
                        printf("%c",innerlast_literal);
                        char buffer[1];
                        sprintf(buffer, "%c", innerlast_literal);
                        sprintf(point, buffer, 1);
                        innerlast_literal=innerliteral;
                        point+=1;
                    }
                    fflush(stdout);
                    // sleep(1);
                }
                last_literal=literal;
            }
        }
        printf("\n");
        fclose(fptr);
        fflush(stdout);
        char rbuff[10]="Done";
        *(long*)and_point=(long)point;
        write(fdr, rbuff, sizeof(rbuff));
        close(fde);
        close(fdw);
        close(fdr);
        return 0;
    }
    else
    {
        //parent goes here
        child=forker;
        int fdw = open(writequeuepath, O_WRONLY);
        int fdr = open(readqueuepath, O_RDONLY);
        int fde = open(eofpipepath, O_WRONLY);
        FILE* fptr = fopen("play.txt", "r");  
        //we'll loop over every character untill it the next character's turn
        char last_literal = '\0';
        while (1)
        {
            // sprintf(point, "hi", sizeof());
            // break;
            char literal = getc(fptr);
            if (feof(fptr))
            {
                break;
            }
            if (literal==':' && last_literal=='1')
            {
                //this is the start of out portion
                char innerlast_literal='\0';
                printf("Ben: ");
                sprintf(point, "Ben: ");
                point+=strlen("Ben: ");
                // sprintf(point, "hiiii");
                // break;
                while (1)
                {
                    char innerliteral = getc(fptr);
                    point+=1;
                    if (feof(fptr))
                    {
                        break;
                    }
                    if (innerliteral==':' && innerlast_literal=='2')
                    {
                        //our part has ended and we must return back to normalcy
                        char wbuff[10]="yourTurnS";
                        // msgsnd();
                        *(long*)and_point=(long)point;
                                // exit(-1);

                        write(fdw, wbuff, sizeof(wbuff));
                        read(fdr, wbuff, sizeof(wbuff));
                        point=(void*)*(long*)and_point;
                        if (strcmp(wbuff, "yourTurn")==0)
                        {
                            break;
                        }
                        else
                        {
                            if (strcmp(wbuff, "Done")==0)
                            {
                                break;
                            }
                            else 
                            panic();
                        }
                    }
                    printf("%c",innerlast_literal);
                    char buffer[1];
                    sprintf(buffer, "%c", innerlast_literal);
                    sprintf(point, buffer);
                    point+=1;
                    innerlast_literal=innerliteral;
                }
                fflush(stdout);
                // sleep(1);
            }
            last_literal=literal;
        }
        fclose(fptr);
        sprintf(point, "\n");// temporary sol
        char wbuff[10]="Done";\

        *(long*)and_point=(long)point;                                                                                                                                                                              

        // printf("pardone\n");
        fflush(stdout);
        if (munmap(point_blanc, shmSIZE)<0)
        {
            perror("munmap: ");
            return 0;
        }
        // if (shm_unlink(shmpath)<0)
        // {
        //     perror("unlink: ");
        //     return 0;
        // }
        // printf("here\n");
        write(fdw, wbuff, sizeof(wbuff));
        wait(NULL);
        close(fde);
        close(fdw);
        close(fdr);
    }
    return 0;
}

//writing to pipe ex: 
        // printf("p:I am the parent\n");
        // int times=3;
        // while (times-->0)
        // {
        //     printf("p:killing child in %d.\n", times);
        //     char rbuff[10]="wait";
        //     write(fde, rbuff, sizeof(rbuff));
        //     sleep(1);
        // }
        // printf("p:die mothafacka.\n");
        // char rbuff[10]="diescomos";
        // write(fde, rbuff, sizeof(rbuff));