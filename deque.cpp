#include <iostream>
#include <deque>

class deque {
    int** m_map;
    size_t m_size;
    size_t m_index_size;  

    void AllocateMapArray(const size_t& arrSize) {
        m_map = new int*[arrSize]; 
    }
    
    int* AllocateNewArray() {
        return new int[sizeof(int)]; 
    }

    int* AllocateNewArray(const size_t& index_Amount, const int& newData) {
        int* arr = new int[sizeof(int)]; 

        for(int i = 0; i < index_Amount; i++) {
            if(i >= sizeof(int))
                break; 

            arr[i] = newData; 
        }

        return arr; 
    }

    size_t indexSize(const size_t &index_size) {
        return (index_size/sizeof(int))+1; 
    }

public: 
    deque()
    : m_size(1), m_index_size(0) {
        AllocateMapArray(indexSize(1));  
    }

    deque(const size_t& size, const int& arrData = 0) 
    : m_size(indexSize(size)), 
    m_index_size(size) {
        int indexAmount = size;
        
        AllocateMapArray(indexSize(size));  

        for(int i = 0; i < indexSize(size); i++) {
            indexAmount -= sizeof(int);
            m_map[i] = AllocateNewArray(indexAmount, arrData);  
        }
    }

    deque(std::initializer_list<int> list) 
    : m_size(
        (list.size()/sizeof(int))+1
    ), 
    m_index_size(list.size()) 
    {
        AllocateMapArray(indexSize(list.size())); 

        for(int i = 0; i < indexSize(list.size()); i++) 
            m_map[i] = AllocateNewArray();
        
        size_t currentIndex = 0; 
        size_t currentArr = 0;
        for(auto i : list) {
            if(currentIndex == sizeof(int)) {
                currentIndex = 0; 
                ++currentArr; 
            }

            m_map[currentArr][currentIndex] = i; 
            ++currentIndex; 
        }   
    }

    ~deque() {
        for(int i = 0; i < m_size; i++) {
            int* arr = m_map[i]; 
            delete[] arr; 
        }
        delete[] m_map; 
    }
}; 

int main(void) {
    deque dq(7, 10); 

    std::cin.get(); 
}