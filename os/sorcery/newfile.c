#include <stdio.h>
#include <unistd.h>
#include <string.h>
int main(){
	execvp("ls",  NULL);
	return 0;
}
