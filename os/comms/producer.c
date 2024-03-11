/**
 * Simple program demonstrating shared memory in POSIX systems.
 *
 * This is the producer process that writes to the shared memory region.
 *
 * Figure 3.17
 *
 * @author Silberschatz, Galvin, and Gagne
 * Operating System Concepts  - Ninth Edition
 * Copyright John Wiley & Sons - 2013
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#define BYTES 24
#define TIMES 15
#define WRITING_DELAY 1
#define RESIDUAL_WRITING_DELAY 3
int init(void *ptr, const int SIZE, const char *frii)
{
    int iter = 0;
    void *temp = ptr;
    while (iter < (SIZE / sizeof(frii)))
    {
        iter++;
        int c = sprintf(ptr, "%s\0", frii);
        ptr += strlen(frii) + 1;
    }
    ptr = temp;
    // printf("vibe check :%d", iter);
    return 0;
}

int typewriter(void *ptr, const int SIZE, const char *word, int written_pipe)
{
    // sleep(10);

    // printf("vibe check :%d", iter);
    return 0;
}

char *e_sync_path = "/tmp/e_sync_pipe";
char *written_path = "/tmp/written_pipe";
char *freed_path = "/temp/freed_pipe";

int main()
{
    mkfifo(e_sync_path, 0666);
    mkfifo(written_path, 0666);
    mkfifo(freed_path, 0666);
    const int SIZE = BYTES;
    const char *name = "gangsta";
    const char *frii = "friiiii";
    const char *writes1 = "NARCOZ";
    const char *writes2 = "ballasz";
    int e_sync_pipe = open(e_sync_path, O_RDONLY);
    int written_pipe = open(written_path, O_WRONLY);
    int freed_pipe = open(freed_path, O_RDONLY);
    int shm_fd;
    void *ptr;

    /* create the shared memory segment*/
    shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);

    /* configure the size of the shared memory segment*/
    ftruncate(shm_fd, SIZE);

    /* now map the shared memory segment in the address space of the process*/
    ptr = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED)
    {
        printf("Map failed\n");
        return -1;
    }
    init(ptr, SIZE, frii);
    printf("shared memory now availible to use!!\n");

    int iter = 0;
    char buf[10];
    char index[10] = "-1";
    int idx = -1;
    int c;
    // sizeof = strlen +1;
    while (iter < BYTES / (sizeof(writes1)))
    {
        sleep(WRITING_DELAY);
        if (iter % 2 == 0)
            c = sprintf(ptr + (8 * (iter % (SIZE / sizeof(writes1)))), "%s\0", writes1);
        else
        {
            c = sprintf(ptr + (8 * (iter % (SIZE / sizeof(writes1)))), "%s\0", writes2);
        }
        printf("%d\n", iter);
        sprintf(buf, "%d", (iter % (SIZE / sizeof(writes1))));
        write(written_pipe, buf, 10);
        iter++;
    }

    while (iter < TIMES)
    {
        int readstatus = read(e_sync_pipe, buf, 10);
        // printf("%s, %d\n", buf, readstatus);
        sscanf(buf, "%d", &idx);
        if (iter % 2 == 0)
            c = sprintf(ptr + (8 * (idx)), "%s\0", writes1);
        else
        {
            c = sprintf(ptr + (8 * (idx)), "%s\0", writes2);
        }
        sleep(RESIDUAL_WRITING_DELAY);
        printf("%d\n", iter);
        sprintf(buf, "%d", (iter % (SIZE / sizeof(writes1))));
        write(written_pipe, buf, 10);
        iter++;
    }

    printf("Writing Done!!\n");
    // send writing end signal
    sprintf(buf, "%d", -1);
    write(written_pipe, buf, 10);

    while (read(e_sync_pipe, buf, 10))
    {
        if (strcmp(buf, "-2") == 0)
        {
            printf("Job Done!!\n");
        }
    }
    close(freed_pipe);
    close(e_sync_pipe);
    close(written_pipe);
    return 0;
}
