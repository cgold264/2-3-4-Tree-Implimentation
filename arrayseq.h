//---------------------------------------------------------------------------
// NAME: Connor Goldschmidt
// FILE: arrayseq.h
// DATE: Spring 2022
// DESC: Resizeable-array implementation of the sequence interface.
//----------------------------------------------------------------------


#ifndef ARRAYLIST_H
#define ARRAYLIST_H

#include <stdexcept>
#include <ostream>
#include "sequence.h"


template<typename T>
class ArraySeq : public Sequence<T>
{
public:

  // Default constructor
  ArraySeq();

  // Copy constructor
  ArraySeq(const ArraySeq& rhs);

  // Move constructor
  ArraySeq(ArraySeq&& rhs);

  // Copy assignment operator
  ArraySeq& operator=(const ArraySeq& rhs);

  // Move assignment operator
  ArraySeq& operator=(ArraySeq&& rhs);
  
  // Destructor
  ~ArraySeq();
  
  // Returns the number of elements in the sequence
  int size() const;

  // Tests if the sequence is empty
  bool empty() const;

  // Removes all of the elements from the sequence
  void clear();
  
  // Returns a reference to the element at the index in the
  // sequence. Throws out_of_range if index is invalid (less than 0 or
  // greater than or equal to size()).
  T& operator[](int index);

  // Returns a constant address to the element at the index in the
  // sequence. Throws out_of_range if index is invalid (less than 0 or
  // greater than or equal to size()).
  const T& operator[](int index) const;

  // Extends the sequence by inserting the element at the given index.
  // Throws out_of_range if the index is invalid (less than 0 or
  // greater than size()).
  void insert(const T& elem, int index);

  // Shrinks the sequence by removing the element at the index in the
  // sequence. Throws out_of_range if index is invalid.
  void erase(int index);

  // Returns true if the element is in the sequence, and false
  // otherwise.
  bool contains(const T& elem) const;

  // Sorts the elements in the sequence in place using less than equal
  // (<=) operator. Uses quick sort with a randomly selected pivot
  // index.
  void sort(); 

  // Sorts the sequence in place using the merge sort algorithm.
  void merge_sort();

  // Sorts the sequence in place using the quick sort algorithm. Uses
  // first element for pivot values.
  void quick_sort();

  // Sorts the sequence in place using the quick sort algorithm. Uses
  // randomly selected indexes for pivot values.
  void quick_sort_random();

  
private:

  // resizable array
  T* array = nullptr;

  // size of list
  int count = 0;

  // max capacity of the array
  int capacity = 0;

  // helper to double the capacity of the array
  void resize();

  // sort function helpers
  void merge_sort(int start, int end);
  void quick_sort(int start, int end);
  void quick_sort_random(int start, int end);

  // random seed for quick sort
  int seed = 22;
  
};


template<typename T>
std::ostream& operator<<(std::ostream& stream, const ArraySeq<T>& seq)
{
  int n = seq.size();
  for (int i = 0; i < n - 1; ++i) 
    stream << seq[i] << ", ";
  if (n > 0)
    stream << seq[n - 1];
  return stream;
}


template<typename T>
ArraySeq<T>::ArraySeq()
{
}

//Post: Initializes the copy constructor.
template<typename T>
ArraySeq<T>::ArraySeq(const ArraySeq& rhs)
{
  *this = rhs;
}

//Post: Initializes the move constructor.
template<typename T>
ArraySeq<T>::ArraySeq(ArraySeq&& rhs)
{
  *this = std::move(rhs);
}

//Post: Initializes the copy assignment. 
template<typename T>
ArraySeq<T>& ArraySeq<T>::operator=(const ArraySeq<T>& rhs)
{
  if(this != &rhs){
    clear();
    array = new T[rhs.capacity];
    for(int i = 0; i < rhs.count; i++){
      array[i] = rhs.array[i];
    }
    capacity = rhs.capacity;
    count = rhs.count;
  }
  return *this;
}

//Post: Initializes the move assignment.
template<typename T>
ArraySeq<T>& ArraySeq<T>::operator=(ArraySeq<T>&& rhs)
{
  if(this != &rhs){
    clear();
    array = rhs.array;
    capacity = rhs.capacity;
    count = rhs.count;
    rhs.capacity = 0;
    rhs.count = 0;
    rhs.array = nullptr;
  }
}

//Post: Initalizes the deconstructor.
template<typename T>
ArraySeq<T>::~ArraySeq<T>()
{
  clear();
}

//Post: Clears the current ArraySeq object.
template<typename T>
void ArraySeq<T>::clear()
{
  delete [] array;
  array = nullptr;
  capacity = 0;
  count = 0;
}

//Post: Returns the current size. 
template<typename T>
int ArraySeq<T>::size() const
{
  return count;
}

//Post: Returns true if the current arrayseq is empty, false if not.
template<typename T>
bool ArraySeq<T>::empty() const
{
  if(count == 0){
    return true;
  }
  return false;
}

//Post: Overrides the access operator. 
template<typename T>
T& ArraySeq<T>::operator[](int index)
{
  if(index < 0 || index >= count){
    throw std::out_of_range("Out of Range in ArraySeq Operator"); 
  } 
    return array[index];
}

//Post: Overrides the update operator.
template<typename T>
const T& ArraySeq<T>::operator[](int index) const
{
  if(index < 0 || index >= count){
    throw std::out_of_range("Out of Range in ArraySeq Operator"); 
  } 
    return array[index];
}

//Pre: Index must be in range. Must be greater or equal to 
//0 and less than or equal to count.
//Post: Inserts the given value and the given index.
template<typename T>
void ArraySeq<T>::insert(const T& elem, int index)
{
  if(index < 0 || index > count){
    throw std::out_of_range("Out of Range in Insert"); 
  } 
  if(count >= capacity ){
      resize();
  }
  for(int i = count - 1; i >= index; i--){
    array[i + 1] = array[i];
  }
  count++;
  array[index] = elem;
}

//Pre: Index must be in range. Must be greater or equal to 
//0 and less than or equal to count.
//Post: Removes the element at the given index.
template<typename T>
void ArraySeq<T>::erase(int index)
{
  if(index < 0 || index >= count){
    throw std::out_of_range("Out of Range in Erase"); 
  } 
  for(int i = index; i < count - 1; i++){
    array[i] = array[i + 1];
  }
  count--;
}

//Post: Returns true if the given element is contained in the list,
//false if not.
template<typename T>
bool ArraySeq<T>::contains(const T& elem) const
{
  for(int i = 0; i < size(); i++){
    if(array[i] == elem){
      return true;
    }
  }
  return false;
}  

//Post: Increases the capacity of the array by twice it's origional
//value. Sets the size as 1 if it is empty.
template<typename T>
void ArraySeq<T>::resize()
{
  if(capacity == 0){
    capacity = 1;
  } else {
    capacity *= 2;
  }
    T* new_array = new T[capacity];
    for(int i = 0; i < count; i++){
      new_array[i] = array[i];
    }
    delete [] array;
    array = new_array;
  
}

//Post: Sorts the current arrayseq.
template<typename T>
void ArraySeq<T>::sort()
{
  merge_sort();
}

//Post: Sorts the current arrayseq using the merge sort technique.
template<typename T>
void ArraySeq<T>::merge_sort()
{
  if(count <= 1){
    return;
  }
  merge_sort(0, count - 1);
}
  
//Post: Sorts the current arrayseq using the quick sort technique. The pivot value is set to the
//first index of the sequence.
template<typename T>
void ArraySeq<T>::quick_sort()
{
  if(count <= 1){
    return;
  }
  quick_sort(0, count - 1);  
}

//Post: Sorts the current arrayseq using the quick sort technique. The pivot value is set to a
//random index in the sequence.
template<typename T>
void ArraySeq<T>::quick_sort_random()
{
  if(count <= 1){
    return;
  }
    std::srand(seed);
    quick_sort_random(0, count - 1);    
}

//Pre: Start and end are assumed to not be less than 0.
//Post: Impliments the private helper method for the merge sort.
template<typename T>
void ArraySeq<T>::merge_sort(int start, int end)
{
  if(start < end){
    int mid = (start + end) / 2;
    merge_sort(start, mid);
    merge_sort(mid + 1, end);
    T temp[(end - start) + 1];
    int first = start;
    int second = mid + 1;
    int list_index = 0;
    while(first <= mid && second <= end){
      if(array[first] < array[second]){
        temp[list_index++] = array[first++];
      } else {
        temp[list_index++] = array[second++];
      }
    }
    while(first <= mid){
      temp[list_index++] = array[first++];
    }
    while(second <= end){ 
      temp[list_index++] = array[second++];
    }
    for(int i = 0; i <= (end - start); i++){
      array[start + i] = temp[i];
    }
  }
}

//Pre: Start and end are assumed to not be less than 0.
//Post: implements the private helper method for the quick sort.
template<typename T>
void ArraySeq<T>::quick_sort(int start, int end)
{
  if(start < end){
    T pivot_val = array[start];
    int end_p1 = start;
    for(int i = start + 1; i <= end; i++){
      if(array[i] < pivot_val){
        end_p1++;
        T temp = array[i];
        array[i] = array[end_p1];
        array[end_p1] = temp;
      }
    }
    T temp = array[start];
    array[start] = array[end_p1];
    array[end_p1] = temp;
    quick_sort(start, end_p1 - 1);
    quick_sort(end_p1 + 1, end);
  }
}

//Pre: Start and end are assumed to not be less than 0.
//Post: Impliments the private helper method for the random quick sort.
template<typename T>
void ArraySeq<T>::quick_sort_random(int start, int end)
{
  if(start < end){
    int pivot_index = (std::rand() % (end - start));
    T pivot_val = array[pivot_index];
    T move_front = array[0];
    array[0] = pivot_val;
    array[pivot_index] = move_front;
    int end_p1 = start;
    for(int i = start + 1; i <= end; i++){
      if(array[i] < pivot_val){
        end_p1++;
        T temp = array[i];
        array[i] = array[end_p1];
        array[end_p1] = temp;
      }
    }
    T temp = array[start];
    array[start] = array[end_p1];
    array[end_p1] = temp;
    quick_sort(start, end_p1 - 1);
    quick_sort(end_p1 + 1, end);
  }
}


#endif
