#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#define NUM_THREADS 8
#define RL_SPACING_RANGE 40
#define DELAY 1
pthread_mutex_t mootex =PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condn =PTHREAD_COND_INITIALIZER;
struct sequenceArg{
    int i;
    int* order;
};

int spac1=0;//initialize random spacing

void _printer(int n)
{
    n=2*n-1;
    int num_threads1=2*NUM_THREADS+1;
    int spac=(num_threads1-n)/2;
    if ((num_threads1-n)%2) spac=(num_threads1-n+1)/2;
    if (n==1)
    {
        spac1=rand()%RL_SPACING_RANGE;
        for (int i=0; i<spac1; i++) printf(" ");
        for (int i=0; i<spac; i++) printf(" ");
        printf(" *");
        printf(" ");
        for (int i=0; i<spac; i++) printf(" ");
        for (int i=0; i<spac1; i++) printf(" ");
        printf("\n") ;
    }

    for (int i=0; i<spac1; i++) printf(" ");
    
    for (int i=0; i<spac; i++) printf(" ");
    printf("/");
    for (int i=0; i<n; i++)
    {   
        int rand_ascii = rand()%94 + 33;
        printf("%c", rand_ascii);
    }
    printf("\\");
    for (int i=0; i<spac; i++) printf(" ");
    
    for (int i=0; i<spac1; i++) printf(" ");

    printf("> %d\n", (n+1)/2);
    if ((n+1)/2 == NUM_THREADS)
    {
        n=n+2;
        num_threads1=2*NUM_THREADS+1;
        spac=(num_threads1-n)/2;
        if ((num_threads1-n)%2) spac=(num_threads1-n+1)/2;
        for (int i=0; i<spac1; i++) printf(" ");
        
        for (int i=0; i<spac; i++) printf(" ");
        printf("/");
        for (int i=0; i<n; i++)
        {   
            if (i%2) printf("|");
            else printf("_");
        }
        printf("\\");
        for (int i=0; i<spac; i++) printf(" ");
        
        for (int i=0; i<spac1; i++) printf(" ");
        printf("\n\n");
    }
    return;
}

void *sequence(void *dummy)
{
    struct sequenceArg* arg=(struct sequenceArg*)dummy;
    while (1)
    {
        pthread_mutex_lock(&mootex);
        if (arg->order[arg->i-1]==0)
        {
            pthread_cond_wait(&condn, &mootex);
        }
        else
        {
            _printer(arg->i);
            arg->order[arg->i-1]=0;
            arg->order[(arg->i)%NUM_THREADS]=1;
            if (arg->i==NUM_THREADS) sleep(DELAY);
            pthread_cond_broadcast(&condn);
        }
        pthread_mutex_unlock(&mootex);
    }
    pthread_exit(NULL);
    return 0x0;
}

void intro() {
    printf("ADJUSTING THE CURTAINS. . .\n");
    sleep(2);
    printf(".        .          .    .    .            .            .                   .\n");
    printf("               .               ..       .       .   .             .\n");
    printf(" .      .     T h i s   i s   t h e   g a l a x y   o f   . . .             .\n");
    printf("                     .              .       .                    .      .\n");
    printf(".        .               .       .     .            .\n");
    printf("   .           .        .                     .        .            .\n");
    printf("             .               .    .          .              .   .         .\n");
    printf("               _________________      ____         __________\n");
    printf(" .       .    /                 |    /    \\    .  |          \\\n");
    printf("     .       /    ______   _____| . /      \\      |    ___    |     .     .\n");
    printf("             \\    \\    |   |       /   /\\   \\     |   |___>   |\n");
    printf("           .  \\    \\   |   |      /   /__\\   \\  . |         _/               .\n");
    printf(" .     ________>    |  |   | .   /            \\   |   |\\    \\_______    .\n");
    printf("      |            /   |   |    /    ______    \\  |   | \\           |\n");
    printf("      |___________/    |___|   /____/      \\____\\ |___|  \\__________|    .\n");
    printf("  .     ____    __  . _____   ____      .  __________   .  _________\n");
    printf("       \\    \\  /  \\  /    /  /    \\       |          \\    /         |      .\n");
    printf("        \\    \\/    \\/    /  /      \\      |    ___    |  /    ______|  .\n");
    printf("         \\              /  /   /\\   \\ .   |   |___>   |  \\    \\\n");
    printf("   .      \\            /  /   /__\\   \\    |         _/.   \\    \\            +\n");
    printf("           \\    /\\    /  /            \\   |   |\\    \\______>    |   .\n");
    printf("            \\  /  \\  /  /    ______    \\  |   | \\              /          .\n");
    printf(" .       .   \\/    \\/  /____/      \\____\\ |___|  \\____________/  LS\n");
    printf("                               .                                        .\n");
    printf("     .                           .         .               .                 .\n");
    printf("                .                                   .            .\n");
    printf("WE WILL BE DEPLOYING THE SHIPS SHORTLY!!\n");
    sleep(4);
    for (int i=0; i<2*(2*NUM_THREADS+1)+2*RL_SPACING_RANGE; i++)
    {
        int disp=rand()%2;
        if (disp) printf("*");
        else printf(" ");
    }
    printf("\n\n");
    return;
}

int main()
{
    srand((int)time(NULL));
    intro();
    pthread_t threads[NUM_THREADS];
    int order[NUM_THREADS];
    for (int i=0; i<NUM_THREADS; i++) order[i]=0;
    order[0]=1;
    for (int i=0; i<NUM_THREADS; i++)
    {
        struct sequenceArg* arg = (struct sequenceArg*)malloc(sizeof(struct sequenceArg));
        arg->i=i+1;
        arg->order=order;
        pthread_create(&threads[i], NULL, &sequence, (void*)arg);
    }
    for (int i=0; i<NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }
    return 0b0;
}
