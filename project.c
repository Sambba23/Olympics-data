#include "project.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//struct for handling all of the information
struct Country {
    char name[256];
    int golds;
    int silvers;
    int bronzes;
};

struct Country countrys[100]; //Database
int allCountrys = 0; //number of all countrys in database

//function for commands that start with a
void ACommand(char* input) {
    char* helper = strchr(input, ' ');
    //Handler errors
    if (helper == NULL || strlen(helper) <= 1) {
        printf("A should be followed by exactly 1 argument.\n");
        return;
    }
    //Handle more errors
    char* temp = strtok(helper+1, "\n");
    if (temp == NULL || strspn(temp, " ") == strlen(temp)) {
        printf("A should be followed by exactly 1 argument.\n");
        return;
    }
    if (helper != NULL) {
        
        char* temp = strtok(helper+1, "\n");
        
        // does it exist?
        for (int i = 0; i < allCountrys; i++) {

            if (strcmp(temp, countrys[i].name) == 0) {

                printf("Nation \"%s\" is already in the database.\n", temp);
                return;

            }
        }
        
        //create new counrty
        struct Country new;
        strcpy(new.name, temp);
        new.golds = 0;
        new.silvers = 0;
        new.bronzes = 0;
        
        //add to database
        countrys[allCountrys] = new;
        allCountrys += 1;
        printf("SUCCESS\n");
        
    }
}

//Here we handle the case where M is the first letter
void M(char* input) {
    char* help = strchr(input, ' ');
    //Error handler
    if (help == NULL || strlen(help) <= 1) {
        printf("M should be followed by exactly 4 arguments.\n");
        return;
    }
    
    char* temporary = strtok(input + 2, " ");

    int golds, silvers, bronzes;
    //Error Handler
    char* nextItem = strtok(NULL, " ");

    if (nextItem == NULL || sscanf(nextItem, "%d", &golds) != 1) {
        printf("M should be followed by exactly 4 arguments.\n");
        return;
    }

    //Error Handler
    nextItem = strtok(NULL, " ");
    if (nextItem == NULL || sscanf(nextItem, "%d", &silvers) != 1) {
        printf("M should be followed by exactly 4 arguments.\n");
        return;
    }

    //Error Handler
    nextItem = strtok(NULL, " ");
    if (nextItem == NULL || sscanf(nextItem, "%d", &bronzes) != 1) {
        printf("M should be followed by exactly 4 arguments.\n");
        return;
    }

    //find the country
    int numberOf = -1;
    for (int i = 0; i < allCountrys; i++) {

        if (strcmp(temporary, countrys[i].name) == 0) {

            numberOf = i;
            break;
        }
    }

    if (numberOf == -1) {

        printf("Invalid command %s %d %d %d\n", input, golds, silvers, bronzes);
        return;
    }

    countrys[numberOf].golds += golds;
    countrys[numberOf].silvers += silvers;
    countrys[numberOf].bronzes += bronzes;

    printf("SUCCESS\n");
}


//helper function for sorting all the counrtys
int compare(const void* a, const void* b) {

    const struct Country* first = (const struct Country*)a;
    const struct Country* sec = (const struct Country*)b;
    
    if (first->golds != sec->golds) {
        return sec->golds - first->golds;
    }
   
    if (first->silvers != sec->silvers) {
        return sec->silvers - first->silvers;
    }
   
    return sec->bronzes - first->bronzes;
}
//If the first letter is L
void LCommand(void) {
    if (allCountrys != 0) {
        qsort(countrys, allCountrys, sizeof(struct Country), compare);
        
        for (int i = 0; i < allCountrys; i++) {
            printf("%s %d %d %d\n", countrys[i].name, countrys[i].golds, countrys[i].silvers, countrys[i].bronzes);
        }
        
        printf("SUCCESS\n");
    }
}

void WCommand(char* input) {
    char* helps = strchr(input, ' ');
    
    if (helps != NULL) {
       
        char filename[100];
        strncpy(filename, helps + 1, sizeof(filename) - 1);
        filename[strcspn(filename, "\n")] = '\0';
        
    
        FILE* file = fopen(filename, "w");
        
        if (file == NULL) {
            printf("Invalid command %s\n", input);
            return;
        }
        
        // Write info to file
        for (int i = 0; i < allCountrys; i++) {
            fprintf(file, "%s %d %d %d\n", countrys[i].name, countrys[i].golds, countrys[i].silvers, countrys[i].bronzes);
        }
        
        
        fclose(file);
        printf("SUCCESS\n");
    }
}

void OCommand(char* input) {

    char* filename = strtok(input + 2, " \n");

    FILE* file = fopen(filename, "r");

    if (file == NULL) {
        printf("Cannot open file %s for reading.\n", filename);
        return;
    }
    
    // Reads all of the data
    struct Country* temp_countrys = calloc(100, sizeof(struct Country));
    int temp_numOfCountrys = 0;
    char temp_name[100];
    int temp_golds, temp_silvers, temp_bronzes;
    
    while (fscanf(file, "%[^ ] %d %d %d\n", temp_name, &temp_golds, &temp_silvers, &temp_bronzes) == 4) {
        strcpy(temp_countrys[temp_numOfCountrys].name, temp_name);
        temp_countrys[temp_numOfCountrys].golds = temp_golds;
        temp_countrys[temp_numOfCountrys].silvers = temp_silvers;
        temp_countrys[temp_numOfCountrys].bronzes = temp_bronzes;
        temp_numOfCountrys += 1;
    }
    
    //change the current info
    memcpy(countrys, temp_countrys, sizeof(struct Country) * temp_numOfCountrys);
    allCountrys = temp_numOfCountrys;
    
    free(temp_countrys);
    fclose(file);
    printf("SUCCESS\n");
}

//Main function!
int main(void) {
    char input[256];
    char firstLetter;
    
    while (1) {
        fgets(input, sizeof(input), stdin);
        
        // get the first letter:
        firstLetter = input[0];

        //Quit:
        if (firstLetter == 'Q') {
            printf("SUCCESS\n");
            break;
        }
        //Add a country:
        else if (firstLetter == 'A') {
            ACommand(input);
        }
        else if (firstLetter == 'L') {
            LCommand();
        }
        else if (firstLetter == 'W') {
            WCommand(input);
        }
        else if (firstLetter == 'M'){
            M(input);
        }
        else if (firstLetter == 'O') {
            OCommand(input);
        }
        //Handles other cases
        else {
            printf("Invalid command %s\n", input);;
        }
    }

    return 0;
}
