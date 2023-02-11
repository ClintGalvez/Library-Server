#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 60002

#define C_OK                 0
#define C_NOK               -1
#define C_BOOK_NOT_FOUND    -2
#define C_BAD_STATUS        -3

#define MAX_BUFF   2048
#define MAX_ARR     128
#define MAX_STR      64

typedef enum {CHECKED_IN, CHECKED_OUT, UNDER_REPAIR, LOST
} BookStatusType;

typedef enum {REQ_RET_BOOKS, REQ_CHECK_IN, 
              REQ_CHECK_OUT, REQ_CLOSE
} RequestType;

typedef struct {
  int id;
  char title[MAX_STR];
  char author[MAX_STR];
  int  year;
  BookStatusType status;
} BookType;

typedef struct Node {
  BookType    *data;
  struct Node *next;
} NodeType;

typedef struct {
  NodeType *head;
  NodeType *tail;
} BookListType;

typedef struct {
  char            name[MAX_STR];
  BookListType    books;
} LibraryType;



void serveOneClient(int clientSocket, LibraryType *lib);
void closeAll(LibraryType *lib);
void handleSig1(int);

void clientMenu(int*);

void initList(BookListType *list);
void initBook(int i, char *t, char *a, int y, BookStatusType st, BookType **book);
void addBook(BookListType *list, BookType *b);
int findBook(BookListType *list, int id, BookType **b);
void formatBook(BookType *b, char *outStr);
void convertStatus(BookStatusType status, char *string);
void formatBooks(BookListType *list, char *outStr);
void cleanupList(BookListType *list);

void loadBooks(BookListType *books);
void initLibrary(LibraryType **library, char *n);
int checkOutBook(LibraryType *lib, int bookId);
int checkInBook(LibraryType *lib, int bookId);
void cleanupLibrary(LibraryType *lib);

void setupServerSocket(int*);
void acceptConnection(int, int*);
void setupClientSocket(int*);
void rcvData(int, char*);
void sendData(int, char*);