#include <iostream>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>

// Function that takes in a message strips everything
// Before the '/' and after the filename
char* parse_http(char* message) {

  //initialize variables to use
  char* token;

  // Strips everything to the '/' character out of the string
  token = strtok(message, "/");

  // Strips either to the new line character '\n' or a space " "
  // Meaning you can type things after the filename and it will
  // Extract the filename as expected
  token = strtok(NULL, " \r\n\r\n");

  // Returns the filename with everything stripped
  return token;
}

int main() {

  // Intialize all variables that C++ needs
  int connectionSocket, message;
  int portNum = 8015;
  int bufsize = 1024;

  // Prebuilt struct / types for socket programming
  // In C/C++
  struct sockaddr_in server_addr;
  socklen_t size;

  // HTTP Response initializers
  const char* not_found_message;
  const char* not_found_html;
  const char* htttp_ok;

  // Assign Messages to relay back on error
  not_found_message = "HTTP/1.1 404 Not Found\r\n\r\n";
  not_found_html = "<html><head></head><body><h1>404 Not Found</h1></body></html>\r\n";
  htttp_ok = "HTTP/1.1 200 OK\r\n\r\n";

  // Establish the intial socket for communication
  // socket(domain, type, IP)
  // Domain = AF_INET makes it a telnet socket
  // type = SOCK_STREAM enables TCP to be used
  // IP = 0 Always like this
  connectionSocket = socket(AF_INET, SOCK_STREAM, 0);

  // Assigns telnet to the predefined C struct
  // Which enables telnet connection
  server_addr.sin_family = AF_INET;

  // Allows the server to accept any connections i.e. localhost
  server_addr.sin_addr.s_addr = htons(INADDR_ANY);

  // Assigns the port number to use for said socket specified with
  // Variable 'portNum'
  server_addr.sin_port = htons(portNum);

  // Binds the socket 'connectionSocket' to the right portNum
  // And address using the predefined struct C 'sockaddr_in'
  bind(connectionSocket, (struct sockaddr*)&server_addr, sizeof(server_addr));

  // Takes the size of server_addr and makes a variable with that size
  size = sizeof(server_addr);

  // Listens on a predefined socket and only accepts one connection at a time
  listen(connectionSocket, 1);

  // Makes an infinite loop to make the server listen either indefinitely
  // Or until a 404 Error occurs
  while(true) {

    // Initialize all characters after every iteration of the
    // Loop is executed
    char buffer[bufsize];
    char data;
    char* filename;
    char* message;
    FILE* file;
    data = 0;

    // Relays to the terminal the server is ready to go
    printf("Server is ready to connect\n");

    // Accept connections on the socket established previously
    int socket = accept(connectionSocket, (struct sockaddr *)&server_addr, &size);

    // Tries to send and throws an exceptions if the filename is NULL
    try {

      // Recieves a message of size 'buffsize', stores the message in 'buffer',
      // Over the socket 'socket'
      read(socket, buffer, bufsize);

      // Displays message from client to console
      printf("Message: %s\n", buffer);

      // Calls the function to strip everything but the
      // Actual filename of the message
      filename = parse_http(buffer);

      // Takes the filename and opens the file for parsing
      file = fopen(filename, "r");

      // Displays the filename to console
      printf("Filename: %s\n", filename);

      // If the filename is NULL, throws an exception
      if(file == NULL) {

         throw file;

      }

      // Sends an HTTP OK message to the client because the
      // File was succesful in opening
      send(socket, htttp_ok, strlen(htttp_ok), 0);


      // While the file open still has data parses
      // Through it one character at a time and sends
      // It over the socket
      while(data != EOF) {

        if(data != 0) {

          // Sends the value of 'data' over the socket 'socket'
          // Of maxmimum size of 1 byte
          send(socket, &data, 1, 0);

          // Print to console each character sent over the socket
          printf("Sent: %c\n", data);

          // Iterates to the next character in the file 'file'
          data = fgetc(file);

        }

        else {
          // If data equals '1' gets the first
          // Character of the open file 'file'
          data = fgetc(file);
        }

      }

      // Closes the file to rewind the FILE pointer, to the start
      // Of the file
      fclose(file);

      // Sends the filename 'filename' over the socket
      // With maxmimum size filename length
      send(connectionSocket, filename, strlen(filename), 0);

      // Relays that the data was sent succesfully
      printf("Data/File Sent\n");

      // Close the socket to be able to reuse the same port
      close(socket);

    }

    // Exception for NULL file
    catch(FILE* file) {

      // Displays the incorrect filename to console
      printf("File '%s' not found\n", filename);

      // Sends HTTP 404 Error over socket
      send(socket, not_found_message, strlen(not_found_message), 0);

      // Sends HTTP 404 HTML code to be displayed in a internet browser
      send(socket, not_found_html, strlen(not_found_html), 0);

      // Closes the socket for port reuse
      close(socket);

      // Sytem exit call with a '1'
      exit(1);
    }
  }
    // Will never execute, satisfying int() function type
    return 0;

}
