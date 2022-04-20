//---------------------------------------------------------------------------
// NAME: 
// FILE: hashmap.h
// DATE: Spring 2022
// DESC: 
//---------------------------------------------------------------------------

#ifndef HASHMAP_H
#define HASHMAP_H

#include "map.h"
#include "arrayseq.h"
#include <functional>


template<typename K, typename V>
class HashMap : public Map<K,V>
{
public:

  // default constructor
  HashMap();

  // copy constructor
  HashMap(const HashMap& rhs);

  // move constructor
  HashMap(HashMap&& rhs);

  // copy assignment
  HashMap& operator=(const HashMap& rhs);

  // move assignment
  HashMap& operator=(HashMap&& rhs);  

  // destructor
  ~HashMap();
  
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
  bool prev_key(const K& key, K& next_key) const;

  // Removes all key-value pairs from the map. Does not change the
  // current capacity of the table.
  void clear();
  
  // statistics functions for the hash table implementation
  int min_chain_length() const;
  int max_chain_length() const;
  double avg_chain_length() const;
  
private:

  // node for linked-list separate chaining
  struct Node {
    K key;
    V value;
    Node* next;
  };

  // number of key-value pairs in map
  int count = 0;

  // max size of the (array) table
  int capacity = 16;

  // threshold for resize and rehash
  const double load_factor_threshold = 0.75;
  
  // array of linked lists
  Node** table = new Node*[capacity];

  // the hash function
  int hash(const K& key) const;

  // resize and rehash the table
  void resize_and_rehash();

  // initialize the table to all nullptr
  void init_table();
  
};


template<typename K, typename V>
HashMap<K,V>::HashMap()
{
  init_table();
}

// copy constructor
template<typename K, typename V>
HashMap<K, V>::HashMap(const HashMap& rhs)
{
  init_table();
  *this = rhs;
}

// move constructor
template<typename K, typename V>
HashMap<K, V>::HashMap(HashMap&& rhs)
{
  init_table();
  *this = std::move(rhs);
}

// copy assignment
template<typename K, typename V>
HashMap<K, V>& HashMap<K, V>::operator=(const HashMap<K, V>& rhs)
{
  if(this != &rhs){
    clear();
    delete[] table;
    capacity = rhs.capacity;
    table = new Node*[capacity];
    init_table();
    for(int i = 0; i < capacity; i++){
      Node* temp = rhs.table[i];
      while(temp){
        insert(temp -> key, temp -> value);
        temp = temp -> next;
      }
    }
  }
}

// move assignment
template<typename K, typename V>
HashMap<K, V>& HashMap<K, V>::operator=(HashMap<K, V>&& rhs)
{
  if(this != &rhs){
    clear();
    delete[] table;
    table = rhs.table;
    capacity = rhs.capacity;
    count = rhs.count;
    rhs.capacity = 16;
    rhs.count = 0;
    rhs.table = new Node*[rhs.capacity];
    rhs.init_table();
  }
}  

// destructor
template<typename K, typename V>
HashMap<K, V>::~HashMap()
{
  clear();
  delete[] table;
}

// Returns the number of key-value pairs in the map
template<typename K, typename V>
int HashMap<K, V>::size() const
{
  return count;
}

// Tests if the map is empty
template<typename K, typename V>
bool HashMap<K, V>::empty() const
{
  return count == 0;
}

// Allows values associated with a key to be updated. Throws
// out_of_range if the given key is not in the collection.
template<typename K, typename V>
V& HashMap<K, V>::operator[](const K& key)
{
  if(!contains(key)){
    throw std::out_of_range("Out of Range in Operator"); 
  }
  int index = hash(key);
  Node* temp = table[index];
  while(temp){
    if(temp -> key == key){
      return temp -> value;
    }
  }
}

// Returns the value for a given key. Throws out_of_range if the
// given key is not in the collection. 
template<typename K, typename V>
const V& HashMap<K, V>::operator[](const K& key) const
{
  if(!contains(key)){
    throw std::out_of_range("Out of Range in Operator"); 
  }
  int index = hash(key);
  Node* temp = table[index];
  while(temp){
    if(temp -> key == key){
      return temp -> value;
    }
  }
}

// Extends the collection by adding the given key-value pair.
// Expects key to not exist in map prior to insertion.
template<typename K, typename V>
void HashMap<K, V>::insert(const K& key, const V& value)
{
  if(count/(capacity*1.0) >= load_factor_threshold){
    resize_and_rehash();
  }
  int index = hash(key);
  Node* temp = table[index];
  Node* insertNode = new Node;
  insertNode -> key = key;
  insertNode -> value = value;
  insertNode -> next = temp;
  table[index] = insertNode;
  count++;
}

// Shrinks the collection by removing the key-value pair with the
// given key. Does not modify the collection if the collection does
// not contain the key. Throws out_of_range if the given key is not
// in the collection.
template<typename K, typename V>
void HashMap<K, V>::erase(const K& key)
{
  if(!contains(key)){
    throw std::out_of_range("Out of Range in Erase"); 
  }
  int index = hash(key);
  Node* temp = table[index] -> next;
  delete table[index];
  table[index] = temp;
  count--;
}

// Returns true if the key is in the collection, and false otherwise.
template<typename K, typename V>
bool HashMap<K, V>::contains(const K& key) const
{
  int index = hash(key);
  Node* temp = table[index];
  while(temp){
    if(temp -> key == key){
      return true;
    }
    temp = temp -> next;
  }
  return false;
}

// Returns the keys k in the collection such that k1 <= k <= k2
template<typename K, typename V>
ArraySeq<K> HashMap<K, V>::find_keys(const K& k1, const K& k2) const
{
  ArraySeq<K> keys;
  for(int i = 0; i < capacity; i++){
    Node* temp = table[i];
    while(temp){
      if(temp -> key >= k1 && k2 >= temp -> key){
        keys.insert(temp -> key, keys.size());
      }
      temp = temp -> next;
    }
  }
  return keys;
}

// Returns the keys in the collection in ascending sorted order
template<typename K, typename V>
ArraySeq<K> HashMap<K, V>::sorted_keys() const
{
  ArraySeq<K> keys;
  for(int i = 0; i < capacity; i++){
    Node* temp = table[i];
    while(temp){
      keys.insert(temp -> key, keys.size());
      temp = temp -> next;
    }
  }
  keys.sort();
  return keys;
}

// Gives the key (as an ouptput parameter) immediately after the
// given key according to ascending sort order. Returns true if a
// successor key exists, and false otherwise.
template<typename K, typename V>
bool HashMap<K, V>::next_key(const K& key, K& next_key) const
{
  K temp_next_key = key;
  for(int i = 0; i < capacity; i++){
    Node* temp = table[i];
    while(temp){
      if(temp -> key > key && (temp -> key < temp_next_key || temp_next_key == key)){
        temp_next_key = temp -> key;
      }
      temp = temp -> next;
    }
  }
  if(temp_next_key != key){
    next_key = temp_next_key;
    return true;
  } else {
    return false;
  }
}

// Gives the key (as an ouptput parameter) immediately before the
// given key according to ascending sort order. Returns true if a
// predecessor key exists, and false otherwise.
template<typename K, typename V>
bool HashMap<K, V>::prev_key(const K& key, K& next_key) const
{
  K temp_prev_key = key;
  for(int i = 0; i < capacity; i++){
    Node* temp = table[i];
    while(temp){
      if(temp -> key < key && (temp -> key > temp_prev_key || temp_prev_key == key)){
        temp_prev_key = temp -> key;
      }
      temp = temp -> next;
    }
  }
  if(temp_prev_key != key){
    next_key = temp_prev_key;
    return true;
  } else {
    return false;
  }
}

// Removes all key-value pairs from the map. Does not change the
// current capacity of the table.
template<typename K, typename V>
void HashMap<K, V>::clear()
{
  for(int i = 0; i < capacity; i++){
    Node* temp = table[i];
    while(temp){
      temp = temp -> next;
      delete table[i];
      table[i] = temp;
    }
  }
  count = 0;
}

// statistics functions for the hash table implementation
template<typename K, typename V>
int HashMap<K, V>::min_chain_length() const
{
  if(empty()){
    return 0;
  }
  int min = 0;
  for(int i = 0; i < capacity; i++){
    if(table[i] != nullptr){
      Node* temp = table[i];
      int tempCount = 0;
      while(temp){
        tempCount++;
        temp = temp -> next;
      }
      if(min > tempCount || min == 0){
        min = tempCount;
      }
    }
  }
  return min;
}

template<typename K, typename V>
int HashMap<K, V>::max_chain_length() const
{
  if(empty()){
    return 0;
  }
  int max = 0;
  for(int i = 0; i < capacity; i++){
    if(table[i] != nullptr){
      Node* temp = table[i];
      int tempCount = 0;
      while(temp){
        tempCount++;
        temp = temp -> next;
      }
      if(max < tempCount){
        max = tempCount;
      }
    }
  }
  return max;
}

template<typename K, typename V>
double HashMap<K, V>::avg_chain_length() const
{
  if(empty()){
    return 0.0;
  }
  double total_links = 0.0;
  for(int i = 0; i < capacity; i++){
    if(table[i] != nullptr){
      total_links++;
    }
  }
  return (count/total_links);
}

// the hash function
template<typename K, typename V>
int HashMap<K, V>::hash(const K& key) const
{
  std::hash<K>hash_code;
  return hash_code(key) % capacity;
}

// resize and rehash the table
template<typename K, typename V>
void HashMap<K, V>::resize_and_rehash()
{
  int old_capacity = capacity;
  capacity = capacity * 2;
  count = 0;
  Node** old_table = table;
  table = new Node*[capacity];
  init_table();
  for(int i = 0; i < old_capacity; i++){
    Node* temp = old_table[i];
    while(temp){
      insert(temp -> key, temp -> value);
      temp = temp -> next;
    }
  }
  for(int i = 0; i < old_capacity; i++){
    Node* temp = old_table[i];
    while(temp){
      temp = temp -> next;
      delete old_table[i];
      old_table[i] = temp;
    }
  }
  delete[] old_table;
}

// initialize the table to all nullptr
template<typename K, typename V>
void HashMap<K, V>::init_table()
{
  for(int i = 0; i < capacity; i++){
    table[i] = nullptr;
  }
}


#endif
