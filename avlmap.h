//---------------------------------------------------------------------------
// NAME: Connor Goldschmidt
// FILE: avlmap.h
// DATE: Spring 2022
// DESC: Impliments the AVLMap for the Map interface.
//---------------------------------------------------------------------------

#ifndef AVLMAP_H
#define AVLMAP_H

#include "map.h"
#include "arrayseq.h"


template<typename K, typename V>
class AVLMap : public Map<K,V>
{
public:

  // default constructor
  AVLMap();

  // copy constructor
  AVLMap(const AVLMap& rhs);

  // move constructor
  AVLMap(AVLMap&& rhs);

  // copy assignment
  AVLMap& operator=(const AVLMap& rhs);

  // move assignment
  AVLMap& operator=(AVLMap&& rhs);  

  // destructor
  ~AVLMap();
  
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

  // helper to print the tree for debugging
  void print() const;

private:

  // node for linked-list separate chaining
  struct Node {
    K key;
    V value;
    int height;
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

  // insert helper
  Node* insert(const K& key, const V& value, Node* st_root);
  
  // erase helper
  Node* erase(const K& key, Node* st_root);

  // find_keys helper
  void find_keys(const K& k1, const K& k2, const Node* st_root,
                 ArraySeq<K>& keys) const;

  // sorted_keys helper
  void sorted_keys(const Node* st_root, ArraySeq<K>& keys) const;

  // rotations
  Node* rotate_right(Node* k2);
  Node* rotate_left(Node* k2);

  // rebalance
  Node* rebalance(Node* st_root);

  // print helper
  void print(std::string indent, const Node* st_root) const;
};


template<typename K, typename V>
void AVLMap<K,V>::print() const
{
  print(std::string(""), root);
}


template<typename K, typename V>
void AVLMap<K,V>::print(std::string indent, const Node* st_root) const
{
  if (!st_root)
    return;
  std::cout << st_root->key << " (" << st_root->height << ")" <<  std::endl;
  if (st_root->left) {
    std::cout << indent << " lft: ";
    print(indent + " ", st_root->left);
  }
  if (st_root->right) {
    std::cout << indent << " rgt: ";
    print(indent + " ", st_root->right);
  }
}

template<typename K, typename V>
AVLMap<K,V>::AVLMap()
{
}

//initalizes the copy constructor
template<typename K, typename V>
AVLMap<K,V>::AVLMap(const AVLMap& rhs)
{
  *this = rhs;
}

//initalizes the move constructor
template<typename K, typename V>
AVLMap<K,V>::AVLMap(AVLMap&& rhs)
{
  *this = std::move(rhs);
}

//initalizes the copy assignment
template<typename K, typename V>
AVLMap<K, V>& AVLMap<K, V>::operator=(const AVLMap& rhs)
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
AVLMap<K, V>& AVLMap<K, V>::operator=(AVLMap&& rhs)
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
AVLMap<K, V>::~AVLMap()
{
  clear();
}

//returns the number of nodes stored in the tree
template<typename K, typename V>
int AVLMap<K, V>::size() const
{
  return count; 
}

//returns true if the tree is empty, false if not
template<typename K, typename V>
bool AVLMap<K, V>::empty() const
{
  if(count == 0){
    return true;
  }
  return false;
}

//given a valid key, returns the corrisponding key value
//throws an out_of_range exception if key is invalid
template<typename K, typename V>
V& AVLMap<K, V>::operator[](const K& key)
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
const V& AVLMap<K, V>::operator[](const K& key) const
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
void AVLMap<K, V>::insert(const K& key, const V& value)
{
  root = insert(key, value, root);
  count++;
}

//removes the given key and corrisponding value pair from the tree
template<typename K, typename V>
void AVLMap<K, V>::erase(const K& key)
{
  if(empty() || !contains(key)){
    throw std::out_of_range("Out of Range in Erase");
  }
  root = erase(key, root);
  count--;
}

//returns true if given key is in the tree, false if not
template<typename K, typename V>
bool AVLMap<K, V>::contains(const K& key) const
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
ArraySeq<K> AVLMap<K, V>::find_keys(const K& k1, const K& k2) const
{
  ArraySeq<K> keys;
  find_keys(k1, k2, root, keys);
  return keys;
}

//returns an array seq of all of the keys in sorted order
template<typename K, typename V>
ArraySeq<K> AVLMap<K, V>::sorted_keys() const
{
  ArraySeq<K> keys;
  sorted_keys(root, keys);
  return keys;
}

//returns the next key value in the tree
template<typename K, typename V>
bool AVLMap<K, V>::next_key(const K& key, K& next_key) const
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
bool AVLMap<K, V>::prev_key(const K& key, K& prev_key) const
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
void AVLMap<K, V>::clear()
{
  clear(root);
  count = 0;
  root = nullptr;
}

//returns the largest root to leaf path in the tree
template<typename K, typename V>
int AVLMap<K, V>::height() const
{
  if(!root){
    return 0;
  }
  return root -> height;
}

//helper function for the clear method
template<typename K, typename V>
void AVLMap<K, V>::clear(Node* st_root)
{
  if(!st_root){
    return;
  }
  clear(st_root -> left);
  clear(st_root -> right);
  delete st_root;
}

//Helper function for the insert method
template<typename K, typename V>
typename AVLMap<K,V>::Node* AVLMap<K,V>::insert(const K& key, const V& value, Node* st_root)
{
  if(st_root == nullptr){
    Node* new_node = new Node;
    new_node -> key = key;
    new_node -> value = value;
    new_node -> height = 1;
    new_node -> left = nullptr;
    new_node -> right = nullptr;
    st_root = new_node;
  } else { 
    if(key < st_root -> key){
      st_root -> left = insert(key, value, st_root -> left);
    } else {
      st_root -> right = insert(key, value, st_root -> right);
    }
    if(st_root -> left && st_root -> right){
      st_root -> height = std::max(st_root -> left -> height, st_root -> right -> height) + 1;
    } else if(st_root -> left){
      st_root -> height = st_root -> left -> height + 1;
    } else {
      st_root -> height = st_root -> right -> height + 1;
    } 
    st_root = rebalance(st_root);
  }
  return st_root;
}

//Rotates the given node to the right
template<typename K, typename V>
typename AVLMap<K,V>::Node* AVLMap<K,V>::rotate_right(Node* k2)
{
  Node* k1 = k2 -> left;
	k2 -> left = k1 -> right;
	k1 -> right = k2;
	return k1;
}

//Rotates the given node to the left
template<typename K, typename V>
typename AVLMap<K,V>::Node* AVLMap<K,V>::rotate_left(Node* k2)
{
  Node* k1 = k2 -> right;
	k2 -> right = k1 -> left;
	k1 -> left = k2;
	return k1;
}

//Balances the given node to a balance factor that is greater than -1 and less than 1
//then updates the height of the nodes
template<typename K, typename V>
typename AVLMap<K,V>::Node* AVLMap<K,V>::rebalance(Node* st_root)
{
  Node* left_child = st_root -> left;
  Node* right_child = st_root -> right;
  if(left_child && !right_child && left_child -> height > 1){
    if(left_child -> right){
      st_root -> left = rotate_left(left_child);
    }
    st_root = rotate_right(st_root);
  } else if(!left_child && right_child && right_child -> height > 1){
    if(right_child -> left){
        st_root -> right = rotate_right(right_child);
      }
    st_root = rotate_left(st_root);
  } else if(left_child && right_child){
    int balance_factor = left_child -> height - right_child -> height;
    if(balance_factor > 1){
      if(left_child -> right && (!left_child -> left || left_child -> left -> height < left_child -> right -> height)){
        st_root -> left = rotate_left(left_child);
      } 
      st_root = rotate_right(st_root);
    } else if(balance_factor < -1){
      if(right_child -> left && (!right_child -> right || right_child -> left -> height > right_child -> right -> height)){
        st_root -> right = rotate_right(right_child);
      }
      st_root = rotate_left(st_root);
    }
  }
  if(st_root -> left){
    if(st_root -> left -> right && st_root -> left -> left){
      st_root -> left -> height = std::max(st_root -> left -> left -> height, st_root -> left -> right -> height) + 1;
    } else if(st_root -> left -> right){
      st_root -> left -> height = st_root -> left -> right -> height + 1;
    } else if(st_root -> left -> left){
      st_root -> left -> height = st_root -> left -> left -> height + 1;
    } else {
      st_root -> left -> height = 1;
    }
  }
  if(st_root -> right){
    if(st_root -> right -> right && st_root -> right -> left){
      st_root -> right -> height = std::max(st_root -> right -> left -> height, st_root -> right -> right -> height) + 1;
    } else if(st_root -> right -> right){
      st_root -> right -> height = st_root -> right -> right -> height + 1;
    } else if(st_root -> right -> left){
      st_root -> right -> height = st_root -> right -> left -> height + 1;
    } else {
      st_root -> right -> height = 1;
    }
  }
  if(st_root -> left && st_root -> right){
    st_root -> height = std::max(st_root -> left -> height, st_root -> right -> height) + 1;
  }else if(st_root -> left){
    st_root -> height = st_root -> left -> height + 1;
  } else if(st_root -> right){
    st_root -> height = st_root -> right -> height + 1;
  } 
  return st_root;
}

//Helper function for the erase method
template<typename K, typename V>
typename AVLMap<K,V>::Node* AVLMap<K,V>::erase(const K& key, Node* st_root)
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
  if(st_root){
    if(st_root -> left && st_root -> right){
      st_root -> height = std::max(st_root -> left -> height, st_root -> right -> height) + 1;
    } else if(st_root -> left){
      st_root -> height = st_root -> left -> height + 1;
    } else if(st_root -> right){
      st_root -> height = st_root -> right -> height + 1;
    } else {
      st_root -> height = 1;
    }
    st_root = rebalance(st_root);
  } 
  return st_root;
}

//returns the root of the copied tree
template<typename K, typename V>
typename AVLMap<K,V>::Node* AVLMap<K, V>::copy(const Node* rhs_st_root) const
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

//helper function for the find_keys method
template<typename K, typename V>
void AVLMap<K, V>::find_keys(const K& k1, const K& k2, const Node* st_root, ArraySeq<K>& keys) const
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
void AVLMap<K, V>::sorted_keys(const Node* st_root, ArraySeq<K>& keys) const
{
  if(st_root){
    sorted_keys(st_root -> left, keys);
    keys.insert(st_root -> key, keys.size());
    sorted_keys(st_root -> right, keys);
  }
}

#endif