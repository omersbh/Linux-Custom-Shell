#include <stdio.h> //Standart input and output like printf
#include <stdlib.h> //Standart library
#include <string.h> //String library
#include <fcntl.h> //File control options - For access modes like O_RDONLY

#define BUFF_SIZE 128 //Assuming that each line contains 128 bytes only

int main (int argc, char* argv[]){
    if (argc != 2){
        printf("Usage: program name, company name\n");
        exit(EXIT_FAILURE);
    }
    char filePath[BUFF_SIZE]; //Creating filePath so we will not change argv[1]
    char *path = "/home/braude/Black_Friday/";
    char *textFormat = ".txt"; //Determining the file format
    int from, rbytes, fileSize; //Reading from a file
	char *buffer; //Saving a segment of a text
    strcpy(filePath, path); //Copying path to filePath
    strcat(filePath, argv[1]); //Creating filePath
    strcat(filePath, textFormat); //Concatenating the strings filePath and textFormat for one string, creating format "x.txt" - stored in filePath
    from = open(filePath, O_RDONLY); //Opening a file for reading
	// If file is invalid or doesn't exists
	if(from == -1) {
		perror("The file is invalid or doesn't exists");
		exit(EXIT_FAILURE);
	}
    fileSize = (int)lseek(from, 0, SEEK_END); //File size
    buffer = (char*)malloc(fileSize * sizeof(char)); //sizeof(char) = 1
    // Malloc failed
    if(buffer == NULL){
        free(buffer);
	    close(from);
        exit(EXIT_FAILURE);
    }
    lseek(from, 0, SEEK_SET); //Taking the cursor to the beggining
    if(rbytes = read(from, buffer, fileSize) > 0){
        printf("%s", buffer);
        printf("\n");
    }
    free(buffer); //Freeing allocating memmory
	close(from); //Closing the file
    return 0; //Finished 
}