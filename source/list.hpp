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


// Iterator struct
template <typename T>
struct ListIterator {
  using Self              = ListIterator<T>;
  using value_type        = T;
  using pointer           = T*;
  using reference         = T&;
  using difference_type   = ptrdiff_t;
  using iterator_category = std::bidirectional_iterator_tag;


  // operator* - return node value
  T&  operator*()  const {
    if(nullptr == node) {
      throw "Iterator does not point to valid node";
    }
    else {
      return node->value;
    }
  }

  // operator-> - return pointer to node value
  T* operator->() const {
    if(nullptr == node) {
      throw "Iterator does not point to valid node";
    }
    else {
      T* ptr = &node->value;
      return ptr;
    }
  }


  // PREINCREMENT, call: ++it, first increment, then return iterator
  ListIterator<T>& operator++() {
    if(nullptr == node) {
      throw "Iterator does not point to valid node";
    }
    else {
      node = node->next;
      return *this;
    }
  }

  /* POSTINCREMENT (signature distinguishes the iterators), 
                    call:  it++, increment, return old (before-increment) iterator */
  ListIterator<T> operator++(int) {
    if(nullptr == node) {
      throw "Iterator does not point to valid node";
    }
    else {
      ListNode<T>* old_node = node;
      node = node->next;
      return ListIterator<T>{old_node};
    }
  }


  // operator== - check if node pointers refer to the same nodes
  bool operator==(ListIterator<T> const& x) const {
    if (this->node == x.node) {
      return true;
    }
    else {
      return false;
    }
  }

  // operator!= - check if node pointers do not refer to the same nodes
  bool operator!=(ListIterator<T> const& x) const {
    if (! operator==(x)) {
      return true;
    }
    else {
      return false;
    }
  }

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

    // Swap operator
    void swap(List& rhs) {
      std::swap(first_, rhs.first_);
      std::swap(last_,rhs.last_);
    }

    // Unifying assignment operator
    List& operator=(List rhs) {
      swap(rhs);
      size_ = rhs.size_;
      return *this;
    }

    // (In)Equality check: operator== and operator!=
    bool operator==(List const& rhs) const
    {
      if (rhs.size_ == this->size_) {
        // Start with first node of each list
        ListNode<T>* curr_node_this = this->first_;
        ListNode<T>* curr_node_rhs = rhs.first_;
        // Check for every node
        for (unsigned int i = 0; i < this->size_; ++i) {
          if (curr_node_this->value == curr_node_rhs->value) {
            // Set next node as current node
            curr_node_this = curr_node_this->next;
            curr_node_rhs = curr_node_rhs->next;
          }
          // In case of an unequal value, return false
          else {
            return false;
          }
        }
      }
      // In case of unequal list sizes, return false
      else {
        return false;
      }
    }

    // Inequality operator makes use of equality operator
    bool operator!=(List const& rhs) const
    {
      if (! operator==(rhs)) {
        return true;
      }
      else {
        return false;
      }
    }

    // Destructor calls clear function for deleting all elements
    ~List() {
      this->clear();
    }

    /* ... */
    ListIterator<T> begin() {

      //TODO: begin-Method returning an Iterator to the 
      //      first element in the List (Aufgabe 3.11)
      return iterator{first_};
    }

    /* ... */
    ListIterator<T> end() {
      //TODO: end-Method returning an Iterator to element after (!) 
      //      the last element in the List (Aufgabe 3.11)
      return iterator{nullptr};
    }

    // Clear list - delete all elements
    void clear() {
      while(!this->empty()) {
        this->pop_back();
      }
    }


    // Insert element at given position (takes iterator)
    ListIterator<T> insert(T new_element, ListIterator<T> const& pos) {
      // Special case: Insertion before first element
      if (pos == this->begin()) {
        this->push_front(new_element);
        return ListIterator<T>{first_};
      }
      // Special case: Insertion at the end of the list
      else if (pos == this->end())
      {
        this->push_back(new_element);
        return ListIterator<T>{last_};
      }
      // Insert element between two nodes
      ListNode<T>* curr_prev = pos.node->prev;
      ListNode<T>* new_node = new ListNode<T>{new_element};
      curr_prev->next = new_node;
      new_node->prev = curr_prev;
      pos.node->prev = new_node;
      new_node->next = pos.node;
      ++size_;
      return ListIterator<T>{new_node};
    }

    // Erase element at given position (takes iterator)
    ListIterator<T> erase(ListIterator<T>& pos){
      // Cannot delete an element if given iterator refers to nullptr
      if (nullptr == pos.node) {
        throw "Iterator does not point to valid node";
      }
      // Special case: Erase before first element
      if (pos == this->begin()) {
        this->pop_front();
        return ListIterator<T>{first_};
      }
      // Erase element between two nodes
      ListNode<T>* curr_prev = pos.node->prev;
      ListNode<T>* curr_next = pos.node->next;
      curr_prev->next = curr_next;
      curr_next->prev = curr_prev;
      delete pos.node;
      --size_;
      return ListIterator<T>{curr_next};
    }


    // Reverse function - change list order
    void reverse() {

      if(empty()) {
        throw "List is empty";
      }
      // Get list size for for-loop
      int list_size = this->size();
      // Start with first node as current node
      ListNode<T>* curr_node = this->first_;
      // Reverse every node
      for (int i = 0; i < list_size; ++i) {
          // Swap node's next + prev pointer
          std::swap(curr_node->next, curr_node->prev);
          // Set prev node (former next node) to current node
          curr_node = curr_node->prev;
      }
      // Finally, swap first and last pointer of list head
      std::swap(first_, last_);
    }


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

// Free reverse function - return new list with reverse order
template <typename T>
List<T> reverse(List<T> const& to_be_reverted) {
  List<T> result{to_be_reverted};
  result.reverse();
  return result;
}

// Free vector-list comparison
template <typename T>
bool has_same_content(List<T> const& list, std::vector<T> const& vec) {
  // If list and vector have different sizes, return false
  if (list.size() != vec.size()) {
    return false;
  }
  List<T> temp_list{list};
  std::vector<T> temp_vec{vec};
  ListIterator<T> it = temp_list.begin();
  // Check for every list item ...
  for (unsigned int i = 0; i < temp_list.size(); ++i) {
    // ... if the respective vector item has the same value
    if (it.node->value != vec[i]) {
      // If not, they do not have the same content
      return false;
    }
    // Increment iterator
    ++it;
  }
  return true;
}

/* ... */
//TODO: Freie Funktion operator+ (3.10 - Teil 2)


#endif // # define BUW_LIST_HPP
