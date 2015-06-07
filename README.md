RBTree
======

Red Black Tree for C++

Requirements:
1.  Overload the comparison operators, >, <, >=, ==, for the key class.

bool insert(T x, U y);
Inserts a key-value pair into the tree. If key already exists, the value would be updated.
Input:
  1.  x: key
  2.  y: value
Output:
  1. true if no such key, else false
  
bool remove(T x);
Removes a key-value pair into the tree.
Input:
  1.  x: key
  2.  y: value
Output:
  1. true if key is found, else false
  
bool minimum(T &x, U &y) const;
Finds the key-value pair with the smallest key in the tree.
Input:
  1.  x: parameter to store the key
  2.  y: parameter to the store the value
Output:
  1. true unless tree is empty
  
bool maximum(T &x, U &y) const;
Finds the key-value pair with the largest key in the tree.
Input:
  1.  x: parameter to store the key
  2.  y: parameter to the store the value
Output:
  1. true unless tree is empty
  
unsigned int getSize() const;
Returns the size of the tree.
Output:
  1. size of the tree
  
bool search(T x, U&y) const;
Find the value with the given key.
Input:
  1. x: key to search for
  2. y: parameter to store the value
Output:
  1. true if key is found, else false

void print(ostream& stream)
Prints the whole red-black tree using the provided ostream object
