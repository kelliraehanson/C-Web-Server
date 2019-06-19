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

Webserver: MIME Types:

History:
    - Multipurpose Internet Mail Extensions: originally used for email.
    - Found more life in the world wide web.
    - Turns out they were very similar problems. 

How the web uses MIME:
    - Web data is just streams of bytes
    - Need some way to differentiate the GIF from HTML data
    - The Content-Type HTTP header specifies what type the bytes are
    Content-Type: text/markdown
    - That header uses MINE types like text/html or image/jpeg

Web Servers and MIME:
    - A requested file's extension is examined to determine the MIME type
    - Then the MIME type of that file is included in the response
    - Example:
    images/goats.jpg
    - Mime type must be image/jpeg

Common MIME types:
Extention       |        MIME Type
.html           |        text/html
.js             |        application/javascript
.css            |        text/css
.jpg            |        image/jpeg
.gif            |        image/gif
.png            |        image/png
.json           |        application/json   
.txt            |        text/plain     
.md             |        text/markdown

How does the browser use MIME?
    - If it's a type of data the browser can display, it does.
    - Otherwise it downloads the data
    - Or launches an external viewer

Programmatic Data:
    - What if you are creating data on the fly, not getting it from a file?
    - Choose the appropriate MIME type for the response data
    - If your response is JSON:
    { "status": "success" }
    Set the Content-Type to application/json

Doubly-Linked Lists:
    - Has next and prev pointers 
Benefits: 
    - Can be traversed both ways. Two linked lists in one!
    - If you have a pointer to a node, that node can be removed in O(1) time.
Drawbacks:
    - A bit more memory per node.
    - Twice as much pointer management when you manipulate the list.

LUR Cache: 

Caches:
    - Keep copies of data from slow sources in a fast, local source.
    - Frequently-used data shows the greatest speedup.

Cache Terminology:
    - Cache Hit: When the data you want is in the cache.
    - Cache Miss: When you have to go to primary storage to get the data.

LRU Caches:
    - Caches are limited in size compared to primary storage.
    - LRU Caches discard the Least-Recently used item in the cache when it is full. 

Building an LUR Cache:
    - Need a hash table to quickly look up cache entries by key
    - Need the cache entries in a doubly-linked list
        - Makes it easy to move them to the head, O(1)
        - Makes it easy to remove them from the tail, O(1)

LRU Cache Overview:
    - Hash Table gives quick access to all entries in the cache
    - Doubly-Linked List is ordered from Most-Recently Used to Least-Recently Used

Putting Entires in the Cache:
    - Move to the head of the list
    - Add hash table entry 

Getting entries from the Cache:
    - Move the accessed element to the head of the list (MRU)
    - Then return a pointer to that entry

Deleting LRU from the Cache:
    - If the cahce is overfull, simply delete the tail entry
    - This discards the least-recently used entry 