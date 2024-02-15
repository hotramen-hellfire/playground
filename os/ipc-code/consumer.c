/**
 * Simple program demonstrating shared memory in POSIX systems.
 *
 * This is the consumer process
 *
 * Figure 3.18
 *
 * @author Gagne, Galvin, Silberschatz
 * Operating System Concepts - Ninth Edition
 * Copyright John Wiley & Sons - 2013
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#define BYTES 48

char *e_sync_path = "/tmp/e_sync_pipe";
char *written_path = "/tmp/written_pipe";
char *freed_path = "/temp/freed_pipe";

int main()
{
    const char *name = "gangsta";
    int e_sync_pipe = open(e_sync_path, O_WRONLY);
    int written_pipe = open(written_path, O_RDONLY);
    int freed_pipe = open(freed_path, O_WRONLY);
    const char *frii = "friiiii";
    const int SIZE = BYTES;
    int shm_fd;
    void *ptr;
    void *ptr2;
    int i;

    /* open the shared memory segment */
    shm_fd = shm_open(name, O_RDWR, 0666);
    if (shm_fd == -1)
    {
        printf("shared memory failed\n");
        exit(-1);
    }

    /* now map the shared memory segment in the address space of the process */
    ptr = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    if (ptr == MAP_FAILED)
    {
        printf("Map failed\n");
        exit(-1);
    }

    /* now read from the shared memory region */
    // printf("%d", strlen((char *)ptr));
    char messagerecv[8];
    char index[10] = "-1";
    int idx = -1;
    char buf[10];

    // assume all were free before
    // for (int iters = 0; iters < BYTES / (sizeof(frii)); iters++)
    // {
    //     // sscanf();

    // }

    strcpy(index, "-1");
    while (read(written_pipe, index, sizeof(index)) > 0)
    {
        sscanf(index, "%d", &idx);
        if (idx == -1)
        {
            printf("Reading Done!!\n");
            break;
        }
        printf("%d: ", idx);
        printf((char *)(ptr + 8 * idx));
        printf(" at location %lu\n", ptr + 8 * idx);
        // free this segment
        sprintf(ptr + 8 * idx, "%s\0", frii);
        // inform the producer
        sprintf(buf, "%d", idx);
        write(e_sync_pipe, buf, 10);
    }

    /* remove the shared memory segment */
    if (shm_unlink(name) == -1)
    {
        printf("Error removing %s\n", name);
        exit(-1);
    }
    sprintf(buf, "%d", -2);
    write(e_sync_pipe, buf, 10);
    printf("Job Done!!\n");
    close(freed_pipe);
    close(e_sync_pipe);
    close(written_pipe);
    return 0;
}
