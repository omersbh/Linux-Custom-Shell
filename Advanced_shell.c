#include <stdio.h> //Standart input and output like printf
#include <stdlib.h> //Standart library
#include <string.h> //String library
#include <fcntl.h> //File control options - For access modes like O_RDONLY
#include <unistd.h> // For fork, exec and wait commands

#define BUFF_SIZE 128 //Buffer

void createDirectory(char* filePath); //Function that creates a directory
void createText(char* filePath); //Function that creates a text file ("x.txt")

//Function that creates a directory
void createDirectory(char* filePath){
    mkdir(filePath, 0777);
}

//Function that creates a text file ("x.txt")
void createText(char* filePath){
    int to = open(filePath, O_WRONLY | O_CREAT, 0644); //Opening a new file
    //If the file creation failed
    if(to == -1){
        perror("Could not create the file");
        exit(EXIT_FAILURE);
    }
    //Finish, closing our new file
    close(to);
}

int main(int argc){
    if (argc != 1){
        printf("Usage: program name\n");
        exit(EXIT_FAILURE);
    }
    char *argv[5] = {NULL}; //Initializing argv array to NULL ---> Program name, path, 3 parameters
    char input[BUFF_SIZE]; //Info from the user
    char *split; //Splitting string into tokens using strtok
    int i = 0, pid, wid, status; //For processes

    createDirectory("/home/braude/Black_Friday"); //New directory Black_Friday
    createText("/home/braude/Black_Friday/camp_partic.txt"); //New text file camp_partic.txt

    printf("AdvShell> "); //Our advanced shell

    while(fgets(input, sizeof(input), stdin) != NULL && strcmp(input, "exit\n")){
        input[strcspn(input, "\n")] = 0; //Removing the \n from input created by fgets
        split = strtok(input, " ");
        while(split != NULL && i < 5){
            argv[i] = (char*)malloc((strlen(split) + 1) * sizeof(char)); //Allocating memmory
            if(argv[i] == NULL){
                printf("Memory allocation has failed.\n");
                exit(EXIT_FAILURE);
            }
            strcpy(argv[i], split); //Saving the data
            i++; //Next index
            split = strtok(NULL, " "); //Keep splitting
        }
        // Execute the GetFlyer command
        if ((pid = fork()) == 0) {
            if(!strcmp(argv[0], "CreateFlyer")){
                execvp("./CreateFlyer", argv);
                perror("CreateFlyer failed.");
                exit(EXIT_FAILURE);
            }
            if(!strcmp(argv[0], "GetFlyer")){
                execvp("./GetFlyer", argv);
                perror("GetFlyer failed.");
                exit(EXIT_FAILURE);
            }
            if(!strcmp(argv[0], "MakeOrder")){
                execvp("./MakeOrder", argv);
                perror("MakeOrder failed.");
                exit(EXIT_FAILURE);
            }
            if(!strcmp(argv[0], "GetNumComp")){
                execvp("./GetNumComp", argv);
                perror("GetNumComp failed.");
                exit(EXIT_FAILURE);
            }
            if(!strcmp(argv[0], "GetOrderNum")){
                execvp("./GetOrderNum", argv);
                perror("GetOrderNum failed.");
                exit(EXIT_FAILURE);
            }
            if(!strcmp(argv[0], "GetSummary")){
                execvp("./GetSummary", argv);
                perror("GetSummary failed.");
                exit(EXIT_FAILURE);
            }
            execvp(argv[0], argv); //Other original shell options
            printf("Not Supported\n");
            exit(EXIT_FAILURE);
        }
        wid = wait(&status); //Waiting for child finishing process
        //Freeing allocated memmory
        for(i = 0; i < 5; i++){
            free(argv[i]);
            argv[i] = NULL;
        }
        i = 0;
        printf("AdvShell> ");
    }
    if((pid = fork()) == 0){
        execl("exit", "exit", NULL);
        perror("exit failed.");
        exit(EXIT_FAILURE);
    }
    wid = wait(&status); //Waiting for child finishing process
    return 0; //Finish main process
}