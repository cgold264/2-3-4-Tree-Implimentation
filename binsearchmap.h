//---------------------------------------------------------------------------
// NAME: Connor Goldschmidt
// FILE: binsearchmap.h
// DATE: 3/21/22
// DESC: Impliments the Map interface and utilizes the binary search algorithm.
//---------------------------------------------------------------------------

#ifndef BINSEARCHMAP_H
#define BINSEARCHMAP_H

#include "map.h"
#include "arrayseq.h"


template<typename K, typename V>
class BinSearchMap : public Map<K,V>
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

  // Extends the collection by adding the given key-value
  // pair. Assumes the key being added is not present in the
  // collection. Insert does not check if the key is present.
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

  // Gives the key (as an ouptput parameter) immediately before the
  // given key according to ascending sort order. Returns true if a
  // predecessor key exists, and false otherwise.
  bool prev_key(const K& key, K& prev_key) const; 

  // Removes all key-value pairs from the map.
  void clear();
  

private:

  // If the key is in the collection, bin_search returns true and
  // provides the key's index within the array sequence (via the index
  // output parameter). If the key is not in the collection,
  // bin_search returns false and provides the last index checked by
  // the binary search algorithm. 
  bool bin_search(const K& key, int& index) const;
  
  // implemented as a resizable array of (key-value) pairs
  ArraySeq<std::pair<K,V>> seq;

};

//Returns true if the given key is contained in the list. 
//Updates the index parameter with the idex of the given key.
//Returns false if the key is not contained in the Map.
template<typename K, typename V>
bool BinSearchMap<K, V>::bin_search(const K& key, int& index) const
{
  int start = 0;
  int end = size() - 1;
  int mid;
  while(start <= end){
    mid = (start + end) / 2;
    if(key == seq[mid].first){
      index = mid;
      return true;
    }else if(key < seq[mid].first){
      end = mid - 1;
    } else {
      start = mid + 1;
    }
  }
  return false;
}

//Returns the size of the current BinSearchMap
template<typename K, typename V>
int BinSearchMap<K, V>::size() const
{
  return seq.size();
}

//Returns true if the current BinSearchMap is empty, false if not
template<typename K, typename V>
bool BinSearchMap<K, V>::empty() const
{
  return seq.empty();
}

//Returns the assocated value pair of the input key, 
//throws an out_of_range exception if key is not contained 
//in the BinSearchMap 
template<typename K, typename V>
V& BinSearchMap<K, V>::operator[](const K& key)
{
  int index;
  if(bin_search(key, index)){
    return seq[index].second;
  }
  throw std::out_of_range("Out of Range in Operator BinSearchMap"); 
}

//Returns the assocated value pair of the input key, 
//throws an out_of_range exception if key is not contained 
//in the BinSearchMap 
template<typename K, typename V>
const V& BinSearchMap<K, V>::operator[](const K& key) const
{
  int index;
  if(bin_search(key, index)){
    return seq[index].second;
  }
  throw std::out_of_range("Out of Range in Operator BinSearchMap"); 
}

//Inserts the given key value pair in the BinSearchMap
template<typename K, typename V>
void BinSearchMap<K, V>::insert(const K& key, const V& value)
{
  if(empty() || seq[size() - 1].first < key){
    seq.insert(std::make_pair(key, value), size());
  } else if(seq[0].first > key){
    seq.insert(std::make_pair(key, value), 0);
  } else {
    K previous;
    int index;
    prev_key(key, previous);
    bin_search(previous, index);
    seq.insert(std::make_pair(key, value), index + 1);
  }
}

//Removes the key value pair of the given key in the BinSearchMap,
//throws an out_of_range exception if key is not in the list
template<typename K, typename V>
void BinSearchMap<K, V>::erase(const K& key)
{
  int index;
  if(!bin_search(key, index)){
    throw std::out_of_range("Out of Range in Erase"); 
  }
  seq.erase(index);
}

//Returns true if the given key is found in the BinSearchMap, false if not
template<typename K, typename V>
bool BinSearchMap<K, V>::contains(const K& key) const
{
  int index;
  return bin_search(key, index);
}

//Returns all of the keys between or equal to the values of k1 and k2
template<typename K, typename V>
ArraySeq<K> BinSearchMap<K, V>::find_keys(const K& k1, const K& k2) const
{
  ArraySeq<K> keys;
  int index;
  if(!empty() && k1 <= seq[size() - 1].first){
    if(k1 < seq[0].first){
      index = 0;
    } 
    bin_search(k1, index);
    while(index < size() && seq[index].first <= k2){
      keys.insert(seq[index].first, keys.size());
      index++;
    }
  }
  return keys;
}

//Returns a sorted ArraySeq of all of the keys
template<typename K, typename V>
ArraySeq<K> BinSearchMap<K, V>::sorted_keys() const
{
  ArraySeq<K> keys;
  for(int i = 0; i < size(); i++){
    keys.insert(seq[i].first, keys.size());
  }
  return keys;
}

//Returns true if there is a key larger than the input key and
//updates the next_key parameter. Returns false if not
template<typename K, typename V>
bool BinSearchMap<K, V>::next_key(const K& key, K& next_key) const
{
  if(empty() || seq[size() - 1].first <= key){
    return false;
  }
  int index;
  if(bin_search(key, index)){
    next_key = seq[index + 1].first;
    return true;
  } else {
    for(int i = 1; i < size(); i++){
      if(seq[i - 1].first <= key && key < seq[i].first){
        next_key = seq[i].first;
        return true;
      }
    }
  }
}

//Returns true if there is a key smaller than the input key and
//updates the next_key parameter. Returns false if not
template<typename K, typename V>
bool BinSearchMap<K, V>::prev_key(const K& key, K& prev_key) const
{
  if(empty() || seq[0].first >= key){
    return false;
  }
  int index;
  if(bin_search(key, index)){
    prev_key = seq[index - 1].first;
    return true;
  } else {
    for(int i = 0; i < size() - 1; i++){
      if(seq[i].first < key && key <= seq[i + 1].first){
        prev_key = seq[i].first;
        return true;
      }
    }
  }
}

//Clears the current BinSearchMap
template<typename K, typename V>
void BinSearchMap<K, V>::clear()
{
  seq.clear();
}

#endif
