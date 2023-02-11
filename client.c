#include "defs.h"


int main()
{
  int  choice = -1;
  int clientSocket; 
  setupClientSocket(&clientSocket);

  char request[MAX_BUFF];
  char response[MAX_BUFF];
  int id;
  char str[MAX_STR];

  while (1) {

   clientMenu(&choice);

   switch(choice) {

      case 1:   // Print books
        sprintf(request, "%d", REQ_RET_BOOKS);
        sendData(clientSocket, request);
        rcvData(clientSocket, response);
        printf("%s\n", response);
        break;

      case 2:   // Check out a book
        // read book id
        printf("Enter book id: ");
        fgets(str, sizeof(str), stdin);
        sscanf(str, "%d", &id);
        sprintf(request, "%d %d", REQ_CHECK_OUT, id);

        // send request
        sendData(clientSocket, request);

        // receive response
        rcvData(clientSocket, response);

        // print response status
        printf("%s\n", response);
        break;

      case 3:   // Check in a book
        // read book id
        printf("Enter book id: ");
        fgets(str, sizeof(str), stdin);
        sscanf(str, "%d", &id);
        sprintf(request, "%d %d", REQ_CHECK_IN, id);

        // send request
        sendData(clientSocket, request);

        // receive response
        rcvData(clientSocket, response);

        // print response status
        printf("%s\n", response);
        break;

      case 0:   // Close everything
        sprintf(request, "%d", REQ_CLOSE);
        sendData(clientSocket, request);
        close(clientSocket);
        return 0;
        break;

      default:
        printf("ERROR:  invalid option\n");
    }
  }

  return 0;
}

void clientMenu(int* choice)
{
  int c = -1;
  int numOptions = 3;
  char str[MAX_STR];

  printf("\nMAIN MENU\n");
  printf("  (1) Print books\n");
  printf("  (2) Check out a book\n");
  printf("  (3) Check in a book\n");
  printf("  (0) Exit\n\n");

  printf("Please enter your selection: ");
  fgets(str, sizeof(str), stdin);
  str[strcspn(str, "\n")] = 0;
  sscanf(str, "%d", &c);

  if (c == 0) {
    *choice = c;
    return;
  }

  while (c < 0 || c > numOptions) {
    printf("Please enter your selection: ");
    fgets(str, sizeof(str), stdin);
    sscanf(str, "%d", &c);
  }

  *choice = c;
}

