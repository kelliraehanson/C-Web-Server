/**
 * webserver.c -- A webserver written in C
 * 
 * Test with curl (if you don't have it, install it):
 * 
 *    curl -D - http://localhost:3490/
 *    curl -D - http://localhost:3490/d20
 *    curl -D - http://localhost:3490/date
 * 
 * You can also test the above URLs in your browser! They should work!
 * 
 * Posting Data:
 * 
 *    curl -D - -X POST -H 'Content-Type: text/plain' -d 'Hello, sample data!' http://localhost:3490/save
 * 
 * (Posting data is harder to test from a browser.)
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <time.h>
#include <sys/file.h>
#include <fcntl.h>
#include "net.h"
#include "file.h"
#include "mime.h"
#include "cache.h"

#define PORT "3490"  // the port users will be connecting to

#define SERVER_FILES "./serverfiles"
#define SERVER_ROOT "./serverroot"

/**
 * Send an HTTP response
 *
 * header:       "HTTP/1.1 404 NOT FOUND" or "HTTP/1.1 200 OK", etc.
 * content_type: "text/plain", etc.
 * body:         the data to send.
 * 
 * Return the value from the send() function.
 */
int send_response(int fd, char *header, char *content_type, void *body, int content_length)
{
    const int max_response_size = 262144;
    char response[max_response_size];

    // Build HTTP response and store it in response

    int response_length = sprintf(response, 
        "%s\n" // Header
        "Content-Type: %s\n" // content_type
        "Content-Length: %d\n" // content_length
        "Connection: close\n" // Important
        "\n", // Meaning this is the end of the header
        // The header is a bunch of fields followed by a blank line and then followed by the body.
        // "%s", // The body. Doesn't work with binary data (things with 0s in it).
        header, content_type, content_length); // parameters. Make sure these are in the order they are above.

    // Send it all!
    // Send the header
    int rv = send(fd, response, response_length, 0);

    if (rv < 0) {
        perror("send");
    }
    // Send the body
    rv = send(fd, body, content_length, 0);

    if (rv < 0) {
        perror("send");
    }

    return rv;
}


// Works like this also:
// int send_response(int fd, char *header, char *content_type, void *body, int content_length)
// {
//     const int max_response_size = 262144;
//     char response[max_response_size];

//     // Build HTTP response and store it in response

//     int response_length = sprintf(response, 
//         "%s\n" // Header
//         "Content-Type: %s\n" // content_type
//         "Content-Length: %d\n" // content_length
//         "Connection: close\n" // Important
//         "\n", // Meaning this is the end of the header
//         // The header is a bunch of fields followed by a blank line and then followed by the body.
//         // "%s", // The body. Doesn't work with binary data (things with 0s in it).
//         header, content_type, content_length, body); // parameters. Make sure these are in the order they are above.

//     memcpy(response + response_length, body, content_length);
//     response_length += content_length;

//     // int response_length = strlen(response); // could end in this and delete int response_length in beginning.

//     ///////////////////
//     // IMPLEMENT ME! //
//     ///////////////////

//     // Send it all!
//     int rv = send(fd, response, response_length, 0);

//     if (rv < 0) {
//         perror("send");
//     }

//     return rv;
// }

/**
 * Send a /d20 endpoint response
 */
void get_d20(int fd)
{
    // Generate a random number between 1 and 20 inclusive
    int x = rand() % 20 + 1; // Mod this with 20 and it will give you results in the range 0-19. Adding 1 goes to 20.

    char s[1024]; // A string that is big enough.

    int resp_len = sprintf(s, "%d\n", x);
    
    ///////////////////
    // IMPLEMENT ME! //
    ///////////////////

    // Use send_response() to send it back as text/plain data
    send_response(fd, "HTTP/1.1 200 OK", "text/plain", s, resp_len); // send to s(string) in resp_len.
    
    // send_response(fd, "HTTP/1.1 404 NOT FOUND", mime_type, filedata->data, filedata->size);
    // send_response(fd, "HTTP/1.1 200 OK", "text/plain", "hi", 2); // hi is 2 bytes long.

    ///////////////////
    // IMPLEMENT ME! //
    ///////////////////
}

/**
 * Send a 404 response
 */
void resp_404(int fd)
{
    char filepath[4096];
    struct file_data *filedata; 
    char *mime_type;

    // Fetch the 404.html file
    snprintf(filepath, sizeof filepath, "%s/404.html", SERVER_FILES);
    filedata = file_load(filepath);

    if (filedata == NULL) {
        // TODO: make this non-fatal
        fprintf(stderr, "cannot find system 404 file\n");
        exit(3);
    }

    mime_type = mime_type_get(filepath);

    send_response(fd, "HTTP/1.1 404 NOT FOUND", mime_type, filedata->data, filedata->size);

    file_free(filedata);
}

/**
 * Get cat
 */
void get_cat(int fd)
{
    char filepath[4096];
    struct file_data *filedata; 
    char *mime_type;

    // Fetch the cat.jpg file
    snprintf(filepath, sizeof filepath, "%s/cat.jpg", SERVER_ROOT);
    filedata = file_load(filepath);

    if (filedata == NULL) {
        // TODO: make this non-fatal
        fprintf(stderr, "cannot find cat\n");
        exit(3);
    }

    mime_type = mime_type_get(filepath);

    send_response(fd, "HTTP/1.1 200 OK", mime_type, filedata->data, filedata->size);

    file_free(filedata);
}

/**
 * Read and return a file from disk or cache
 */
void get_file(int fd, struct cache *cache, char *request_path)
{
    (void) cache;

    char file_path[4096];
    struct file_data *filedata;
    char *mime_type;

    // Fetch the file
    snprintf(file_path, sizeof file_path, "%s%s", SERVER_ROOT, request_path);
    filedata = file_load(file_path);

    if (filedata == NULL) {
        resp_404(fd);
        return;
    }

    mime_type = mime_type_get(file_path);

    send_response(fd, "HTTP/1.1 200 OK", mime_type, filedata->data, filedata->size);

    file_free(filedata);
}

// void get_file(int fd, struct cache *cache, char *request_path)
// {
//     ///////////////////
//     // IMPLEMENT ME! //
//     ///////////////////
//     struct file_data *filedata;
//     char *mime_type;
//     char file_path[4096];

//     // printf(request_path);
//     // printf("\n %s%s \n", SERVER_ROOT, request_path);

//         sprintf(file_path, "./serverroot%s", request_path); // File path to the ./serverroot directory
        
//         filedata = file_load(file_path);

//         if (filedata == NULL)
//         {
//             sprintf(file_path, "./serverroot%s/index.html", request_path); // The input '/' goes to the index.html file.
//             filedata = file_load(file_path);
//             if (filedata == NULL)
//             {
//                 resp_404(fd);
//             }
//         }

//         mime_type = mime_type_get(file_path);

//         send_response(fd, "HTTP/1.1 200 OK", mime_type, filedata->data, filedata->size);

//         file_free(filedata);
// }

/**
 * Search for the end of the HTTP header
 * 
 * "Newlines" in HTTP can be \r\n (carriage return followed by newline) or \n
 * (newline) or \r (carriage return).
 */
char *find_start_of_body(char *header)
{
    ///////////////////
    // IMPLEMENT ME! // (Stretch)
    ///////////////////
}

/**
 * Handle HTTP request and send response
 */
void handle_http_request(int fd, struct cache *cache)
{
    const int request_buffer_size = 65536; // 64K
    char request[request_buffer_size];
    char method[200]; // GET, POST, etc.
    char path[8192]; // URL path info, for /d20

    // Read request
    int bytes_recvd = recv(fd, request, request_buffer_size - 1, 0);

    if (bytes_recvd < 0) {
        perror("recv");
        return;
    }

    ///////////////////
    // IMPLEMENT ME! //
    ///////////////////
    // Read the first two components of the first line of the request 
    sscanf(request, "%s %s", method, path);

    printf("\nHTTP Request: \n - Type: %s \n - Path: %s \n", method, path);

    // resp_404(fd); // 404 Message from lecture
 
    // If GET, handle the get endpoints
    if (strcmp(method, "GET") == 0) {
        // Check if it's /d20 and handle that special case
        // Otherwise serve the requested file by calling get_file()
        if (strcmp(path, "/d20") == 0) {
            get_d20(fd);

        } else if (strcmp(path, "/cat") == 0) {
                get_cat(fd);

        } else {
            get_file(fd, NULL, path); 
            // resp_404(fd); // If you can't find the GET handler, call `resp_404()` instead to give them a "404 Not Found" response.
        }
    } else {
       printf("Unhandled method: %s\n", method);
    }

    // (Stretch) If POST, handle the post request
}

/**
 * Main
 */
int main(void)
{
    int newfd;  // listen on sock_fd, new connection on newfd
    struct sockaddr_storage their_addr; // connector's address information
    char s[INET6_ADDRSTRLEN];

    srand(time(NULL));

    struct cache *cache = cache_create(10, 0);

    // Get a listening socket
    int listenfd = get_listener_socket(PORT);

    if (listenfd < 0) {
        fprintf(stderr, "webserver: fatal error getting listening socket\n");
        exit(1);
    }

    printf("webserver: waiting for connections on port %s...\n", PORT);

    // This is the main loop that accepts incoming connections and
    // responds to the request. The main parent process
    // then goes back to waiting for new connections.
    
    while(1) {
        socklen_t sin_size = sizeof their_addr;

        // Parent process will block on the accept() call until someone
        // makes a new connection:
        newfd = accept(listenfd, (struct sockaddr *)&their_addr, &sin_size);
        if (newfd == -1) {
            perror("accept");
            continue;
        }
        // resp_404(newfd); // 404 Message

        // Print out a message that we got the connection
        inet_ntop(their_addr.ss_family,
            get_in_addr((struct sockaddr *)&their_addr),
            s, sizeof s);
        printf("server: got connection from %s\n", s);
        
        // newfd is a new socket descriptor for the new connection.
        // listenfd is still listening for new connections.

        handle_http_request(newfd, cache);

        close(newfd);
    }

    // Unreachable code

    return 0;
}