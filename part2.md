#### Part 2

Implement an LRU cache. This will be used to cache files in RAM so you don't have to load them through the OS.

When a file is requested, the cache should be checked to see if it is there. If so, the file is served from the cache. If not, the file is loaded from disk, served, and saved to the cache.

The cache has a maximum number of entries. If it has more entries than the max, the least-recently used entries are discarded.

The cache consists of a [doubly-linked
list](https://en.wikipedia.org/wiki/Doubly_linked_list) and a
[hash table](https://en.wikipedia.org/wiki/Hash_table).

The hashtable code is already written and can be found in `hashtable.c`.

1. Implement `cache_put()` in `cache.c`.

   Algorithm:

   * Allocate a new cache entry with the passed parameters.
   * Insert the entry at the head of the doubly-linked list.
   * Store the entry in the hashtable as well, indexed by the entry's `path`.
   * Increment the current size of the cache.
   * If the cache size is greater than the max size:
     * Remove the cache entry at the tail of the linked list.
     * Remove that same entry from the hashtable, using the entry's `path` and the `hashtable_delete` function.
     * Free the cache entry.
     * Ensure the size counter for the number of entries in the cache is correct.

2. Implement `cache_get()` in `cache.c`.

   Algorithm:

   * Attempt to find the cache entry pointer by `path` in the hash table.
   * If not found, return `NULL`.
   * Move the cache entry to the head of the doubly-linked list.
   * Return the cache entry pointer.

3. Add caching functionality to `server.c`.

   When a file is requested, first check to see if the path to the file is in
   the cache (use the file path as the key).

   If it's there, serve it back.

   If it's not there:

   * Load the file from disk (see `file.c`)
   * Store it in the cache
   * Serve it

There's a set of unit tests included to ensure that your cache implementation is functioning correctly. From the `src` directory, run `make tests` in order to run the unit tests against your implementation. 