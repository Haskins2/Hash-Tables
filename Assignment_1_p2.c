#include<stdio.h>
#include<stdlib.h>
#include <string.h> //for strcpy and strcmp
#include <ctype.h>  //for isalnum

#define MAX_STRING_SIZE 20 //max length of a string
#define ARRAY_SIZE 59  //best be prime
#define NAME_PROMPT "Enter term to get frequency or type \"quit\" to escape\n" //you can use this string as your invitation to the user
#define NEW_LINE_PROMPT ">>> " //you can use this string as your invitation to the user where the user should input the name

typedef struct Element Element;
struct Element{
    // TODO1
    char elementName[MAX_STRING_SIZE];
    int quantity;
};

Element* hashTable[ARRAY_SIZE];
int collisions = 0;
int num_terms = 0;


//djb2 hashing algorithm
//https://theartincode.stanis.me/008-djb2/#:~:text=The%20simple%20C%20function%20starts,the%20current%20character%20to%20it.
int hashFunc(char* s){
    unsigned long hash = 5381;
    int c;

    while ((c = *s++)) {
        hash = ((hash << 5) + hash) + c; 
    }

    return hash % ARRAY_SIZE;
}


void initTable(){
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        hashTable[i] = NULL;
    }
}

void printTable()
{
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        if (hashTable[i] == NULL)
        {
            printf("\t%i\t---\n", i);
        }
        else
        {
            printf("\t%i\t%s\n", i, hashTable[i]->elementName);
        }
    }
}

Element* createNewElement(char* name){
    Element *newElement = (Element *)malloc(sizeof(Element));

    // inserting newElement values
    strcpy(newElement->elementName, name);
    newElement->quantity = 1;

	return newElement;
}


// returns the element with name name or NULL if the element is not present
int search (char* name){
    int key = hashFunc(name);
    if(hashTable[key] == NULL){
        return -1;
    }else{
        while(hashTable[key] != NULL){
            if(strcmp(hashTable[key]->elementName, name) == 0){
                return key;
            }
            key++;
        }
    }
    return -1;
}


// prints the number of occurences, or 0 if not in the file
void printNumberOfOccurences(char* name){
    int key = search(name);
    if(key == -1){
        printf(NEW_LINE_PROMPT);
        printf("%s - %i \n", name, 0);
        return;
    }
    if(strcmp(hashTable[key]->elementName, name)==0){
        printf(NEW_LINE_PROMPT);
        printf("%s - %i \n", name, hashTable[key]->quantity);
    }
}

//searches the name in the array, if it is there increment its count, if not, add it
void addOrIncrement(char* name){
    int key = 0;
    if (search(name) != -1){
        //then name is already in table
        key = search(name);
        //increment quantity
        hashTable[key]->quantity++;
        return;
    }
    
    num_terms++;
    //continue if name isnt found
    key = hashFunc(name);
    if (hashTable[key] == NULL){
        //then inser name
        hashTable[key] = createNewElement(name);
        return;
    }

    //continue if space is occupied
    while(hashTable[key] != NULL){
        key++; //move onto next slot
        collisions++;
        if(hashTable[key] == NULL){
            hashTable[key] = createNewElement(name);
            return;
        }
    }
}



// From assignment 0
// Reads strings of alpha numeric characters from input file. Truncates strings which are too long to string_max-1
void next_token ( char *buf, FILE *f, int string_max ) {
	// start by skipping any characters we're not interested in
	buf[0] = fgetc(f);
	while ( !isalnum(buf[0]) && !feof(f) ) { buf[0] = fgetc(f); }
	// read string of alphanumeric characters
	int i=1;
	for (;;) {
		buf[i] = fgetc(f);                // get next character from file
		if( !isalnum(buf[i]) ) { 
            if(buf[i]!=' '){ // we still want to keep spaces
                break; // only load letters and numbers
            }
        } 
		if( feof(f) ) { break; }          // file ended?
		if( i < (string_max-1) ) { ++i; } // truncate strings that are too long
	}
	buf[i] = '\0'; // NULL terminate the string
}


//  Reads the contents of a file and adds them to the hash table - returns 1 if file was successfully read and 0 if not.
int load_file ( char *fname ) {
	FILE *f;
	char buf[MAX_STRING_SIZE];

	// boiler plate code to ensure we can open the file
	f = fopen(fname, "r");
	if (!f) { 
		printf("Unable to open %s\n", fname);
		return 0; 
	}
	
	// read until the end of the file
	while ( !feof(f) ) {
		next_token(buf, f, MAX_STRING_SIZE);
		addOrIncrement( buf);
	}
    
    printf("File %s loaded\n", fname);
    float load = ((float)num_terms/(float)ARRAY_SIZE);
    printf(" Capacity: %i\n Num Terms: %i\n Collisions: %i\n Load: %f\n", ARRAY_SIZE, num_terms, collisions,load);
	// always remember to close your file stream
	fclose(f);

	return 1;
}


int main ( int argc, char *argv[] ) {

    initTable();
    load_file("names.csv");
    char input[MAX_STRING_SIZE];
    printf(NAME_PROMPT);
    while (strcmp(input, "quit") != 0)
    {
        scanf("%s", input);
        if (strcmp(input, "print") == 0)
        {
            printTable();
        }
        if (strcmp(input, "quit") == 0){
            break;
        }
        else
        {
            printNumberOfOccurences(input);
        }
    }
    printf(NEW_LINE_PROMPT);
    
    
    return 0;
}