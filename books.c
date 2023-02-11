#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "defs.h"

/*
  Function: initList
  Purpose:  initializes the required fields of the given book list
   in-out:  the allocated and initialized book collection
   return:  nothing
*/
void initList(BookListType *list)
{
    list->head = NULL;
    list->tail = NULL;
}

/*
  Function: initBook
  Purpose:  initializes the required fields of the given book
       in:  the id
       in:  the title
       in:  the author
       in:  the year
       in:  the status       
   in-out:  the allocated and initialized book
   return:  nothing
*/
void initBook(int i, char *t, char *a, int y, BookStatusType st, BookType **book)
{
    *book = malloc(sizeof(BookType));

    (*book)->id = i;
    strcpy((*book)->title, t);
    strcpy((*book)->author, a);
    (*book)->year = y;
    (*book)->status = st;
}

/*
  Function: addBook
  Purpose:  inserts the new book into the book collection, directly in its correct 
            position, in ascending order by title
       in:  the book
   in-out:  the book collection
   return:  nothing
*/
void addBook(BookListType *list, BookType *b)
{
    NodeType *currNode;
    NodeType *prevNode;
    NodeType *newNode;

    currNode = list->head;
    prevNode = NULL;

    while (currNode != NULL)
    {
        if (strcmp(b->title, currNode->data->title) < 0)
        {
            break;
        }

        prevNode = currNode;
        currNode = currNode->next;
    }

    newNode = malloc(sizeof(NodeType)); // BEFORE --> newNode = malloc(sizeof(BookType)); 
    newNode->data = b;
    newNode->next = NULL;
    
    if (prevNode == NULL)  // update head
    {
        list->head = newNode;
    }
    else
    {
        prevNode->next = newNode;
    }

    if (currNode == NULL) // update tail
    {
        list->tail = newNode;
    }
    else
    {
        newNode->next = currNode;
    }
}

/*
  Function: findBook
  Purpose:  traverses the given book collection to find the book with the given 
            id, and returns the found book pointer using the b parameter
       in:  the id
       in:  the book collection
   in-out:  the book
   return:  C_OK for success, C_NOK for failure
*/
int findBook(BookListType *list, int id, BookType **b)
{
    NodeType *currNode;

    currNode = list->head;

    while (currNode != NULL)
    {
        if (currNode->data->id == id)
        {
            (*b) = currNode->data;
            return C_OK;
        }
        
        // next node
        currNode = currNode->next;
    }
    return C_NOK;
}

/*
  Function: formatBook
  Purpose:  formats all the information for a single book into one long 
            string, and stores that string in the outStr parameter, 
            which is “returned” to the calling function
       in:  the book
      out:  the formatted string
   return:  nothing
*/
void formatBook(BookType *b, char *outStr)
{
    char status[MAX_STR];
    convertStatus(b->status, status);

    sprintf(outStr, "%d : %-38s : %-18s : %d : %-s", b->id, b->title, b->author, b->year, status);
}

/*
  Function: convertStatus
  Purpose:  convert the book status enumerated data type to a meaningful string
       in:  the book status type
      out:  the book status as a string
   return:  nothing
*/
void convertStatus(BookStatusType status, char *string)
{
    switch(status)
    {
       case CHECKED_IN:
            strcpy(string, "Checked In");
            break;
        case CHECKED_OUT:
            strcpy(string, "Checked Out");
            break;
        case UNDER_REPAIR:
            strcpy(string, "Under Repair");
            break;
        case LOST:
            strcpy(string, "Lost");
            break;
        default:
            // NOTHING
            break;
    }
}

/*
  Function: formatBooks
  Purpose:  traverses the book collection list, formats the information for each 
            individual book, and populates the outStr parameter with the entire 
            contents of the list, with each book separated from the other with 
            a “new line” character
       in:  the book collection
      out:  the formatted string
   return:  nothing
*/
void formatBooks(BookListType *list, char *outStr)
{
    char formattedBook[MAX_BUFF];
    NodeType *currNode;

    currNode = list->head;

    while (currNode != NULL)
    {
        formatBook(currNode->data, formattedBook);
        
        if (currNode == list->head)
        {
            strcpy(outStr, formattedBook);
        }
        else
        {
            strcat(outStr, "\n");
            strcat(outStr, formattedBook);
        }
        
        // next node
        currNode = currNode->next;
    }

    // head
    formatBook(list->head->data, formattedBook);
    strcat(outStr, "\n-- HEAD: ");
    strcat(outStr, formattedBook);

    // tail
    formatBook(list->tail->data, formattedBook);
    strcat(outStr, "\n-- TAIL: ");
    strcat(outStr, formattedBook);
}

/*
  Function: cleanupList
  Purpose:  deallocates all the memory that was dynamically allocated for the book collection
       in:  the book collection
   return:  nothing
*/
void cleanupList(BookListType *list)
{
    NodeType *currNode;
    NodeType *nextNode;

    currNode = list->head;
    nextNode = NULL;

    while (currNode != NULL)
    {
        // free inner memory allocated data
        free(currNode->data);

        nextNode = currNode->next;
        free(currNode);
        currNode = nextNode;
    }
}