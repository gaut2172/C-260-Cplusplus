# Data Structures
### Overview

  Data structures are important to understand in order to be an efficient problem solver in IT. The project I completed called HashTable exemplifies my knowledge in C++ vectors. That same project also shows my skills in hash table structures. The project called BinarySearchTree best illustrates my knowledge in tree structures.

### Vector Data Structures
  A vector is a versatile data structure that can be used in conjunction with other data structures and algorithms to efficiently solve different problems. For instance, in VectorSorting I used vectors in conjunction with the quicksort algorithm which proved to be very efficient at sorting by bid title. This could’ve been done with an array, but vectors allow convenient size change and arrays do not. In my project called HashTable I combined vectors with linked lists and a hashing algorithm to create an extremely fast hash table. A vector is generally very useful in this instance because it maintains list order and allows instant access based on index. We were presented with a large CSV file that held bid data and we needed to be able to search using a unique identifier. My program HashTable is extremely efficient at solving this problem, which is arguably because of the vector structure. A hash algorithm is only as fast as the structure that accesses the element, and a vector allows instant access using the unique key as an index. The program uses a vector as the bucket list, using each index as a hash table bucket. Each index of the vector can contain an empty node, one populated node, or a linked list of nodes. The vector data structure organizes the data so that the hash algorithm can be applied. The hash table is fast because the vector has all elements in contiguous memory locations, allowing instant access.

### Hash Table Data Structures
  Hash tables are widely used in programming because they can allow for very fast searches. If planned properly, searching a hash table for a given element could take as little as O(1). In VectorSorting I used a hash table to solve the stated problem of searching by unique identifier which gave faster searches than with a vector alone. Though a vector is useful, creating a hash table by combining it with other structures and algorithms allows for the implementation of lightning fast searches of any data size. 

### Tree Data Structures
  Tree structures such as a binary search tree can also be useful data structures. Unlike hash tables, tree structures maintain original order. Combining search algorithms with binary trees can be very powerful, allowing for O(log(N)) speeds. We were presented another problem with the original large CSV file of bids—we needed an efficient search algorithm that keeps sorted order and has near-instant searching no matter how many bids there are. My project BinarySearchTree uses a tree structure to maintain list order and solves the problem efficiently. The tree structure is similar to a linked list, however each node holds two pointers. One pointer goes left, and the other goes right. All nodes in its left subtree have a key that’s less than the parent, while all nodes in right subtree are equal or more than the parent. This organization allows for order-keeping and the traversal of a much smaller sublist to get to the target.

# Algorithms
### Overview
  Using various known algorithms in conjunction with data structures is also vitally important to avoid wasting time trying to solve a problem that has already been solved. Knowing when and where to use certain algorithms makes for a better programmer. The HashTable program best illustrates my skills with both search and hash/chaining algorithms. VectorSorting best shows my knowledge of effective sorting algorithms and my awareness of inefficient sorting algorithms.

### Search Algorithms
  There are many search algorithms that are useful for specific tasks. However, the problem we were tasked with was to allow for efficient searches of larger data sets with a unique identifier. The HashTable program I wrote is a technically sound choice for this scenario and performs the required functions the most efficiently. Given there is no need for keeping order, a hash table is a better choice than a binary search tree because it has a potential speed of O(1), which is faster than a BST at O(log(N)). The hash algorithm I chose has minimal collisions for the data set, making the overall speed closer to O(1) than its worst case O(N).

### Sorting Algorithms
  There are numerous sort algorithms, but many are inefficient. For example, the bubblesort and selectionsort algorithms are simple to write but very slow. The problem we were given was to sort the CSV file of bids by bid amount from highest to lowest and by department and fund alphabetically. My program VectorSorting uses two algorithms to accomplish this—selectionsort and quicksort. The results for sorting 18,000 bids was 2.6 seconds with selectionsort, and 0.016 seconds for quicksort. These results illustrate why quicksort is so widely used in programming. Essentially, quicksort algorithm is faster because it has exponentially less comparisons to make than selectionsort.

### Collision Handling in Hash Algorithms
  Collision handling is necessary when hashing to account for identical keys. Chaining handles hash table collisions by using a list for each bucket. My HashTable program utilizes a modulo hash function along with a linked list chaining technique to handle collisions. This is an extremely efficient algorithm and structure combination because the search runtime is 0 clock ticks. It is worth mentioning there is only a few collisions in this hashtable where there are buckets that have at most two or three nodes. If this program was not efficient then there would be many more collisions, and as a result the runtime would be much slower for this large data set.
