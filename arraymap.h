//---------------------------------------------------------------------------
// NAME: Connor Goldschmidt
// DATE: Spring 2022
// DESC: Implements the ArrayMap implementation of the Map interface.
//---------------------------------------------------------------------------

#ifndef ARRAYMAP_H
#define ARRAYMAP_H

#include "map.h"
#include "arrayseq.h"


template<typename K, typename V>
class ArrayMap : public Map<K,V>
{
public:

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

  // Returns true if the key is in the collection, and false
  // otherwise.
  bool contains(const K& key) const;

  // Returns the keys k in the collection such that k1 <= k <= k2
  ArraySeq<K> find_keys(const K& k1, const K& k2) const;

  // Returns the keys in the collection in ascending sorted order.
  ArraySeq<K> sorted_keys() const;  

  // Gives the key (as an ouptput parameter) immediately after the
  // given key according to ascending sort order. Returns true if a
  // successor key exists, and false otherwise.
  bool next_key(const K& key, K& next_key) const; 

  // Gives the key (as an ouptput parameter) ly before the
  // given key according to ascending sort order. Returns true if a
  // predecessor key exists, and false otherwise.
  bool prev_key(const K& key, K& next_key) const;

  // Removes all key-value pairs from the map.
  void clear();

private:

  // implemented as a resizable array of (key-value) pairs
  ArraySeq<std::pair<K,V>> seq;

};


//Returns the size of the current ArrayMap
template<typename K, typename V>
int ArrayMap<K, V>::size() const
{
  return seq.size();
}

//Returns true if the current ArrayMap is empty, false if not
template<typename K, typename V>
bool ArrayMap<K, V>::empty() const
{
  return seq.empty();
}

//Returns the assocated value pair of the input key, 
//throws an out_of_range exception if key is not contained 
//in the ArrayMap 
template<typename K, typename V>
V& ArrayMap<K, V>::operator[](const K& key)
{
  for(int i = 0; i < seq.size(); i++){
    if(seq[i].first == key){
      return seq[i].second;
    }
  }
  throw std::out_of_range("Out of Range in Operator ArrayMap"); 
}

//Returns the assocated value pair of the input key, 
//throws an out_of_range exception if key is not contained 
//in the ArrayMap 
template<typename K, typename V>
const V& ArrayMap<K, V>::operator[](const K& key) const
{
  for(int i = 0; i < seq.size(); i++){
    if(seq[i].first == key){
      return seq[i].second;
    }
  }
  throw std::out_of_range("Out of Range in Operator ArrayMap"); 
}

//Inserts the given key value pair in the ArrayMap
template<typename K, typename V>
void ArrayMap<K, V>::insert(const K& key, const V& value)
{
  seq.insert(std::make_pair(key, value), seq.size());
}

//Removes the key value pair of the given key in the ArrayMap,
//throws an out_of_range exception if key is not in the list
template<typename K, typename V>
void ArrayMap<K, V>::erase(const K& key)
{
  for(int i = 0; i < seq.size(); i++){
    if(seq[i].first == key){
      seq.erase(i);
      return;
    }
  }
  throw std::out_of_range("Out of Range in Erase"); 
}

//Returns true if the given key is found in the ArrayMap, false if not
template<typename K, typename V>
bool ArrayMap<K, V>::contains(const K& key) const
{
  for(int i = 0; i < seq.size(); i++){
    if(seq[i].first == key){
      return true;
    }
  }
  return false;
}

//Returns all of the keys between or equal to the values of k1 and k2
template<typename K, typename V>
ArraySeq<K> ArrayMap<K, V>::find_keys(const K& k1, const K& k2) const
{
  ArraySeq<K> keys;
  for(int i = 0; i < size(); i++){
    if(seq[i].first >= k1 && seq[i].first <= k2){
      keys.insert(seq[i].first, keys.size());
    }
  }
  return keys;
}

//Returns a sorted ArraySeq of all of the keys
template<typename K, typename V>
ArraySeq<K> ArrayMap<K, V>::sorted_keys() const
{
  ArraySeq<K> keys;
  if(empty()){
    return keys;
  }
  for(int i = 0; i < size(); i++){
    keys.insert(seq[i].first, keys.size());
  }
  keys.sort();
  return keys;
}

//Returns true if there is a key larger than the input key and
//updates the next_key parameter. Returns false if not
template<typename K, typename V>
bool ArrayMap<K, V>::next_key(const K& key, K& next_key) const
{
  if(empty() || seq[size() - 1].first <= key){
    return false;
  }
  for(int i = 1; i <= size() - 1; i++){
    if(seq[i - 1].first <= key && key < seq[i].first){
      next_key = seq[i].first;
      return true;
    }
  }
}

//Returns true if there is a key smaller than the input key and
//updates the next_key parameter. Returns false if not
template<typename K, typename V>
bool ArrayMap<K, V>::prev_key(const K& key, K& prev_key) const
{
  if(empty() || seq[0].first >= key){
    return false;
  }
  for(int i = 0; i < size() - 1; i++){
    if(seq[i].first < key && key <= seq[i + 1].first){
      prev_key = seq[i].first;
      return true;
    }
  }
}

//Clears the current ArrayMap
template<typename K, typename V>
void ArrayMap<K, V>::clear()
{
  seq.clear();
}

#endif