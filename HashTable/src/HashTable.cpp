//============================================================================
// Name        : HashTable.cpp
// Author      : Josh Gauthier
// Version     : 1.0
// Copyright   : Copyright © 2017 SNHU COCE
// Description : Hash table
//============================================================================

#include <algorithm>
#include <climits>
#include <iostream>
#include <string> // atoi
#include <time.h>
#include <vector>

#include "CSVparser.hpp"

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

const unsigned int DEFAULT_SIZE = 179;

// forward declarations
double strToDouble(string str, char ch);

// define a structure to hold bid information
struct Bid {
    string bidId; // unique identifier
    string title;
    string fund;
    double amount;
    Bid() {
        amount = 0.0;
    }
};

void displayBid(Bid bid);

//============================================================================
// Hash Table class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a hash table with chaining.
 */
class HashTable {

private:
    struct Node {
       Bid bid;
       unsigned int key;
       Node* next;

       // Default constructor
       Node() {
          key = UINT_MAX;        // Set default key to highest possible int value
          next = nullptr;
       }

       // Constructor that takes a bid as parameter
       // Calls default constructor first before executing body
       Node(Bid bidToSet) : Node() {
          bid = bidToSet;
       }

       // Constructor that takes bid and key as parameter
       // Calls default constructor and constructor(bid) before executing body
       Node(Bid abidToSet, unsigned int keyToSet) : Node(abidToSet) {
          key = keyToSet;
       }
    };

    vector<Node> nodeVector;

    unsigned int tableSize = DEFAULT_SIZE;

    unsigned int hash(int key);

public:
    HashTable();
    HashTable(unsigned int size);
    virtual ~HashTable();
    void Insert(Bid bid);
    void PrintAll();
    void Remove(string bidId);
    Bid Search(string bidId);
};

/**
 * Default constructor
 */
HashTable::HashTable() {
   nodeVector.resize(tableSize);
}

// Constructor with vector size as parameter
HashTable::HashTable(unsigned int size) {
   this->tableSize = size;
   nodeVector.resize(tableSize);
}


/**
 * Destructor
 */
HashTable::~HashTable() {
   nodeVector.erase(nodeVector.begin());
}

/**
 * Calculate the hash value of a given key.
 * Note that key is specifically defined as
 * unsigned int to prevent undefined results
 * of a negative list index.
 *
 * @param key The key to hash
 * @return The calculated hash
 */
unsigned int HashTable::hash(int key) {
   return key % tableSize;
}

/**
 * Insert a bid
 *
 * @param bid The bid to insert
 */
void HashTable::Insert(Bid bid) {
   // Use bidId to calculate hash key
   // c_str() converts C++ string object to old fashioned c string
   // atoi() converts the ASCII c string into integer
   // Finally, call hash function to calculate key with modulus operation
   unsigned int key = hash(atoi(bid.bidId.c_str()));

   Node* oldNode = &(nodeVector.at(key));

   // If the bucket was empty (IOW, if there was no nodes already at the key location
   if (oldNode == nullptr) {
      // Create new node with constructor, initializing it with the bid and key data
      Node* newNode = new Node(bid, key);

      // Use built in vector method "insert" to insert new bid into the right index/bucket
      nodeVector.insert(nodeVector.begin() + key, (*newNode));

      delete newNode;
   }

   // If there is a node already in this bucket
   else {

      // If the found node is empty
      if (oldNode->key == UINT_MAX) {
         // Update this blank node to new node
         oldNode->bid = bid;
         oldNode->key = key;
         oldNode->next = nullptr;
      }
      // If the found node is not empty
      else {
         // Find the last node in the list
         while (oldNode->next != nullptr) {
            oldNode = oldNode->next;
         }
         Node* newNode = new Node(bid, key);
         // Add new node to end of the list
         oldNode->next = newNode;
         // FIXME deallocate memory for newNode?
      }
   }
}

/**
 * Print all bids
 */
void HashTable::PrintAll() {
   Node* currNode;

   // Traverse vector
   for (unsigned int i = 0; i < tableSize; i++) {
      currNode = &(nodeVector.at(i));

      // If there is a node at this bucket and it's not blank
      if (currNode != nullptr && currNode->key != UINT_MAX) {

         cout << "Key " << i << ": ";
         displayBid(currNode->bid);

         // Traverse all nodes at this bucket, displaying each bid
         while (currNode->next != nullptr) {
            currNode = currNode->next;
            cout << "    " << i << ": ";
            displayBid(currNode->bid);
         }
      }
   }
}

/**
 * Remove a bid
 *
 * @param bidId The bid id to search for
 */
void HashTable::Remove(string bidId) {

   Bid emptyBid;
   Node* currNode = nullptr;
   Node* prevNode = nullptr;

   // Calculate key from given bidId
   unsigned int key = hash(atoi(bidId.c_str()));

   // Search for node with that key (IOW, find the bucket and see if it is populated)
   currNode = &(nodeVector.at(key));

   // If the bucket is not null or empty and the node matches bidId
   if (currNode != nullptr && currNode->key != UINT_MAX && currNode->bid.bidId.compare(bidId) == 0) {

      // If the matching node is the only node in bucket
      if (currNode->next == nullptr) {
         // Update node to empty node
         currNode->bid = emptyBid;
         currNode->key = UINT_MAX;
         currNode->next = nullptr;
      }
      // If there is another node after match, make it the first node in the bucket
      else {
         nodeVector.at(currNode->key) = *(currNode->next);
      }
   }

   // If the bucket contains a non-empty bid but the bidId doesn't match
   else if (currNode != nullptr && currNode->key != UINT_MAX) {

      // Walk the linked list till end or till we find matching bidId
      while(currNode->next != nullptr) {
         prevNode = currNode;
         currNode = currNode->next;

         // If this node matches bidId and there is no next node, point prevous node to nothing
         if (currNode->bid.bidId == bidId && currNode->next == nullptr) {
            prevNode->next = nullptr;
            break;
         }
         // If current node matches bidId and there are more nodes after it, point previous to current's next node
         else {
            prevNode->next = currNode->next;
            break;
         }
      }
   }
}

/**
 * Search for the specified bidId
 *
 * @param bidId The bid id to search for
 */
Bid HashTable::Search(string bidId) {
    Bid emptyBid;

    // Calculate key from given bidId
    unsigned int key = hash(atoi(bidId.c_str()));

    // Search for the node with matching key (IOW, search the bucket for a node)
    Node* searchedNode = &(nodeVector.at(key));

    // If bucket is empty or only has a blank node
    if (searchedNode == nullptr || searchedNode->key == UINT_MAX) {
       return emptyBid;
    }

    // If node found in that bucket, it is not an empty node, and the node has matching bidId [compare() returns 0 if the strings match]
    if (searchedNode != nullptr && searchedNode->key != UINT_MAX && searchedNode->bid.bidId.compare(bidId) == 0) {
       return searchedNode->bid;
    }

    // If node found in that bucket but doesn't have matching bidId, walk the linked list to find match
    while (searchedNode != nullptr) {

       // If current node matches bidId, return it
       if (searchedNode->key != UINT_MAX && searchedNode->bid.bidId.compare(bidId) == 0) {
          return searchedNode->bid;
       }
       if (searchedNode->next != nullptr) {
          searchedNode = searchedNode->next;
       }
    }
    return emptyBid;
}

//============================================================================
// Static methods used for testing
//============================================================================

/**
 * Display the bid information to the console (std::out)
 *
 * @param bid struct containing the bid info
 */
void displayBid(Bid bid) {
    cout << bid.bidId << ": " << bid.title << " | " << bid.amount << " | "
            << bid.fund << endl;
    return;
}

/**
 * Load a CSV file containing bids into a container
 *
 * @param csvPath the path to the CSV file to load
 * @return a container holding all the bids read
 */
void loadBids(string csvPath, HashTable* hashTable) {
    cout << "Loading CSV file " << csvPath << endl;

    // initialize the CSV Parser using the given path
    csv::Parser file = csv::Parser(csvPath);

    // read and display header row - optional
    vector<string> header = file.getHeader();
    for (auto const& c : header) {
        cout << c << " | ";
    }
    cout << "" << endl;

    try {
        // loop to read rows of a CSV file
        for (unsigned int i = 0; i < file.rowCount(); i++) {

            // Create a data structure and add to the collection of bids
            Bid bid;
            bid.bidId = file[i][1];
            bid.title = file[i][0];
            bid.fund = file[i][8];
            bid.amount = strToDouble(file[i][4], '$');

            //cout << "Item: " << bid.title << ", Fund: " << bid.fund << ", Amount: " << bid.amount << endl;

            // push this bid to the end
            hashTable->Insert(bid);
        }
    } catch (csv::Error &e) {
        std::cerr << e.what() << std::endl;
    }
}

/**
 * Simple C function to convert a string to a double
 * after stripping out unwanted char
 *
 * credit: http://stackoverflow.com/a/24875936
 *
 * @param ch The character to strip out
 */
double strToDouble(string str, char ch) {
    str.erase(remove(str.begin(), str.end(), ch), str.end());
    return atof(str.c_str());
}

/**
 * The one and only main() method
 */
int main(int argc, char* argv[]) {

    // process command line arguments
    string csvPath, bidKey;
    switch (argc) {
    case 2:
        csvPath = argv[1];
        bidKey = "98109";
        break;
    case 3:
        csvPath = argv[1];
        bidKey = argv[2];
        break;
    default:
        csvPath = "eBid_Monthly_Sales_Dec_2016.csv";
        bidKey = "98109";
    }

    // Define a timer variable
    clock_t ticks;

    // Define a hash table to hold all the bids
    HashTable* bidTable;

    Bid bid;

    int choice = 0;
    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Load Bids" << endl;
        cout << "  2. Display All Bids" << endl;
        cout << "  3. Find Bid" << endl;
        cout << "  4. Remove Bid" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {

        case 1:
            bidTable = new HashTable();

            // Initialize a timer variable before loading bids
            ticks = clock();

            // Complete the method call to load the bids
            loadBids(csvPath, bidTable);

            // Calculate elapsed time and display result
            ticks = clock() - ticks; // current clock ticks minus starting clock ticks
            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;

        case 2:
            bidTable->PrintAll();
            break;

        case 3:
            ticks = clock();

            bid = bidTable->Search(bidKey);

            ticks = clock() - ticks; // current clock ticks minus starting clock ticks

            if (!bid.bidId.empty()) {
                displayBid(bid);
            } else {
                cout << "Bid Id " << bidKey << " not found." << endl;
            }

            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;

        case 4:
           bidTable->Remove(bidKey);
            break;
        }
    }

    cout << "Good bye." << endl;

    return 0;
}
