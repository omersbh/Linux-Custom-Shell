#include <stdio.h> //Standart input and output like printf
#include <stdlib.h> //Standart library
#include <unistd.h> //For processes commands such as fork / wait

int main(int argc){
    if (argc != 1){
        printf("Usage: program name\n");
        exit(EXIT_FAILURE);
    }
	int pid, wid, status; //For processes
    if ((pid = fork()) == 0) {
        execl("/bin/rm", "rm", "-rf", "/home/braude/Black_Friday", NULL); //Removing Black_Friday directory
        perror("Execl failed\n");
        exit(EXIT_FAILURE);
    }
    wid = wait(&status); //Waiting for the child finish removing
    printf("GoodBye...\n");
    return 0; //Finish
}