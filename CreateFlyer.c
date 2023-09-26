#include <stdio.h> //Standart input and output like printf
#include <stdlib.h> //Standart library
#include <string.h> //String library
#include <fcntl.h> //File control options - For access modes like O_RDONLY

#define BUFF_SIZE 128 //Assuming that each line contains 128 bytes only

void createDirectory(char* filePath);
void createText(char* filePath); //Function that creates a text file ("x.txt")
void writeIntoText(char* filePath, char* toWrite); //Function that writes a string (toWrite) to a certain file
void createTitle(char* string); //Function that creates titles (header and a footer)
void splitStringToItemAndPrice(char* item, char* price); //Function that gets item (full string) and price (empty string) and split the string for - item name (stored in item), and price (stored in price)
void createItemAndPriceWithDots(char* item, char* price, char* strWithDots); //Function that creates a print/write format for item and its price, and stores it in strWithDots

// Creating a new empty dir
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

//Function that writes a string (toWrite) to a certain file
void writeIntoText(char* filePath, char* toWrite){
    int wbytes;
	int to = open(filePath, O_WRONLY | O_CREAT, 0644); //Opening the file
    //If the file open failed
    if(to == -1){
        perror("Could not open the file");
        exit(EXIT_FAILURE);
    }
    lseek(to, 0, SEEK_END); //Taking the cursor to the end of the file (EOF)
    wbytes = write(to, toWrite, strlen(toWrite)); //Writing a string into the file
    //If not all the string written to the file
    if(wbytes != strlen(toWrite)){
        perror("Could not write the new text");
        close(to);
        exit(EXIT_FAILURE);
    }
    close(to); //Finish, closing our new file
}

//Function that creates titles (header and a footer)
void createTitle(char* title){
    char newString[BUFF_SIZE] = ""; //Stores a title (header or footer)
    int numOfSpaces, i; //numOfSpaces - as its name |---| i - for loop
    numOfSpaces = ((BUFF_SIZE / 2) - strlen(title) - 2) / 2; //The number of spaces is determining by half buffer size excluding: title length, and twice "\n" (length 2)
    strcat(newString, "\n"); //Linebreak
    for(i = 0; i < numOfSpaces; i++){
        strcat(newString, " ");
    }
    strcat(newString, title);
    strcat(newString, "\n"); //Linebreak
    strcpy(title, newString); //Storing newString we have created back to the original title so we would be able to use it in the original function
}

//Function that creates a print/write format for item and its price, and stores it in strWithDots
void createItemAndPriceWithDots(char* item, char* price, char* strWithDots){
    strcpy(strWithDots, ""); //Clearing previous string
    int numOfDots, i; //numOfDots - as the name |---| i - for loop
    numOfDots = (BUFF_SIZE / 2) - strlen(item) - strlen(price) - 5; //The number of dots is determining by half buffer size excluding: our string length, a " " (space) before price (length 1), and the last "NIS\n" (length 4)
    strcat(strWithDots, item); //Writing the item name
    //Put dots '.' in our new string
    for(i = 0; i < numOfDots; i++){
        strcat(strWithDots, ".");
    }
    strcat(strWithDots, " "); //Put a space before writing the price
    strcat(strWithDots, price); //Writing the price
    strcat(strWithDots, "NIS\n"); //Writing "NIS"
}

//Function that gets item (full string) and price (empty string) and split the string for - item name (stored in item), and price (stored in price)
void splitStringToItemAndPrice(char* item, char* price){
    char tempItem[BUFF_SIZE]; //Creating tempItem value so we will be able to split the string without damage item
    char* split; //Split the buffer
    strcpy(tempItem, item); //Copying item and stores it in tempItem
    strcpy(item, "");
    split = strtok(tempItem, " "); //Splitting the string to tokens, " " (space) is the delimeter
    //Finding the price ---> last token
    while(split != NULL){
        strcpy(price, split); //Getting the last "word" in a string (should be a price)
        if(atoi(split) == 0){
            strcat(item, split);
            strcat(item, " ");
        }
        split = strtok(NULL, " "); //Reading next command
    }
    item[strlen(item) - 1] = '\0'; //Removing last " " from item
    if(atoi(price) == 0){
        strcpy(price, "0"); //No price entered - unknown
    }
}

int main (int argc, char* argv[]){
    if (argc != 3){
        printf("Usage: program name, advertising company name, discount precentage\n");
        exit(EXIT_FAILURE);
    }
    char filePath[BUFF_SIZE]; //Creating filePath so we will not change argv[1]
    char* path = "/home/braude/Black_Friday/";
    char* dirFormat = "_Order"; //Determining the dir format
    char* textFormat = ".txt"; //Determining the file format
    char input[BUFF_SIZE], item[BUFF_SIZE], price[BUFF_SIZE], strWithDots[BUFF_SIZE];
    int choosedNum = 0;
    //Creating directory
    strcpy(filePath, path);
    strcat(filePath, argv[1]);
    strcat(filePath, dirFormat);
    createDirectory(filePath);
    //Creating a text file
    strcpy(filePath, path);
    strcat(filePath, argv[1]); //Creating filePath
    strcat(filePath, textFormat); //Concatenating the strings filePath and textFormat for one string, creating format "x.txt" - stored in filePath
    createText(filePath); //Creating the desired text
    //Creating the header - X Sale
    strcpy(input, argv[1]);
    strcat(input, " Sale");
    createTitle(input);
    writeIntoText(filePath, input); //Writing it to our text
    //Creating the header - X% Off
    strcpy(input, argv[2]);
    strcat(input, "% Off");
    createTitle(input);
    writeIntoText(filePath, input); //Writing it to our text
    writeIntoText(filePath, "\n"); //Writing a linebreak
    printf("Insert item and item cost:\n");
    while (fgets(input, sizeof(input), stdin) != NULL && strcmp(input, "Stop\n"))
    {
        input[strcspn(input, "\n")] = 0; //Removing the \n from input created by fgets
        strcpy(item, input);
        splitStringToItemAndPrice(item, price);
        createItemAndPriceWithDots(item, price, strWithDots);
        writeIntoText(filePath, strWithDots); //Writing it to our text
    }
    printf("What is your choice?\n");
    printf("1. Enjoyed\n");
    printf("2. Shop Now and have fun\n");
    printf("3. Have Fun\n");
    printf("4. Other\n");
    while((choosedNum < 1 || choosedNum > 4) && fgets(input, sizeof(input), stdin) != NULL){
        choosedNum = atoi(input);
        switch(choosedNum){
            case 1:
                strcpy(input, "Enjoyed");
                createTitle(input);
                writeIntoText(filePath, input); //Writing it to our text
                break;
            case 2:
                strcpy(input, "Shop Now and have fun");
                createTitle(input);
                writeIntoText(filePath, input); //Writing it to our text
                break;
            case 3:
                strcpy(input, "Have Fun");
                createTitle(input);
                writeIntoText(filePath, input); //Writing it to our text
                break;
            case 4:
                printf("Insert your word:\n");
                if(fgets(input, sizeof(input), stdin) != NULL){
                    createTitle(input);
                    writeIntoText(filePath, input); //Writing it to our text
                }
                break;
            default:
                printf("Must choose numbers between 1-4. Please try again.\n");
                break;
        }
    }
    strcpy(filePath, path);
    strcat(filePath, "camp_partic.txt");
    writeIntoText(filePath, argv[1]); //Writing it to our text
    writeIntoText(filePath, "\n\n"); //Writing it to our text
    strcpy(input, argv[1]); //Copying name
    strcat(input, textFormat); //Creating format "x.txt"
    printf("Successfully created // %s created , ", input);
    strcpy(input, argv[1]);
    strcat(input, dirFormat);
    printf("%s dir created\n", input);
    return 0;
}