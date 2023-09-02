template <typename T>
class ListIterator;

template <typename T>
struct ListNode {
    T current;
    ListNode* next;
    ListNode* prev;
};

template <typename T>
class LinkedList {
    private:
    size_t _size = 0;
    ListNode<T>* _head = nullptr;
    ListNode<T>* _tail = nullptr;

    public:
    size_t size() { return _size; }
    ListNode<T>* head() { return _head; }
    ListNode<T>* tail() { return _tail; }
    
    ListNode<T>* at(size_t index);
    T get(size_t index);

    void push_back(T value);
    void push_front(T value);
    void insert(size_t index, T value);
    void insert_after(ListNode<T>* node, T value);
    void insert_before(ListNode<T>* node, T value);
    void remove(ListNode<T>* node);
    void remove(size_t index);

    ListIterator<T> iterator();
};

template <typename T>
class ListIterator {
    LinkedList<T>* list;
    ListNode<T>* current_node;

    public:
    ListIterator(LinkedList<T>* list);
    T current() { return this->current_node->current; }
    bool has_current() { return current_node != nullptr; }
    ListNode<T>* next();
    ListNode<T>* prev();
    void insert_after(T value);
    void insert_before(T value);
    void remove();
};

// Linked List

template <typename T>
ListNode<T>* LinkedList<T>::at(size_t index) {
    if (index == 0)
        return this->_head;
    
    if (index >= _size) // TODO: Produce some kind of error
        throw "Index out of bounds";

    ListNode<T>* node = this->_head;
    for (size_t j = 0; j < _size; j++) {
        if (node->next == nullptr) // TODO: Produce some kind of error
            throw "Index out of bounds";
        
        node = node->next;
    }

    return node;
}

template <typename T>
T LinkedList<T>::get(size_t index) {
    ListNode<T>* node = at(index);
    if (node == nullptr) // TODO: Produce some kind of error
        throw "Index out of bounds";

    return node->current;
}

template <typename T>
void LinkedList<T>::push_back(T value) {
    ListNode<T>* node = new ListNode<T> { value, nullptr, this->_tail };
    if (this->_tail)
        this->_tail->next = node;
    else
        this->_head = node;
    this->_tail = node;
    this->_size++;
}

template <typename T>
void LinkedList<T>::push_front(T value) {
    ListNode<T>* node = new ListNode<T> { value, this->_head, nullptr };
    if (this->_head)
        this->_head->prev = node;
    else
        this->_tail = node;
    this->_head = node;
    this->_size++;
}

template <typename T>
void LinkedList<T>::insert(size_t index, T value) {
    if (index == 0) {
        push_front(value);
        return;
    }

    if (index > _size) // TODO: Produce some kind of error
        throw "Index out of bounds";
        
    ListNode<T>* precendant = at(index - 1);
    if (precendant == nullptr) // TODO: Produce some kind of error
        throw "Index out of bounds";

    insert_after(precendant, value);
}

template <typename T>
void LinkedList<T>::insert_after(ListNode<T>* target, T value) {
    ListNode<T>* node = new ListNode<T> { value, target->next, target };

    if (target->next)
        target->next->prev = node;
    else
        this->_tail = node;
    target->next = node;
    
    this->_size++;
}

template <typename T>
void LinkedList<T>::insert_before(ListNode<T>* target, T value) {
    ListNode<T>* node = new ListNode<T> { value, target, target->prev };

    if (target->prev)
        target->prev->next = node;
    else // If it has no elements before it, update the head
        this->_head = node;
    target->prev = node;
    
    this->_size++;
}

template <typename T>
void LinkedList<T>::remove(size_t index) {
    ListNode<T>* node = at(index);
    if (node == nullptr) // TODO: Produce some kind of error
        throw "Index out of bounds";
    remove(node);
}

template <typename T>
void LinkedList<T>::remove(ListNode<T>* node) {
    if (node->next)
        node->next->prev = node->prev;
    else
        this->_tail = node->prev;
    
    if (node->prev)
        node->prev->next = node->next;
    else
        this->_head = node->next;

    node->next = nullptr;
    node->prev = nullptr;
    delete node;
    this->_size--;
}

template <typename T>
ListIterator<T> LinkedList<T>::iterator() {
    return ListIterator(this);
}

// ListIterator

template <typename T>
ListIterator<T>::ListIterator(LinkedList<T>* list) {
    this->list = list;
    this->current_node = list->at(0);
}

template <typename T>
ListNode<T>* ListIterator<T>::next() {
    this->current_node = this->current_node->next;
    return this->current_node;
}

template <typename T>
ListNode<T>* ListIterator<T>::prev() {
    this->current_node = this->current_node->prev;
    return this->current_node;
}

template <typename T>
void ListIterator<T>::insert_after(T value) {
    this->list->insert_after(this->current_node, value);
}

template <typename T>
void ListIterator<T>::insert_before(T value) {
    this->list->insert_after(this->current_node, value);
}

template <typename T>
void ListIterator<T>::remove() {
    ListNode<T>* next_node = this->current_node->next;
    this->list->remove(this->current_node);
    this->current_node = next_node;
}