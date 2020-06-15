//============================================================================
// Name        : BinarySearchTree.cpp
// Author      : Josh Gauthier and SNHU
// Version     : 1.0
// Copyright   : Copyright © 2017 SNHU COCE
// Description : Binary Search Tree
//============================================================================

#include <iostream>
#include <time.h>
#include <algorithm>


#include "CSVparser.hpp"

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

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

// Structure for binary search tree nodes
struct Node {
   Bid bid;
   Node* left;
   Node* right;

   // Default constructor
   Node() {
      left = nullptr;
      right= nullptr;
   }

   // Initialize with a bid
   Node(Bid bidToAdd) : Node() {
      bid = bidToAdd;
   }
};

//============================================================================
// Binary Search Tree class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a binary search tree
 */
class BinarySearchTree {

private:
    Node* root;

    void addNode(Node* node, Bid bid);
    void inOrder(Node* node);
    Node* removeNode(Node* node, string bidId);

public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void InOrder();
    void Insert(Bid bid);
    void Remove(string bidId);
    Bid Search(string bidId);
    void DestroyTree(Node* node);
};

/**
 * Default constructor
 */
BinarySearchTree::BinarySearchTree() {
    // initialize housekeeping variables
   root = nullptr;
}

/**
 * Destructor
 */
BinarySearchTree::~BinarySearchTree() {
    // recurse from root deleting every node
   DestroyTree(root);
}

/**
 * Deletes entire binary tree recursively
 */
void BinarySearchTree::DestroyTree(Node* node) {
   if (node) {
      DestroyTree(node->left);
      DestroyTree(node->right);
      delete node;
   }
}

/**
 * Traverse the tree in order
 */
void BinarySearchTree::InOrder() {
   this->inOrder(root);
}
/**
 * Insert a bid
 */
void BinarySearchTree::Insert(Bid bid) {
    // If the binary tree is empty
   if (root == nullptr) {
      root = new Node(bid);
   }
   // If binary tree is not empty
   else {
      this->addNode(root, bid);
      }
   }

/**
 * Remove a bid
 */
void BinarySearchTree::Remove(string bidId) {

   // If the binary tree is empty
   if (root == nullptr) {
      cout << "There are no bids to choose from. Please load bids, then remove." << endl;
   }
   // If the binary tree is not empty
   else {
      this->removeNode(root, bidId);
   }

}

/**
 * Search for a bid
 */
Bid BinarySearchTree::Search(string bidId) {

   // Start at top of tree
   Node* currNode = root;
   // Keep looking for matching bid until reaching bottom of tree
   while (currNode != nullptr) {
      // If current node matches, return it
      if (currNode->bid.bidId.compare(bidId) == 0) {
         return currNode->bid;
      }
      // If bidId is less than current
      else if (bidId.compare(currNode->bid.bidId) < 0) {
         currNode = currNode->left;
      }
      // If bidId is higher than current
      else {
         currNode = currNode->right;
      }
   }



	Bid bid;
    return bid;
}

/**
 * Add a bid to some node (recursive)
 *
 * @param node Current node in tree
 * @param bid Bid to be added
 */
void BinarySearchTree::addNode(Node* node, Bid bid) {

   // If node is larger than the bid, add to left subtree
   if (node->bid.bidId.compare(bid.bidId) > 0) {
      // If there is no left node
      if (node->left == nullptr) {
         node->left = new Node(bid);
      }
      // If there already is a left node
      else {
         // Recursively call addNode function
         this->addNode(node->left, bid);
      }
   }
   // Add to right subtree
   else {
      // If right is empty
      if (node->right == nullptr) {
         node->right = new Node(bid);
      }
      // If there already is a right node
      else {
         this->addNode(node->right, bid);
      }
   }
}
Node* BinarySearchTree::removeNode(Node* node, string bidId) {

   // Safety net- if node given is nullptr, return it
   if (node == nullptr) {
      return node;
   }

   // If bidId is less than node, recursively go down left subtree
   if (bidId.compare(node->bid.bidId) < 0) {
      node->left = removeNode(node->left, bidId);
   }
   // If bidId is greater than node, go down right subtree
   else if (bidId.compare(node->bid.bidId) > 0) {
      node->right = removeNode(node->right, bidId);
   }
   // If node matches bidId, remove node
   else {
      // If it's a leaf node, delete it and set it to nullptr
      if (node->left == nullptr && node->right == nullptr) {
         delete node;
         node = nullptr;
      }
      // If it has only a left child
      else if (node->left != nullptr && node->right == nullptr) {
         // Remember node we are deleting so we can deallocate memory properly
         Node* temp = node;
         node = node->left;
         // Deallocate node we are deleting
         delete temp;
      }
      //If it only has a right child
      else if(node->left == nullptr && node->right != nullptr) {
         Node* temp = node->right;
         node = node->right;
         delete temp;
      }
      // If it has two children
      else {
         // Remember right node to find its left most leaf
         Node* temp = node->right;
         // Find temp's left most leaf which is the lowest of all in subtree
         while (temp->left != nullptr) {
            temp = temp->left;
         }
         // Update original node's bid info to temp's (which is the next highest bidId in the subtree)
         node->bid = temp->bid;
         node->right = removeNode(node->right, temp->bid.bidId);
      }
   }
   return node;
}


void BinarySearchTree::inOrder(Node* node) {

   // If root wasn't null, traverse thru binary search tree
   if (node != nullptr) {

      // First go left
      inOrder(node->left);

      // Then print middle
      displayBid(node->bid);

      // Then go right
      inOrder(node->right);
   }

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
void loadBids(string csvPath, BinarySearchTree* bst) {
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
            bst->Insert(bid);
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

    // Define a binary search tree to hold all bids
    BinarySearchTree* bst;

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
            bst = new BinarySearchTree();

            // Initialize a timer variable before loading bids
            ticks = clock();

            // Complete the method call to load the bids
            loadBids(csvPath, bst);

            //cout << bst->Size() << " bids read" << endl;

            // Calculate elapsed time and display result
            ticks = clock() - ticks; // current clock ticks minus starting clock ticks
            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;

        case 2:
            bst->InOrder();
            break;

        case 3:
            ticks = clock();

            bid = bst->Search(bidKey);

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
            bst->Remove(bidKey);
            break;
        }
    }

    cout << "Good bye." << endl;

	return 0;
}
