#include <iostream> 

template<typename T>
class list {
    template<typename t>
    class Node 
    {
        public: 
        t m_data; 
        Node<t>* prev; 
        Node<t>* next; 

        Node(const t &data = 0)
        : m_data(data) {
            prev = nullptr;
            next = nullptr;
        }

        template<typename... Args> 
        Node(Args... newData) 
        : m_data(std::forward<Args>(newData)...) {
            prev = nullptr;
            next = nullptr; 
        }
    }; 

    class Iterator 
    {
        Node<T>* ptr; 
    
    public: 
        Iterator(Node<T>* p)
        : ptr(p)
        {}

        T& operator*() { 
            return ptr->m_data; 
        }

        Iterator& operator++() { 
            ptr = ptr->next; 
            return *this; 
        }

        bool operator!=(const Iterator& other) const { 
            return ptr != other.ptr; 
        }
    }; 

private: 
    Node<T> *head, *tail; 
    size_t m_length; 

    inline Node<T>* AllocateNewNode() {
        return new Node<T>();  
    }

    inline Node<T>* AllocateNewNode(const T& newData) {
        return new Node<T>(newData); 
    }

    template<typename... Args>
    inline Node<T>* Move_AllocateNewNode(Args&&... newData) const {
        return new Node<T>(std::forward<Args>(newData)...); 
    }

    inline Node<T>* getNode(const size_t& pos) { 
        if(pos >= m_length)
            exit(EXIT_FAILURE); 
        
        Node<T>* node = head; 

        for(int i = 0; i < pos; i++) 
            node = node->next; 

        return node; 
    }

public:
    list(const size_t& start_capacity = 0)
    {
        if(start_capacity == 0) {
            head = nullptr; 
            tail = nullptr; 
        }
        else {
            head = AllocateNewNode();
            tail = head;

            for(int i = 1; i < start_capacity; i++) {
                Node<T>* newNode = AllocateNewNode(); 
                newNode->prev = tail; 
                tail->next = newNode; 
                tail = tail->next; 
            }
        }

        m_length = start_capacity;
    }

    list(const size_t& start_capacity, const T& start_data)
    { 
        head = Move_AllocateNewNode(start_data);
        tail = head; 

        for(int i = 1; i < start_capacity; i++) {
            Node<T>* newNode = Move_AllocateNewNode(start_data); 
            newNode->prev = tail; 
            tail->next = newNode; 
            tail = tail->next; 
        }

        m_length = start_capacity; 
    }

    list(std::initializer_list<T> list) 
    { 
        head = nullptr; 
        tail = head; 

        for(auto& i : list) {
            if(head == nullptr) {
                head = Move_AllocateNewNode(i); 
                tail = head; 
            } 
            else { 
                Node<T>* newNode = Move_AllocateNewNode(i); 
                newNode->prev = tail; 
                tail->next = newNode; 
                tail = tail->next;
            }
        }

        m_length = list.size(); 
    }

    void push_front(const T& newData) {
        Node<T>* newNode = AllocateNewNode(newData); 

        if(m_length == 0) {
            head = newNode; 
            tail = head; 
        }
        else {
            newNode->next = head; 
            head->prev = newNode; 
            head = newNode;  
        }
        m_length++;
    }

    void push_back(const T& newData) {
        Node<T>* newNode = AllocateNewNode(newData); 

        if(m_length == 0) {
            tail = newNode; 
            head = tail; 
        }
        else {
            tail->next = newNode; 
            tail->prev = tail; 
            tail = tail->next;  
        }

        m_length++; 
    }

    void insert_after(const size_t& pos, const T& newData) {
        if(pos >= m_length)
            exit(EXIT_FAILURE); 
        
        if(pos == m_length-1) {
            push_back(newData);
            return; 
        } 

        Node<T>* newNode = AllocateNewNode(newData);
        Node<T>* temp = getNode(pos); 

        temp->next->prev = newNode; 
        newNode->next = temp->next;
        temp->next = newNode; 
        newNode->prev = temp;

        m_length++;
    }

/* Note:
*  The emplace functions can't register more than one argument into the list
*/
    template<typename... Args>  
    void emplace_back(Args&&... newData) {
        Node<T>* newNode = Move_AllocateNewNode(newData...); 
        
        if(m_length == 0) {
            tail = newNode;
            head = tail;  
        }
        else {
            tail->next = newNode; 
            tail->prev = tail; 
            tail = tail->next; 
        }
        m_length++; 
    }
 
    template<typename... Args>
    void emplace_front(Args&&... newData) {
        Node<T>* newNode = Move_AllocateNewNode(newData...); 
        
        if(m_length == 0) {
            head = newNode; 
            tail = head; 
        }
        else {
            newNode->next = head; 
            head->prev = newNode; 
            head = newNode;
        }

        m_length++; 
    }

    template<typename... Args>
    void emplace_after(const size_t& pos, Args&&... newData) {
        if(pos >= m_length)
            exit(EXIT_FAILURE); 
        else if(pos == m_length-1) {
            emplace_back(newData...);
            return; 
        } 

        Node<T>* newNode = Move_AllocateNewNode(newData...); 
        Node<T>* temp = getNode(pos);
        
        temp->next->prev = newNode; 
        newNode->next = temp->next;
        temp->next = newNode; 
        newNode->prev = temp;   

        m_length++; 
    }

    T& at(const size_t& pos) {
        Node<T>* temp = getNode(pos); 
        return temp->m_data; 
    }

    void pop_front() {
        if(m_length == 0)
            exit(EXIT_FAILURE); 
        else if(m_length == 1) {
            delete head; 
            head = nullptr; 
            tail = nullptr; 
        }
        else {
            Node<T>* temp = head; 

            head = temp->next; 
            head->prev = nullptr;  
            delete temp; 
        }
        m_length--; 
    }

    void pop_back() {
        if(m_length == 0)
            exit(EXIT_FAILURE);
        else if(m_length == 1) {
            delete tail; 
            tail = nullptr;
            head = nullptr;  
        }
        else {
            Node<T>* temp = tail; 
            
            tail = tail->prev; 
            tail->next = nullptr;
            delete temp; 
        }
        m_length--; 
    }

    // needs fixing
    void erase_after(const size_t& pos) {
        if(m_length == 0 || pos >= m_length-1)
            exit(EXIT_FAILURE); 
          
        if(pos == (m_length-2)) 
            return pop_back(); 
        

        Node<T>* previousTemp = getNode(pos); 
        Node<T>* temp = previousTemp->next; 

        previousTemp->next = temp->next; 
        temp->next->prev = previousTemp; 
        std::cout << temp << '\n'; 
        
        delete temp; 
        m_length--; 
    }

    void clear() {
        Node<T>* temp = head; 

        while(temp->next) {
            Node<T>* nextNode = temp->next; 
            delete temp;  
            temp = nextNode; 
        }

        head = nullptr;
        tail = nullptr; 

        m_length = 0; 
    }

    void reassign(const size_t& size, const T& newData) {
        clear(); 

        for(int i = 0; i < size; i++) 
            emplace_back(newData);  
    }

    void resize(const size_t& size, const T& newData = 0) {        
        if(m_length == size)
            return; 
        
        if(m_length == 0) {
            for(int i = 0; i < size; i++)
                emplace_back(newData); 
            
            return;
        }

        Node<T>* temp = head; 
        if(size < m_length) {
            for(int i = 0; i < size; i++)
                temp = temp->next; 

            tail = temp; 

            for(int i = 0; i < m_length-size; i++) {
                Node<T>* nextNode = temp->next; 
                delete temp;  
                temp = nextNode; 
            } 
            m_length = size; 
        }
        else if(size > m_length){
            for(int i = 0; i < m_length; i++)
                temp = temp->next;

            for(int i = 0; i < m_length-size; i++) 
                emplace_back(newData);  
        }
    }

    bool empty() {
        if(head == nullptr && tail == nullptr)
            return true;

        return false; 
    }

    T front() {
        if(m_length != 0)
            return head->m_data; 
        
        exit(EXIT_FAILURE); 
    }

    T back() {
        if(m_length != 0)
            return tail->m_data; 
        
        exit(EXIT_FAILURE);
    }

    inline void operator=(const list& other) {
        clear(); 

        Node<T>* otherTemp = other.head; 
        while(otherTemp) {
            emplace_back(otherTemp->m_data); 
            otherTemp = otherTemp->next;
        }
    }

    bool operator==(const list& other) {
        if(this->m_length != other.m_length)
            return false; 

        Node<T>* thisTemp = this->head; 
        Node<T>* otherTemp = other.head; 

        while(thisTemp) {
            if(thisTemp->m_data != otherTemp->m_data)
                return false; 
            
            thisTemp = thisTemp->next;
            otherTemp = otherTemp->next; 
        }

        return true; 
    }
 
    size_t size() {
        return m_length; 
    }

    Iterator begin() {
        return Iterator(head); 
    }

    constexpr Iterator begin() const {
        return Iterator(head); 
    }

    Iterator end() {
        return Iterator(tail->next); 
    }

    constexpr Iterator end() const {
        return Iterator(tail->next); 
    }

    void print_reverse() {
        Node<T>* temp = tail; 

        while(temp) {
            std::cout << temp->m_data << '\n'; 
            temp = temp->prev; 
        }
    }

    ~list() {
        Node<T>* temp = head; 
        while(head) { 
            head = temp->next; 
            delete temp;
            temp = head; 
        }
    }
}; 