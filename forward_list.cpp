#include <iostream>
#include <forward_list>

class Node {
public: 
    size_t data; 
    Node* next; 

    Node(const size_t &data = 0)
    : data(data) {
        next = nullptr;
    }
}; 

class forward_list {
    Node *head, *tail; 
    size_t length = 0; 

    Node* AllocateNewNode() {
        std::cout << "Done\n";
        Node* newNode = new Node;
        return newNode;  
    }

    Node* AllocateNewNode(const size_t& newData) {
        std::cout << "Done\n";
        Node* newNode = new Node(newData); 
        return newNode; 
    }

public:
    forward_list(const size_t& start_capacity = 0)
    {
        if(start_capacity == 0) {
            head = nullptr; 
            tail = nullptr; 
        }
        else {
            head = AllocateNewNode();
            tail = head; 

            for(int i = 1; i < start_capacity; i++) {
                tail->next = AllocateNewNode(); 
                tail = tail->next; 
            }
        }
    }

    forward_list(const size_t& start_capacity, const size_t& start_data)
    { 
        head = AllocateNewNode(start_data);
        tail = head; 

        for(int i = 1; i < start_capacity; i++) {
            tail->next = AllocateNewNode(start_data); 
            tail = tail->next; 
        }
    }

 // need to figure out how to allow initializer_list with the implementation I had with the other 2 constructors
    forward_list(std::initializer_list<int> list) 
    { 
        for(auto& i : list) {
            
        }
    }

    class Iterator {
        Node* ptr; 
    
    public: 
        Iterator(Node* p)
        : ptr(p)
        {}

        size_t& operator*() { 
            return ptr->data; 
        }
        Iterator& operator++() { 
            ptr = ptr->next; 
            return *this; 
        }

        bool operator!=(const Iterator& other) const { 
            return ptr != other.ptr; 
        }
    }; 

    Iterator begin() {
        return Iterator(head); 
    }

    Iterator end() {
        return Iterator(tail->next); 
    }
}; 

int main(void){
    const std::forward_list<std::string> List(3);  

    forward_list list(3, 10); 

    for(auto &i : list)
        std::cout << i << '\n'; 

    std::cin.get(); 
}
