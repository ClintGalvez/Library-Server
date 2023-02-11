#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "defs.h"

/*
  Function: loadBooks
  Purpose:  reads in all the data from the provided booksData.txt file, creates and 
            initializes a BookType structure for each book in the data file, and 
            adds each book to the given book collection
      out:  the book collection
   return:  nothing
*/
void loadBooks(BookListType *books)
{
    FILE *infile;
    int id = 1001;
    char str[MAX_STR];
    char title[MAX_STR];
    char author[MAX_STR];
    int year;
    BookStatusType status;

    infile = fopen("booksData.txt", "r");
    if (!infile)
    {
        printf("Error: could not open file\n");
        exit(1);
    }

    while (1)
    {   
        // get book data
        fgets(title, MAX_STR, infile);      // title
        title[strcspn(title,"\n")] = 0;     // |
        fgets(author, MAX_STR, infile);     // author
        author[strcspn(author,"\n")] = 0;   // |
        fgets(str, MAX_STR, infile);        // year
        sscanf(str, "%d", &year);           // |
        fgets(str, MAX_STR, infile);        // status
        sscanf(str, "%d", &status);         // |

        if (feof(infile))
        {
            break;
        }

        // create, initialize, and add book
        BookType *book;
        initBook(id++, title, author, year, status, &book);
        addBook(books, book);
    }

    fclose(infile);

    // print log of loaded books
    char log[MAX_BUFF];
    formatBooks(books, log);
    printf("BOOKS:\n%s\n\n", log);
}

/*
  Function: initLibrary
  Purpose:  initializes the required fields of the given library
   in-out:  the allocated and initialized library
       in:  the name
   return:  nothing
*/
void initLibrary(LibraryType **library, char *n)
{
    *library = malloc(sizeof(LibraryType));

    strcpy((*library)->name, n);
    initList(&(*library)->books);
    loadBooks(&(*library)->books);
}

/*
  Function: checkOutBook
  Purpose:  checks the book out of the given library
   in-out:  the library
       in:  the book id
   return:  C_OK for success, C_NOK for failure
*/
int checkOutBook(LibraryType *lib, int bookId)
{
    BookType *book;

    // find book
    if (findBook(&lib->books, bookId, &book) == C_NOK)
    {
        return C_BOOK_NOT_FOUND;
    }

    // verify if book is currently checked in
    if (book->status != CHECKED_IN)
    {
        return C_BAD_STATUS;
    }

    // update book status
    book->status = CHECKED_OUT;

    return C_OK;
}

/*
  Function: checkInBook
  Purpose:  checks the book into the given library
   in-out:  the library
       in:  the book id
   return:  C_OK for success, C_NOK for failure
*/
int checkInBook(LibraryType *lib, int bookId)
{
    BookType *book;

    // find book
    if (findBook(&lib->books, bookId, &book) == C_NOK)
    {
        return C_BOOK_NOT_FOUND;
    }

    // verify if book is currently checked out
    if (book->status != CHECKED_OUT)
    {
        return C_BAD_STATUS;
    }

    // update book status
    book->status = CHECKED_IN;

    return C_OK;
}

/*
  Function: cleanupLibrary
  Purpose:  deallocates all the memory that was dynamically allocated for the library
       in:  the library
   return:  nothing
*/
void cleanupLibrary(LibraryType *lib)
{
    cleanupList(&lib->books);

    free(lib);
}