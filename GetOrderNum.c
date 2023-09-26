#include <stdio.h> //Standart input and output like printf
#include <stdlib.h> //Standart library
#include <string.h> //String library
#include <dirent.h> //Directory handling

#define BUFF_SIZE 128 //Buffer

int main(int argc, char*argv[]){
    if(argc != 2){
        printf("Usage: program name, company name\n");
        exit(EXIT_FAILURE);
    }
    DIR *dirp;
    struct dirent *entry;
    char dirPath[BUFF_SIZE];
    char* path = "/home/braude/Black_Friday/";
    char *ending = "_Order";
    int numOfOrders = 0;

    strcpy(dirPath, path); //Copying path to dirPath
    strcat(dirPath, argv[1]); //Creating dirPath
    strcat(dirPath, ending);

    if ((dirp = opendir(dirPath)) == NULL) {
        printf("Company Not Found!\n");
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(dirp)) != NULL) {
        //If entry is a text file
        if (entry->d_type != DT_DIR) { 
            numOfOrders++; //Counting the text file
        }
    }
    printf("%s ---- %d Orders\n", argv[1], numOfOrders);
    closedir(dirp);
    return 0;
}