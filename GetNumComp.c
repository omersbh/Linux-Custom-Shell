#include <stdio.h> //Standart input and output like printf
#include <stdlib.h> //Standart library
#include <string.h> //String library
#include <fcntl.h> //File control options - For access modes like O_RDONLY

#define BUFF_SIZE 128 //Assuming that each line contains 128 bytes only

int main (int argc){
    if (argc != 1){
        printf("Usage: program name\n");
        exit(EXIT_FAILURE);
    }
    char filePath[BUFF_SIZE] = "/home/braude/Black_Friday/camp_partic.txt";
    int from, rbytes, fileSize, sumOfComp = 0; //Reading from a file
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
    if(rbytes = read(from, buffer, fileSize) > 0){
        split = strtok(buffer, "\n"); //Splitting, each line is a company
        while(split != NULL){
            sumOfComp++;
            split = strtok(NULL, "\n"); //Reading next line
        }
    }
    printf("%d companies take part in the Campaign\n", sumOfComp);
    free(buffer); //Freeing allocating memmory
	close(from); //Closing the file
    return 0; //Finished 
}