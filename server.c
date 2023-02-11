#include "defs.h"

int listenSocket;

int main()
{
  LibraryType *library;
  int clientSocket;

  char libraryName[MAX_STR] = "Cerulean Library";
  initLibrary(&library, libraryName);
  
  setupServerSocket(&listenSocket);

  signal(SIGUSR1, handleSig1);

  printf("Waiting for connection request...\n");
  acceptConnection(listenSocket, &clientSocket);
  printf("... connected\n");
  serveOneClient(clientSocket, library);
  
  return 0;
}


/*
  Function: serveOneClient
  Purpose:  serves all the requests from the client process connected at 
            the given client socket, using the given library data
       in:  the 
   in-out:  the 
   return:  nothing
*/
void serveOneClient(int clientSocket, LibraryType *lib)
{
  char request[MAX_BUFF];
  char response[MAX_BUFF];
  int id;
  int status;
  int placeHolder;

  while (1)
  {
    printf("waiting for message...\n");

    // receive request
    rcvData(clientSocket, request);
    printf("... received\n");

    // get request type
    RequestType type = request[0] - '0';

    // send response base on request type
    switch(type)
    {
      case REQ_RET_BOOKS:
        formatBooks(&lib->books, response);
        sendData(clientSocket, response);
        break;
        
      case REQ_CHECK_OUT:
        sscanf(request, "%d %d", &placeHolder, &id);
        status = checkOutBook(lib, id);
        switch (status)
        {
          case C_BOOK_NOT_FOUND:
            sprintf(response, "ERROR:\tBook id %d was not found", id);
            break;
          case C_BAD_STATUS:
            sprintf(response, "ERROR:\tBook id %d is not checked in", id);
            break;
          default:  // SUCCESSFULLY CHECKED OUT BOOK (IE. C_OK)
            sprintf(response, "Book id %d was successfully checked out", id);
            break;
        }
        sendData(clientSocket, response);
        break;

      case REQ_CHECK_IN:
        sscanf(request, "%d %d", &placeHolder, &id);
        status = checkInBook(lib, id);
        switch (status)
        {
          case C_BOOK_NOT_FOUND:
            sprintf(response, "ERROR:\tBook id %d was not found", id);
            break;
          case C_BAD_STATUS:
            sprintf(response, "ERROR:\tBook id %d is not checked out", id);
            break;
          default:  // SUCCESSFULLY CHECKED IN BOOK (IE. C_OK)
            sprintf(response, "Book id %d was successfully checked in", id);
            break;
        }
        sendData(clientSocket, response);
        break;

      case REQ_CLOSE:
        printf("Shutting down...\n");
        close(clientSocket);
        closeAll(lib);
        printf("... done\n\n");
        exit(0);
        break;

      default:
        // Nothing, invalid request type
        break;
    }
  }
}

/*
  Function: closeAll
  Purpose:  cleans up all the server resources, in preparation for the process terminating
       in:  the library
   return:  nothing
*/
void closeAll(LibraryType *lib)
{
  close(listenSocket);
  cleanupLibrary(lib);
}

/*
  Function: handleSig1
  Purpose:  cleans up the server resources as much as possible
       in:  the listening socket
   return:  nothing
*/
void handleSig1(int i)
{
  close(listenSocket);
  exit(0);
}