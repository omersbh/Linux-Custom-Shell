#include <stdio.h> //Standart input and output like printf
#include <stdlib.h> //Standart library
#include <string.h> //String library
#include <fcntl.h> //File control options - For access modes like O_RDONLY
#include <unistd.h> // For fork, exec and wait commands

#define BUFF_SIZE 128 //Buffer

void printSummary(char* filePath, char* argument); //Funtion that prints summary using other programs

//Funtion that prints summary using other programs
void printSummary(char* program, char* argument){
    // Execute the program with its argument
    if (fork() == 0) {
        execl(program, program, argument, NULL);
        perror("Execl failed\n");
        exit(EXIT_FAILURE);
    }
    wait(NULL); //Waiting for the program to finish
}

int main(int argc){
	if (argc != 1){
        printf("Usage: program name\n");
        exit(EXIT_FAILURE);
    }
    char filePath[BUFF_SIZE] = "/home/braude/Black_Friday/camp_partic.txt";
    int from, rbytes, fileSize; //Reading from a file
	char *buffer; //Saving a segment of a text in the size of BUFF_SIZE bytes
    char *split; //Split the buffer
    from = open(filePath, O_RDONLY); //Opening a file for reading
	// If file is invalid or doesn't exists
	if(from == -1) {
		perror("The file is invalid or doesn't exists");
		exit(EXIT_FAILURE);
	}
    fileSize = (int)lseek(from, 0, SEEK_END); //File size
    buffer = (char*)malloc(fileSize * sizeof(char)); //sizeof(char) = 1
    //If malloc failed
    if(buffer == NULL){
        free(buffer);
	    close(from);
        exit(EXIT_FAILURE);
    }
    lseek(from, 0, SEEK_SET); //Taking the cursor to the beggining
    printSummary("GetNumComp", NULL); //"Calling" GetNumComp program
    if(rbytes = read(from, buffer, fileSize) > 0){
        split = strtok(buffer, "\n"); //Splitting, each line is a company
        while(split != NULL){
            printSummary("GetOrderNum", split); //"Calling" GetOrderNum program
            split = strtok(NULL, "\n"); //Reading next line
        }
    }
    free(buffer); //Freeing allocating memmory
	close(from); //Closing the file
    return 0; //Finish
}