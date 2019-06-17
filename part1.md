#### Part 1

1. Implement `send_response()`.

   This function is responsible for formatting all the pieces that make up an HTTP response into the proper format that clients expect. In other words, it needs to build a complete HTTP response with the given parameters. It should write the response to the string in the `response` variable.
   
   The total length of the header **and** body should be stored in the `response_length` variable so that the `send()` call knows how many bytes to
   send out over the wire.

   See the [HTTP](guides/net.md) section above for an example of an HTTP response and use that to build your own.

   Hint: `sprintf()` for creating the HTTP response. `strlen()` for computing
   content length. `sprintf()` also returns the total number of bytes in the
   result string, which might be helpful. For getting the current time for the Date field of the response, you'll want to look at the `time()` and `localtime()` functions, both of which are already included in the `time.h` header file.

   > The HTTP `Content-Length` header only includes the length of the body, not
   > the header. But the `response_length` variable used by `send()` is the
   > total length of both header and body.

   You can test whether you've gotten `send_response` working by calling the `resp_404` function from somewhere inside the `main` function and passing it the `newfd` socket (make sure you do this _after_ the `newfd` socket has been initialized by the `accept` system call in the while loop). If the client receives the 404 response when you make a request to the server, then that's a pretty clear indication that your `send_response` is working. 


   2. Examine `handle_http_request()` in the file `server.c`.

   You'll want to parse the first line of the HTTP request header to see if this is a `GET` or `POST` request, and to see what the path is. You'll use this information to decide which handler function to call.

   The variable `request` in `handle_http_request()` holds the entire HTTP request once the `recv()` call returns.

   Read the three components from the first line of the HTTP header. Hint: `sscanf()`.

   Right after that, call the appropriate handler based on the request type
   (`GET`, `POST`) and the path (`/d20` or other file path.) You can start by
   just checking for `/d20` and then add arbitrary files later.

   Hint: `strcmp()` for matching the request method and path. Another hint:
   `strcmp()` returns `0` if the strings are the _same_!

   > Note: you can't `switch()` on strings in C since it will compare the string pointer values instead of the string contents. You have to use an
   > `if`-`else` block with `strcmp()` to get the job done.

   If you can't find an appropriate handler, call `resp_404()` instead to give
   them a "404 Not Found" response.

3. Implement the `get_d20()` handler. This will call `send_response()`.

   See above at the beginning of the assignment for what `get_d20()` should pass to `send_response()`.

   If you need a hint as to what the `send_response()` call should look like, check out the usage of it in `resp_404()`, just above there.

   Note that unlike the other responses that send back file contents, the `d20` endpoint will simply compute a random number and send it back. It does not read the number from a file.

   > The `fd` variable that is passed widely around to all the functions holds a _file descriptor_. It's just a number use to represent an open
   > communications path. Usually they point to regular files on disk, but in
   > this case it points to an open _socket_ network connection. All of the code to create and use `fd` has been written already, but we still need to pass it around to the points it is used.

4. Implement arbitrary file serving.

   Any other URL should map to the `serverroot` directory and files that lie within. For example:

   `http://localhost:3490/index.html` serves file `./serverroot/index.html`.

   `http://localhost:3490/foo/bar/baz.html` serves file
   `./serverroot/foo/bar/baz.html`.

   You might make use of the functionality in `file.c` to make this happen.

   You also need to set the `Content-Type` header depending on what data is in
   the file. `mime.c` has useful functionality for this.