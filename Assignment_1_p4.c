#include <stdio.h>
#include <stdlib.h>
#include <string.h> //for strcpy and strcmp
#include <ctype.h>  //for isalnum

#define MAX_STRING_SIZE 40 //max length of a string
#define ARRAY_SIZE 99991  //best be prime
#define namePrompt "Enter term to get frequency or type \"quit\" to escape\n" //you can use this string as your invitation to the user
#define newLinePrompt ">>> " //you can use this string as your invitation to the user where the user should input the name

int collisions = 0;
int numTerms = 0;

typedef struct Node Node;
struct Node
{
    char personID[MAX_STRING_SIZE];
    char depositionID[MAX_STRING_SIZE];
    char surname[MAX_STRING_SIZE];
    char forename[MAX_STRING_SIZE];
    char age[4]; //name cant be >999
    char personType[MAX_STRING_SIZE];
    char gender[MAX_STRING_SIZE];
    char nationality[MAX_STRING_SIZE];
    char religion[MAX_STRING_SIZE];
    char occupation[MAX_STRING_SIZE];

    Node *next; // node pointer to next node
};


Node *hashTable[ARRAY_SIZE];

void initTable(){
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        hashTable[i] = NULL;
    }
}

int hashFunc(char *s)
{
    int hash = 0;
    while (*s)
    {
        hash = (hash + *s - 'A') % ARRAY_SIZE;
        s++;
    }
    return hash;
}

int hashFunc3(char *s)
{
    int hash = 0;
    while (*s)
    {
        hash = 1 + (hash + *s - 'A') % (ARRAY_SIZE - 1);
        s++;
    }
    return hash;
}

void PrintList(Node *head)
{
    Node *current = head;
    printf(newLinePrompt);
    printf("\t%-15s", "Person ID");
    printf("%-15s", "Deposition ID");
    printf("%-15s", "Surname");
    printf("%-15s", "Forename");
    printf("%-15s", "Age");
    printf("%-15s", "Person Type");
    printf("%-15s", "Gender");
    printf("%-15s", "Nationality");
    printf("%-15s", "Religion");
    printf("%-15s\n", "Occupation");
   

    while (current != NULL)
    {
        printf("\t%-15s", current->personID);
        printf("%-15s", current->depositionID);
        printf("%-15s", current->surname);
        printf("%-15s", current->forename);
        printf("%-15s", current->age);
        printf("%-15s", current->personType);
        printf("%-15s", current->gender);
        printf("%-15s", current->nationality);
        printf("%-15s", current->religion);
        printf("%-15s\n", current->occupation);
        current = current->next;
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
            printf("\t%i\t", i);
            PrintList(hashTable[i]);
            printf("\n");
        }
    }
}

int search (char* s){
    //TODO4
    int key = 0;
    int incr = abs(hashFunc3(s));
    int start = abs(hashFunc(s));



    for (int j = 0; ; j++)
    {
        key = (start + (j * incr)) % ARRAY_SIZE;
        if (hashTable[key] == NULL)
        {
            return -1;
        }
        if (strcmp(hashTable[key]->surname, s) == 0)
        {
            return key;
        }
    }

    return -1;
}

void AddNode(Node **head, Node *newNode)
{
    newNode->next = *head; // pushes head down the list
    *head = newNode;       // value of head is = to new node
}

void addOrLinkNode(Node *in){
    int key = 0;
    if (search(in->surname) != -1){
        //then name is found so
        key = search(in->surname);
        AddNode(&hashTable[key], in);
        return;
    }

    //if name doesnt exist, numTerms++;
    numTerms++;

    int start = abs(hashFunc(in->surname));
    int incr = abs(hashFunc3(in->surname));

  
    for (int j = 0; ; j++)
    {
        key = (start + (j * incr)) % ARRAY_SIZE;
        if (hashTable[key] == NULL)
        {
            hashTable[key] = in;
            // printf("Inserting %15s at %d\n", name,key);

            return;
        }
        else
        {
            //printf("%s Collision at %15d\n", hashTable[key]->surname, key);
            collisions++;
        }
    }
}

void createNode(char *pID, char *dID, char *SN, char *FN, char *AGE, char *pTYPE, char *GEN, char *NAT, char* REL, char *OCC){
    Node *head = (Node *)malloc(sizeof(Node));
    strcpy(head->personID, pID);
    strcpy(head->depositionID, dID);
    strcpy(head->surname, SN);
    strcpy(head->forename, FN);
    strcpy(head->age, AGE);
    strcpy(head->personType, pTYPE);
    strcpy(head->gender, GEN);
    strcpy(head->nationality, NAT);
    strcpy(head->religion, REL);
    strcpy(head->occupation, OCC);

    head->next = NULL;

    addOrLinkNode(head);
}

void printDataWithSurname(char *SN){
    if(search(SN) != -1){
        //then name is found
        int key = search(SN);
        PrintList(hashTable[key]);
    }else{
        printf(newLinePrompt);
        printf("%s not in table\n", SN);
    }
}

void next_token(char *buf, FILE *f, int string_max)
{
    // start by skipping any characters we're not interested in
    buf[0] = fgetc(f);
    int quoted = 0;
    while ((!isalnum(buf[0])) && (!feof(f)) && (buf[0] != '*') && (buf[0] != '[') && (buf[0] != '{') && (buf[0] == '"'))
    {
        //move on
        if(buf[0] == '"'){ 
            quoted=!quoted; //turn on quotes
            buf[0] = fgetc(f);//get next character which will be "
            if(buf[0] == '"'){
                //then double quoted
                //making my own string here
                quoted=!quoted;
                buf[0]=' ';
                buf[1]='\0';
                return;
            }
           
        }else{
            buf[0] = fgetc(f);
        }
    }

    int i = 1;
    for (;;)
    {
        
        buf[i] = fgetc(f); // get next character from file

        if(buf[i] == '"'){ quoted=!quoted; buf[i] = fgetc(f);}
        if(buf[i] == ',' && !quoted){ break; }
        if(buf[i] == '\n'){ break; }
     

        if (feof(f))
        {
            break;
        } // file ended?
        if (i < (string_max - 1))
        {
            ++i;
        } // truncate strings that are too long
    }
    buf[i] = '\0'; // NULL terminate the string
}

//  Reads the contents of a file and adds them to the hash table - returns 1 if file was successfully read and 0 if not.
int load_file(char *fname)
{
    FILE *f;
    char buf[MAX_STRING_SIZE];
    char tempPID[MAX_STRING_SIZE];
    char tempDID[MAX_STRING_SIZE];
    char tempSN[MAX_STRING_SIZE];
    char tempFN[MAX_STRING_SIZE];
    char tempAGE[MAX_STRING_SIZE];
    char tempPT[MAX_STRING_SIZE];
    char tempG[MAX_STRING_SIZE];
    char tempN[MAX_STRING_SIZE];
    char tempR[MAX_STRING_SIZE];
    char tempO[MAX_STRING_SIZE];

    // boiler plate code to ensure we can open the file
    f = fopen(fname, "r");
    if (!f)
    {
        printf("Unable to open %s\n", fname);
        return 0;
    }
    int k = 0;
    // read until the end of the file
    while (!feof(f))
    {
        k++;
        next_token(buf, f, MAX_STRING_SIZE);
        if(feof(f)){
            break;
        }
        strcpy(tempPID, buf);
        next_token(buf, f, MAX_STRING_SIZE);
        strcpy(tempDID, buf);
        next_token(buf, f, MAX_STRING_SIZE);
        strcpy(tempSN, buf);
        next_token(buf, f, MAX_STRING_SIZE);
        strcpy(tempFN, buf);
        next_token(buf, f, MAX_STRING_SIZE);
        strcpy(tempAGE, buf);
        next_token(buf, f, MAX_STRING_SIZE);
        strcpy(tempPT, buf);
        next_token(buf, f, MAX_STRING_SIZE);
        strcpy(tempG, buf);
        next_token(buf, f, MAX_STRING_SIZE);
        strcpy(tempN, buf);
        next_token(buf, f, MAX_STRING_SIZE);
        strcpy(tempR, buf);
        next_token(buf, f, MAX_STRING_SIZE);
        strcpy(tempO, buf);


        if(strcmp(tempPID, "Person ID") != 0){
            createNode(tempPID, tempDID, tempSN, tempFN, tempAGE, tempPT, tempG, tempN, tempR, tempO);
        }
    }
    printf("%d\n", k);

    printf("File %s loaded\n", fname);
    float load = ((float)numTerms / (float)ARRAY_SIZE);
    printf(" Capacity: %i\n Num Terms: %i\n Collisions: %i\n Load: %f\n", ARRAY_SIZE, numTerms, collisions, load);
    // always remember to close your file stream
    fclose(f);

    return 1;
}


int main()
{
    load_file("people.csv");

    char input[MAX_STRING_SIZE];

    printf(namePrompt);
    scanf("%s", input);

    while (strcmp(input, "quit") != 0)
    {
        if (strcmp(input, "print") == 0)
        {
            printTable();
        }
        else
        {
            printDataWithSurname(input);
        }
        scanf("%s", input);
    }
    printf(newLinePrompt);

    return 0;
}
