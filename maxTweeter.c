#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include<stdio.h>
#include<stdlib.h>

struct Node
{
     /* nodes of the linked list data structure */
     int count;
     int tweetCount;
     char person[60]; /*name of the tweeter */
     struct Node *next;
};

typedef struct Node *node_t;

/*
this function find a tweeter inside the linked list, if not found then create
a new node for it and add it to the list. It also update the count of the tweeter
if found
Return head pointer of the list of tweeters
*/
node_t find(node_t node,char * person){

     node_t head = node;
     if(node == NULL ){
          /* No list exist yet, person is the first element */
          node_t first = (struct Node*)malloc(sizeof(struct Node));
          strcpy(first->person,person); /* update person */
          first->count = 1; /* counted once */
          first->next = NULL;  /* no next node yet*/
          first->tweetCount = 1;/* counted once for printing */
          return first;
     }
     while(node != NULL){
          /* if there is list, then loop until person is found */
          if(strcmp(node->person, person) == 0){
               /* person found in the list \, updadte counts and return head of the list*/
               node->count++;
               node->tweetCount++;
               return head;
          }
          node = node->next;
     }
     /*  person was not found inside the linked list, so create a new node for
     the person add to the llist with updated count */
     node_t newNode = (struct Node*)malloc(sizeof(struct Node));
     newNode->next = head->next;
     head->next = newNode;
     strcpy(newNode->person,person);
     newNode->count = 1;
     newNode->tweetCount = 1;
     return head;

}


int main(int argc, char **argv)
{
     if(argc != 2) {
          printf("Error: Invalid Arguments, Extected: <executable> <filepath>\n");
          return -1;
     }

     FILE *fd = fopen(argv[1], "r"); /* open file */
     if(fd == NULL){ /* file not open */
          printf("Cannot open file with path: %s\n", argv[1]);
          perror("Error: ");
          return -1;
     }

     node_t head = NULL; /* head of the node */
     size_t LINE_SIZE = 500; /* maximum line size */
     int nameColumn = -1; /* column in which tweeter names are */
     ssize_t read;
     bool parsingNameCol = true; /* to toggle between first and rest of the column*/
     char *line = NULL;
     while( (read = getline(&line, &LINE_SIZE, fd)) != -1){
          /* scan one line at a time from the file until end of file */
          if(read > LINE_SIZE){
               printf("Error: line length is greater than MAXIMUM \n");
               return -1;
          }
          if(parsingNameCol){
               /* first line of the file containing column number of the name column*/
               char *token[LINE_SIZE];	/* to hold tokens after parsing of maximum line size */
               token[0] = strtok(line, ",");  // extract the first string separated by ","
               int i = 0;
               int token_count = 0;
               while (token[i] != NULL)
               {
                    /* extract all the arguments separated by ',' */
                    token_count++;
                    if(strcmp(token[i],"\"name\"")==0){
                    /* if name column found then set it to nameColumn */
                    nameColumn = i;
               }
               i++; /* parse next token */
               token[i] = strtok (NULL, ",");
          }
          if(nameColumn == -1) {
               /* there is no name column in the first line */
               printf("Error: Invalid file: Missing tweeter name column \n");
               return -1;
          }
          /* name col parsed */
          parsingNameCol = false;
     }
     else{
          /* not first column, so now parse tweeter names and add them to the link list */
          char *token[LINE_SIZE];	/* to hold tokens after parsing of maximum line size */
          token[0] = strtok(line, ",");  // extract the first string separated by ","
          int i = 0;
          int token_count = 0;
          while (token[i] != NULL)
          {
               /* extract all the arguments separated by ',' */
               token_count++;
               if(i == nameColumn){
               /* if name column found then set it to nameColumn */
               break;
               }
               i++;/* parse next token */
               token[i] = strtok (NULL, ",");
          }
          if(strlen(token[nameColumn]) > 60){
               /* Name is greater than the MAX character length of LINE_SIZE */
               printf("Error: Name of tweeter at index %s is greater than MAX(60) character length\n", token[0]);
               return -1;
          }
          /* update the list with the current tweeter's count, and collect the head pointer of the list */
          head = find(head, token[nameColumn]);

}// else
}// fgets while



node_t currNode = head; /* curr pointer pointing to head of list */
int maxCount = 0;
node_t topTen[10];  /* array of nodes to store ten most tweeted owners */
for (int j = 0; j < 10; j++) {
     /* count ten times */
          while(currNode != NULL) {
               /* until the end of the list find the tweeter with max count */
          if(currNode->count > maxCount){
               /* tweeter with count maximum then the previous ones */
               maxCount = currNode->count;
               topTen[j] = currNode; /* temporarily store in the array */
          }
          currNode = currNode->next; /* point to the next node */
     }
     /* decrement count to find the next maximum, since this one will not be maximum anyore  */
     topTen[j]->count = -1;
     currNode = head;
     maxCount = 0;
}

node_t sortedtopTen[10]; /* array of nodes to store ten most tweeted owners in decreasing order */
currNode = head; /* curr pointer pointing to head of list */
maxCount = 0;
for (int j = 0; j < 10; j++) {
          while(currNode != NULL) {
               /* until the end of the list find the tweeter with max count */
          if(currNode->tweetCount > maxCount){
               /* tweeter with count maximum then the previous ones */
               maxCount = currNode->tweetCount;
               sortedtopTen[j] = currNode; /* temporarily store in the array */
          }
          currNode = currNode->next;/* point to the next node */
     }
     currNode = head;
     maxCount = 0;
     printf("%s: %d\n",topTen[j]->person, topTen[j]->tweetCount);
     /* decrement count to find the next maximum, since this one will not be maximum anyore  */
     sortedtopTen[j]->tweetCount = -1;
}

return 0;
}
