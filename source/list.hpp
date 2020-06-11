#ifndef BUW_LIST_HPP
#define BUW_LIST_HPP
#include <cassert>
#include <cstddef>  //ptrdiff_t
#include <iterator> //std::bidirectional_iterator_tag
#include <iostream>

#include <initializer_list>

template <typename T>
class List;

template <typename T>
struct ListNode {
  T         value = T{};
  ListNode* prev = nullptr;
  ListNode* next = nullptr;
};


//TODO: Implementierung der Methoden des Iterators 
//      (nach Vorlesung STL-1 am 09. Juni) (Aufgabe 3.12)
template <typename T>
struct ListIterator {
  using Self              = ListIterator<T>;
  using value_type        = T;
  using pointer           = T*;
  using reference         = T&;
  using difference_type   = ptrdiff_t;
  using iterator_category = std::bidirectional_iterator_tag;


  /* DESCRIPTION  operator*() */
  T&  operator*()  const {
    if(nullptr == node) {
      throw "Iterator does not point to valid node";
    }

    //TODO: remaining implementation of derefenciation of 
    //      iterator using operator* (Aufgabe 3.12 - Teil 1)

  } //call *it

  /* DESCRIPTION  operator->() */
  T* operator->() const {
    if(nullptr == node) {
      throw "Iterator does not point to valid node";
    }

    //TODO: remaining implementation of derefenciation of 
    //      iterator using operator-> (Aufgabe 3.12 - Teil 2)
  }  //call it->method() or it->member


  /* PREINCREMENT, call: ++it, advances one element forward */
  ListIterator<T>& operator++() {
    if(nullptr == node) {
      throw "Iterator does not point to valid node";
    }

    //TODO: Implement Postincrement-Operation for Iterator
    //      (Aufgabe 3.12 - Teil 3)
    
  }

  /* POSTINCREMENT (signature distinguishes the iterators), 
                    call:  it++, advances one element forward*/
  ListIterator<T> operator++(int) {
    if(nullptr == node) {
      throw "Iterator does not point to valid node";
    }

    //TODO: Implement Postincrement-Operation for Iterator
    //      (Aufgabe 3.12 - Teil 4)

  }


  /* ... */
  bool operator==(ListIterator<T> const& x) const {
    //TODO: Implement Equality-Operation for Iterator
    //      (Aufgabe 3.12 - Teil 5)
    // Iterators should be the same if they refer to the same node
    return false;
  } // call it: == it

  /* ... */
  bool operator!=(ListIterator<T> const& x) const {
    //TODO: Implement Inequality-Operation for Iterator  
    //      (Aufgabe 3.12 - Teil 6)
    // Reuse operator==
    return false;
  } // call it: != it

  /* Advances Iterator */
  ListIterator<T> next() const {
    if (nullptr != node) {
      return ListIterator{node->next};
    } else {
      return ListIterator{nullptr};
    }
  }


  ListNode <T>* node = nullptr;
};



template <typename T>
class List {
  public:

    //friend declarations for testing the members   
    template <typename TEST_TYPE>
    friend size_t get_size(List<TEST_TYPE> const& list_to_test);
    template <typename TEST_TYPE>
    friend ListNode<TEST_TYPE>* get_first_pointer(List<TEST_TYPE> const& list_to_test);
    template <typename TEST_TYPE>
    friend ListNode<TEST_TYPE>* get_last_pointer(List<TEST_TYPE> const& list_to_test);

    using value_type      = T;
    using pointer         = T*;
    using const_pointer   = T const*;
    using reference       = T&;
    using const_reference = T const&;
    using iterator        = ListIterator<T>;

    // Default constructor (empty list)
    List():
      size_{0},
      first_{nullptr},
      last_{nullptr} 
      {}

    // Check if list is empty
    // bool empty() const;

    // Deep copy constructor
    List(List<T> const& to_be_copied):
      size_{0},
      first_{nullptr},
      last_{nullptr}
      {
      // Can only copy elements if list to be copied is empty
      if (! to_be_copied.empty() ) {
        /* Create pointer to current node (the next to be copied),
        statrt with the first node */
        ListNode<T>* curr_node = to_be_copied.first_;
        // Push to target list
        this->push_back(curr_node->value);
        // Continiue copying as long as there is a next node
        while (curr_node->next != nullptr)
        {
          curr_node = curr_node->next;
          this->push_back(curr_node->value);
        }
      }
      
    }

    // test and implement:
    // TODO: Move-Konstruktor (Aufgabe 3.9)

    //TODO: Initializer-List Konstruktor (3.10 - Teil 1)
    /* ... */
    // test and implement:
    List(std::initializer_list<T> ini_list) {
      //not implemented yet
    }

    /* ... */
    // test and implement:
    //TODO: (unifying) Assignment operator (Aufgabe 3.6)
    void swap(List& rhs) {
      std::swap(first_, rhs.first_);
      std::swap(last_,rhs.last_);
    }

    List& operator=(List rhs) {
      swap(rhs);
      size_ = rhs.size_;
      return *this;
    }

    /* ... */
    // test and implement:

    bool operator==(List const& rhs) const
    {
      //TODO: operator== (Aufgabe 3.8)
    }

    bool operator!=(List const& rhs) const
    {
      //TODO: operator!= (Aufgabe 3.8)
      // make use of operator==
    }

    // Destructor calls clear function for deleting all elements
    ~List() {
      this->clear();
    }

    /* ... */
    ListIterator<T> begin() {
      //TODO: begin-Method returning an Iterator to the 
      //      first element in the List (Aufgabe 3.11)
      return {};
    }

    /* ... */
    ListIterator<T> end() {
      //TODO: end-Method returning an Iterator to element after (!) 
      //      the last element in the List (Aufgabe 3.11)
      return {};
    }

    // Clear list - delete all elements
    void clear() {
      while(!this->empty()) {
        this->pop_back();
      }
    }


    /* ... */
    //TODO: member function insert (Aufgabe 3.13)

    /* ... */
    //TODO: member function insert (Aufgabe 3.14)

    /* ... */

    //TODO: member function reverse (Aufgabe 3.7 - Teil 1)


    // Add new node at front
    void push_front(T const& element) {
      ListNode<T>* new_node = new ListNode<T>{element};
      
      // If list is not empty, old first el. has to point to its new predecessor
      if (size_ != 0) {
        first_->prev = new_node;
      }
      // If the new node is the only el., it's also the last el.
      else {
        last_ = new_node;
      }
      // New node should point to current first el. as next el.
      new_node->next = first_;
      // List's new first element is the new node
      first_ = new_node;
      ++size_;
    }

    // Add new node at back
    void push_back(T const& element) {
      ListNode<T>* new_node = new ListNode<T>{element};
      new_node->prev = last_;
      if (size_ != 0) {
        last_->next = new_node;
      }
      else {
        first_ = new_node;
      }
      last_ = new_node;
      ++size_;
    }

    // Remove first node
    void pop_front() {
      if(empty()) {
        throw "List is empty";
      }
      if (size_ > 1) {
        first_ = first_->next;
        assert (nullptr != first_->prev);
        delete first_->prev;
        first_->prev = nullptr;
      }
      else {
        assert (nullptr != first_);
        delete first_;
        first_ = nullptr;
        last_ = nullptr;
      }
      
      --size_;
    }

    // Remove last node
    void pop_back() {
      if(empty()) {
        throw "List is empty";
      }
      if (size_ > 1) {
        last_ = last_->prev;
        assert (nullptr != last_->next);
        delete last_->next;
        last_->next = nullptr;
      }
      else {
        assert (nullptr != last_);
        delete last_;
        last_ = nullptr;
        first_ = nullptr;
      }
      --size_;
    }

    // Get first element
    T& front() const {
      if(empty()) {
        throw "List is empty";
      }

      return first_->value;
    }

    // Get last element
    T& back() const {
      if(empty()) {
        throw "List is empty";
      }

      return last_->value;
    }

    // Check if list is empty
    bool empty() const {
      return first_ == nullptr;
    };


    // Return size of list (stored in size_ member var.)
    std::size_t size() const{  
      return size_;
    };


  // list members
  private: 
    std::size_t size_;
    ListNode<T>* first_;
    ListNode<T>* last_;
};

/* ... */
//TODO: Freie Funktion reverse 
//(Aufgabe 3.7 - Teil 2, benutzt Member-Funktion reverse)

/* ... */
//TODO: Freie Funktion operator+ (3.10 - Teil 2)


#endif // # define BUW_LIST_HPP
