Notes from TK videos:

Networking Protocols:

What is a protocol?
    - In simple terms it is basically just a contract between two parties. 
    (Like a contract between the client and the server.)
    "If you send me x, I willl send you back y."

Data Encapsulation:
    - Ethernet Header (deals with routing on LAN)
    - IP Header (deals with routing on the internet)
    - TCP Header (deals with data integrity or reliability)
    - HTTP Header (deals with web data)
    - <h1>Hello World!</h1>
    (Basically this data is going to be wrapped in different headers).

Different Networking Protocols:
    - TCP (Transmission Control Protocol)
    - UDP (User Datagram Protocol)

The TCP Protocol:
    - TCP provides reliabiltiy throught the three way handshake.

The UDP Protocol:
    - In contrast to TCP this is like a gushing water hose. No safe guards.
    - It sends out a bunch of extra volume of data packets.

What are Sockets?
    - sockets.io
    - Everything in Unix is a file.

File Descriptors:
    - Integer values associated with an open file.
    FILE *fp = fopen("text.txt", "w"):
    // this would print an integer value
    printf("%d, fp);

Some Caveats:
"File" can refer to any on of the following:
    - A network connection
    - A pipe
    - A terminal
    - An actual "normal" file

Initializing a Socket:
<!-- #include <sys/socket.h> -->
// initialize a socket
int sockfd = socket(. . .)

// send and recieve data using the socket descriptor
send(sockfd, response, responseLength, 0);
recv(sockfd, request, requestBufferSize - 1, 0);

The HTTP Protocol:
    - HTTP deals with the formatting of requests and responses
    - HTTP is not concerned with how data gets from one place to another

Format of a GET Request:
GET /example HTTP/1.1
Host: lambdaschool.com

Format of a 200 Response:
HTTP/1.1 200 OK
Date: Wed Dec 20 13:05:11 PST 2017
Connection: close
Content-Length: 41749
Content-Type: text/html

<!-- <html><head><title>Lambda School . . .  -->

Format of a 4041 Response:
HTTP/1.1 404 Not Found
Date: Wed Dec 20 13:05:11 PST 2017
Connection: close
Content-Length: 41749
Content-Type: text/plain

404 Not Found

Some things to keep in mind:
    - The Content-Length field gives the length of the request or response 
    body, not counting the blank line between the header and the body.
    - The Content-Type field gives the MIME type of the content in the body.
    This tells the browser how to display page, i.d., as
    plain text, HTML, a GIF image, or whatever other type.
    - Even if a request has no body, it must contain a new line after the header.


