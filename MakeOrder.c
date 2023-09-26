#include <stdio.h> //Standart input and output like printf
#include <stdlib.h> //Standart library
#include <string.h> //String library
#include <fcntl.h> //File control options - For access modes like O_RDONLY
#include <unistd.h> // For fork, exec and wait commands
#include <time.h> //For current time
#include <sys/stat.h> //For using chmod to change file mode to read only

#define BUFF_SIZE 128 //Assuming that each line contains 128 bytes only

void printCompanyFlyer(char *filePath); //Funtion that prints company flyer, using GetFlyer.c program
void createText(char* filePath); //Function that creates a text file ("x.txt")
void writeIntoText(char *filePath, char *toWrite); //Function that writes a string (toWrite) to a certain file
void searchForPercent(char* filePath, int fileSize, int *percent); //Function that search for company discount percentage and stores it in percent
void splitStringToItemAndQuantity (char* item, char *tempQuantity, int *quantity); //Function that gets item (full string), tempQuantity (string), and quantity (int pointer) and split the string for - item name (stored in item), and quantity
void searchForItemPrice(char *filePath, int fileSize, char *item, int *price); //Function that search for item price and stores it in price
void getDateInString(char *date); //Function that stores the date dd/mm/yyyy

//Funtion that prints company flyer, using GetFlyer.c program
void printCompanyFlyer(char* filePath){
    // Execute the GetFlyer command
    if (fork() == 0) {
        execl("GetFlyer", "GetFlyer", filePath, NULL);
        perror("Execl failed\n");
        exit(EXIT_FAILURE);
    }
    wait(NULL);
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

//Function that writes a string (toWrite) to a certain file
void writeIntoText(char *filePath, char *toWrite){
    int to = open(filePath, O_WRONLY | O_CREAT, 0644); //Opening the file
    //If the file open failed
    if(to == -1){
        perror("Could not open the file");
        exit(EXIT_FAILURE);
    }
    lseek(to, 0, SEEK_END); //Taking the cursor to the end of the file (EOF)
    int wbytes = write(to, toWrite, strlen(toWrite)); //Writing a string into the file
    //If not all the string written to the file
    if(wbytes != strlen(toWrite)){
        perror("Could not write the new text");
        close(to);
        exit(EXIT_FAILURE);
    }
    close(to); //Finish, closing our new file
}

//Function that search for company discount percentage and stores it in percent
void searchForPercent(char* filePath, int fileSize, int *percent){
    char buffer[fileSize]; //Used for reading each line in our text
    char* split; //Split the buffer
    int rbytes; //Reading from the text
    int from = open(filePath, O_RDONLY); //Opening the file
    //If the file open failed
    if(from == -1){
        perror("Could not open the file");
        exit(EXIT_FAILURE);
    }
    if((rbytes = read(from, buffer, fileSize)) > 0){
        split = strtok(buffer, " \n"); //Splitting the string to tokens, " \n" is the delimeter
        //Searching the percent
        while(split != NULL){
            //If we reached to the discount percentage parameter
            if(strstr(split, "%") != NULL){
                //If we can translate it to integer
                if(atoi(split) != 0){
                    *percent = atoi(split); //Storing it in percent pointer
                }
            }
            split = strtok(NULL, " \n"); //Reading next command
        }
    }
    close(from); //Finish, closing our new file
}

//Function that gets item (full string), tempQuantity (string), and quantity (int pointer) and split the string for - item name (stored in item), and quantity
void splitStringToItemAndQuantity (char* item, char *tempQuantity, int *quantity){
    char tempItem[BUFF_SIZE]; //Creating tempItem value so we will be able to split the string without damage item
    char* split; //Split the buffer
    strcpy(tempItem, item); //Copying item and stores it in tempItem
    strcpy(item, "");
    split = strtok(tempItem, " "); //Splitting the string to tokens, " " (space) is the delimeter
    //Finding the price ---> last token
    while(split != NULL){
        strcpy(tempQuantity, split);
        if(atoi(split) == 0){
            strcat(item, split);
            strcat(item, " ");
        }
        split = strtok(NULL, " "); //Reading next command
    }
    item[strlen(item) - 1] = '\0'; //Removing last " " from item
    if(atoi(tempQuantity) != 0){
        *quantity = atoi(tempQuantity);
    }
    else{
        strcpy(tempQuantity, "1");
    }
}

//Function that search for company discount percentage and stores it in percent
void searchForItemPrice(char *filePath, int fileSize, char *item, int *price){
    char buffer[fileSize]; //Used for reading each line in our text
    char *split; //Split the buffer
    int rbytes; //Reading from the text
    int from = open(filePath, O_RDONLY); //Opening the file
    //If the file open failed
    if(from == -1){
        perror("Could not open the file");
        exit(EXIT_FAILURE);
    }
    if((rbytes = read(from, buffer, fileSize)) > 0){
        split = strtok(buffer, "\n\0"); //Splitting the string to tokens, "\n\0" is the delimeter
        //Searching the price
        while(split != NULL){
            //If we reached to the item we have searched for
            if(strstr(split, item) != NULL){
                break;
            }
            split = strtok(NULL, "\n\0"); //Reading next command
        }
    }
    if(split != NULL){
        strcpy(buffer, split); //Copying the whole line to the buffer
        split = strtok(split, " "); //Splitting the string to tokens, " " is the delimeter
        //Searching the price
        while(split != NULL){
            if(strstr(split, "NIS") != NULL){
                //If we can translate it to integer
                if(atoi(split) != 0){
                    *price = atoi(split); //Storing it in percent pointer
                }
            }
            split = strtok(NULL, " "); //Reading next command
        }
    }
    close(from); //Finish, closing our new file
}

//Function that stores the date dd/mm/yyyy
void getDateInString(char *date){
    char temp[BUFF_SIZE];
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(date, "%d", tm.tm_mday);
    strcat(date, "/");
    sprintf(temp, "%d", tm.tm_mon + 1);
    strcat(date, temp);
    strcat(date, "/");
    sprintf(temp, "%d", tm.tm_year + 1900);
    strcat(date, temp);
}

int main (int argc, char* argv[]){
    if (argc != 3){
        printf("Usage: program name, company name, name of the orderer\n");
        exit(EXIT_FAILURE);
    }
    char filePath[BUFF_SIZE], clientPath[BUFF_SIZE], input[BUFF_SIZE], tempQuantity[BUFF_SIZE], name[BUFF_SIZE];
    char* path = "/home/braude/Black_Friday/";
    char* textFormat = ".txt"; //Determining the file format
    int from, fileSize; //Reading from a file
    int percent = 0, price = 0, quantity = 1;
    double sum = 0;
    strcpy(filePath, path); //Copying path to filePath
    strcat(filePath, argv[1]); //Creating filePath
    strcat(filePath, textFormat); //Concatenating the strings filePath and textFormat for one string, creating format "x.txt" - stored in filePath
    from = open(filePath, O_RDONLY); //Opening the file
    //If the file open failed
    if(from == -1){
        printf("Company Not Found!\n");
        exit(EXIT_FAILURE);
    }
    fileSize = (int)lseek(from, 0, SEEK_END);
    close(from);
    strcpy(clientPath, path); // ---> /home/braude/Black_Friday/
    strcat(clientPath, argv[1]); // ---> /home/braude/Black_Friday/BUG
    strcat(clientPath, "_Order/"); // ---> /home/braude/Black_Friday/BUG_Order/
    strcat(clientPath, argv[2]); // ---> /home/braude/Black_Friday/BUG_Order/name
    strcat(clientPath, textFormat); // ---> /home/braude/Black_Friday/BUG_Order/name.txt
    createText(clientPath);
    writeIntoText(clientPath, argv[1]);
    writeIntoText(clientPath, " Order\n\n"); // X Order
    searchForPercent(filePath, fileSize, &percent); // Getting percent in int
    printCompanyFlyer(argv[1]);
    printf("Insert your order (STOP to finish):\n");
    while (fgets(input, sizeof(input), stdin) != NULL && strcmp(input, "STOP\n"))
    {
        input[strcspn(input, "\n")] = 0; //Removing the \n from input created by fgets
        splitStringToItemAndQuantity(input, tempQuantity, &quantity); //Getting name (stored in input) and quantity alone
        searchForItemPrice(filePath, fileSize, input, &price); //Searching for an item (stores in input) and its price (stores in price)
        writeIntoText(clientPath, input); //Writing the item name
        writeIntoText(clientPath, " - "); // -
        writeIntoText(clientPath, tempQuantity); //Writing the item quantity
        writeIntoText(clientPath, "\n\n"); //Linebreak
        sum += (price * quantity); //Summing all
        price = 0;
        quantity = 1;
    }
    // Calculating the total price after discount
    sum *= (100 - percent);
    sum /= 100;
    printf("Total Price: %.1f NIS (Confirm to approve/else cancle)\n", sum);
    if (fgets(input, sizeof(input), stdin) != NULL && !strcmp(input, "Confirm\n")){
        sprintf(input, "%.1f", sum); //Storing the sum price as string (stores in input)
        writeIntoText(clientPath, "Total Price: "); 
        writeIntoText(clientPath, input); //Writing the total price
        writeIntoText(clientPath, " NIS\n\n");
        getDateInString(input); //Getting current date
        writeIntoText(clientPath, input); // Writing the order date
        chmod(clientPath, 0444); //Changing file to read only
        strcpy(name, argv[2]);
        strcat(name, textFormat);
        printf("Order created! // %s Created in ", name);
        strcpy(name, argv[1]);
        strcat(name, "_Order");
        printf("%s Dir with Read Mode\n", name);
    }
    else{
        remove(clientPath); //Removing the file order if the customer chosed not to Confirm
    }
    return 0; //Finish
}