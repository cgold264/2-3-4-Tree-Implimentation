//---------------------------------------------------------------------------
// NAME: Connor Goldschmidt
// FILE: bstmap.h
// DATE: Spring 2022
// DESC: Binary Search Tree implimentation of the map interface.
//---------------------------------------------------------------------------

#ifndef BSTMAP_H
#define BSTMAP_H

#include "map.h"
#include "arrayseq.h"


template<typename K, typename V>
class BSTMap : public Map<K,V>
{
public:

  // default constructor
  BSTMap();

  // copy constructor
  BSTMap(const BSTMap& rhs);

  // move constructor
  BSTMap(BSTMap&& rhs);

  // copy assignment
  BSTMap& operator=(const BSTMap& rhs);

  // move assignment
  BSTMap& operator=(BSTMap&& rhs);  

  // destructor
  ~BSTMap();
  
  // Returns the number of key-value pairs in the map
  int size() const;

  // Tests if the map is empty
  bool empty() const;

  // Allows values associated with a key to be updated. Throws
  // out_of_range if the given key is not in the collection.
  V& operator[](const K& key);

  // Returns the value for a given key. Throws out_of_range if the
  // given key is not in the collection. 
  const V& operator[](const K& key) const;

  // Extends the collection by adding the given key-value pair.
  // Expects key to not exist in map prior to insertion.
  void insert(const K& key, const V& value);

  // Shrinks the collection by removing the key-value pair with the
  // given key. Does not modify the collection if the collection does
  // not contain the key. Throws out_of_range if the given key is not
  // in the collection.
  void erase(const K& key);

  // Returns true if the key is in the collection, and false otherwise.
  bool contains(const K& key) const;

  // Returns the keys k in the collection such that k1 <= k <= k2
  ArraySeq<K> find_keys(const K& k1, const K& k2) const;

  // Returns the keys in the collection in ascending sorted order
  ArraySeq<K> sorted_keys() const;  

  // Gives the key (as an ouptput parameter) immediately after the
  // given key according to ascending sort order. Returns true if a
  // successor key exists, and false otherwise.
  bool next_key(const K& key, K& next_key) const; 

  // Gives the key (as an ouptput parameter) immediately before the
  // given key according to ascending sort order. Returns true if a
  // predecessor key exists, and false otherwise.
  bool prev_key(const K& key, K& prev_key) const; 

  // Removes all key-value pairs from the map.
  void clear();

  // Returns the height of the binary search tree
  int height() const;
  
private:

  // node for linked-list separate chaining
  struct Node {
    K key;
    V value;
    Node* left;
    Node* right;
  };

  // number of key-value pairs in map
  int count = 0;

  // array of linked lists
  Node* root = nullptr;

  // clean up the tree and reset count to zero given subtree root
  void clear(Node* st_root);

  // copy assignment helper
  Node* copy(const Node* rhs_st_root) const;
  
  // erase helper
  Node* erase(const K& key, Node* st_root);

  // find_keys helper
  void find_keys(const K& k1, const K& k2, const Node* st_root,
                 ArraySeq<K>& keys) const;

  // sorted_keys helper
  void sorted_keys(const Node* st_root, ArraySeq<K>& keys) const;

  // height helper
  int height(const Node* st_root) const;
  
};


template<typename K, typename V>
BSTMap<K,V>::BSTMap()
{
}

// TODO: Implement the BST Map functions below. Note that you are not
// allowed to add any additional helper functions and must implement
// the functions as per the guidelines in the homework and lecture
// notes.

//initalizes the copy constructor
template<typename K, typename V>
BSTMap<K,V>::BSTMap(const BSTMap& rhs)
{
  *this = rhs;
}

//initalizes the move constructor
template<typename K, typename V>
BSTMap<K,V>::BSTMap(BSTMap&& rhs)
{
  *this = std::move(rhs);
}

//initalizes the copy assignment
template<typename K, typename V>
BSTMap<K, V>& BSTMap<K, V>::operator=(const BSTMap& rhs)
{
  if(this != &rhs){
    clear();
    root = copy(rhs.root);
    count = rhs.count;
  }
  return *this;
}

//initalizes the move assignment
template<typename K, typename V>
BSTMap<K, V>& BSTMap<K, V>::operator=(BSTMap&& rhs)
{
  if(this != &rhs){
    clear();
    root = rhs.root;
    count = rhs.count;
    rhs.count = 0;
    rhs.root = nullptr;
  }
  return *this;
}

//initalizes the destructor
template<typename K, typename V>
BSTMap<K, V>::~BSTMap()
{
  clear();
}

//returns the number of nodes stored in the tree
template<typename K, typename V>
int BSTMap<K, V>::size() const
{
  return count; 
}

//returns true if the tree is empty, false if not
template<typename K, typename V>
bool BSTMap<K, V>::empty() const
{
  if(count == 0){
    return true;
  }
  return false;
}

//given a valid key, returns the corrisponding key value
//throws an out_of_range exception if key is invalid
template<typename K, typename V>
V& BSTMap<K, V>::operator[](const K& key)
{
  Node* temp = root;
  while(temp){
    if(temp -> key == key){
      return temp -> value;
    }
    if(key < temp -> key){
      temp = temp -> left;
    } else {
      temp = temp -> right;
    }
  }
  throw std::out_of_range("Out of Range in Operator");
}

//given a valid key, returns the corrisponding key value as a constant
//throws an out_of_range exception if key is invalid
template<typename K, typename V>
const V& BSTMap<K, V>::operator[](const K& key) const
{
  Node* temp = root;
  while(temp){
    if(temp -> key == key){
      return temp -> value;
    }
    if(key < temp -> key){
      temp = temp -> left;
    } else {
      temp = temp -> right;
    }
  }
  throw std::out_of_range("Out of Range in Operator");
}

//inserts the given key, value pair into a leaf node in the tree
template<typename K, typename V>
void BSTMap<K, V>::insert(const K& key, const V& value)
{
  Node* new_node = new Node;
  new_node -> key = key;
  new_node -> value = value;
  new_node -> left = nullptr;
  new_node -> right = nullptr;
  if(empty()){
    root = new_node;
  } else {
    Node* temp = root;
    Node* parent;
    while(temp){
      parent = temp;
      if(key < temp -> key){
        temp = temp -> left;
      } else {
        temp = temp -> right;
      }
    }
    if(key < parent -> key){
      parent -> left = new_node;
    } else {
      parent -> right = new_node;
    }
  }
  count++;
}

//removes the given key and corrisponding value pair from the tree
template<typename K, typename V>
void BSTMap<K, V>::erase(const K& key)
{
  if(empty() || !contains(key)){
    throw std::out_of_range("Out of Range in Erase");
  }
  root = erase(key, root);
  count--;
}

//returns true if given key is in the tree, false if not
template<typename K, typename V>
bool BSTMap<K, V>::contains(const K& key) const
{
  Node* temp = root;
  while(temp){
    if(temp -> key == key){
      return true;
    }
    if(key < temp -> key){
      temp = temp -> left;
    } else {
      temp = temp -> right;
    }
  }
  return false;
}

//returns an ArraySeq of key values that are between k1 and k2
template<typename K, typename V>
ArraySeq<K> BSTMap<K, V>::find_keys(const K& k1, const K& k2) const
{
  ArraySeq<K> keys;
  find_keys(k1, k2, root, keys);
  return keys;
}

//returns an array seq of all of the keys in sorted order
template<typename K, typename V>
ArraySeq<K> BSTMap<K, V>::sorted_keys() const
{
  ArraySeq<K> keys;
  sorted_keys(root, keys);
  return keys;
}

//returns the next key value in the tree
template<typename K, typename V>
bool BSTMap<K, V>::next_key(const K& key, K& next_key) const
{
  Node* temp = root;
  K temp_next_key = key;
  while(temp){
    if(temp -> key > key && (temp -> key < temp_next_key || temp_next_key == key)){
      temp_next_key = temp -> key;
    }
    if(key < temp -> key){
      temp = temp -> left;
    } else {
      temp = temp -> right;
    }
  }
  if(temp_next_key == key){
    return false;
  } else {
    next_key = temp_next_key;
    return true;
  }
}

//returns the previous key value in the tree
template<typename K, typename V>
bool BSTMap<K, V>::prev_key(const K& key, K& prev_key) const
{
  Node* temp = root;
  K temp_prev_key = key;
  while(temp){
    if(temp -> key < key && (temp -> key > temp_prev_key || temp_prev_key == key)){
      temp_prev_key = temp -> key;
    }
    if(key <= temp -> key){
      temp = temp -> left;
    } else {
      temp = temp -> right;
    } 
  }
  if(temp_prev_key == key){
    return false;
  } else {
    prev_key = temp_prev_key;
    return true;
  }
}

//deletes all of the values in the tree
template<typename K, typename V>
void BSTMap<K, V>::clear()
{
  clear(root);
  count = 0;
  root = nullptr;
}

//returns the largest root to leaf path in the tree
template<typename K, typename V>
int BSTMap<K, V>::height() const
{
  return height(root);
}

//helper function for the clear method
template<typename K, typename V>
void BSTMap<K, V>::clear(Node* st_root)
{
  if(!st_root){
    return;
  }
  clear(st_root -> left);
  clear(st_root -> right);
  delete st_root;
}

//returns the root of the copied tree
template<typename K, typename V>
typename BSTMap<K,V>::Node* BSTMap<K, V>::copy(const Node* rhs_st_root) const
{
  if(!rhs_st_root){
    return nullptr;
  } else {
    Node* new_node = new Node;
    new_node -> key = rhs_st_root -> key;
    new_node -> value = rhs_st_root -> value;
    new_node -> left = copy(rhs_st_root -> left);
    new_node -> right = copy(rhs_st_root -> right);
    return new_node;
  }
}

//helper function for the erase method
template<typename K, typename V>
typename BSTMap<K,V>::Node* BSTMap<K, V>::erase(const K& key, Node* st_root)
{
  if(!st_root){
    return nullptr;
  }
  if(key < st_root -> key ){
    st_root -> left = erase(key, st_root -> left);
  } else if(key > st_root -> key){
    st_root -> right = erase(key, st_root -> right);
  } else {
    if(st_root -> left == nullptr && st_root -> right == nullptr){
      delete st_root;
      st_root = nullptr;
    } else if(st_root -> left == nullptr){
      Node* temp = st_root;
      st_root = st_root -> right;
      delete temp;
    } else if(st_root -> right == nullptr){
      Node* temp = st_root;
      st_root = st_root -> left;
      delete temp;
    } else {
      Node* temp = st_root -> right;
      while(temp -> left){
        temp = temp -> left;
      }
      st_root -> key = temp -> key;
      st_root -> value = temp -> value;
      st_root -> right = erase(temp -> key, st_root -> right);
    }
  }
  return st_root;
}

//helper function for the find_keys method
template<typename K, typename V>
void BSTMap<K, V>::find_keys(const K& k1, const K& k2, const Node* st_root, ArraySeq<K>& keys) const
{
  if(st_root){
    if(k1 <= st_root -> key){
      find_keys(k1, k2, st_root -> left, keys);
    }
    if(k2 >= st_root -> key && k1 <= st_root -> key){
      keys.insert(st_root -> key, keys.size());
    }
    if(k2 >= st_root -> key){
      find_keys(k1, k2, st_root -> right, keys);
    }
  }
}

//helper function for the sorted_keys method
template<typename K, typename V>
void BSTMap<K, V>::sorted_keys(const Node* st_root, ArraySeq<K>& keys) const
{
  if(st_root){
    sorted_keys(st_root -> left, keys);
    keys.insert(st_root -> key, keys.size());
    sorted_keys(st_root -> right, keys);
  }
}

//helper function for the height method
template<typename K, typename V>
int BSTMap<K, V>::height(const Node* st_root) const
{
  if(!st_root){
    return 0;
  }
  int count_left = height(st_root -> left);
  int count_right = height(st_root -> right);
  if(count_left > count_right){
    return 1 + count_left;
  }
  return 1 + count_right;
}

#endif
