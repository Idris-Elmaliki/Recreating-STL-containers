#include <iostream>

template<typename T, size_t S> 
class array {
    T m_arr[S] = {};  
    size_t m_size = S; 
public: 
    array(std::initializer_list<T> list) {
        if(list.size() > S)
            exit(1); 
        
        size_t i = 0; 
        for(auto& data : list) {
            m_arr[i] = data; 
            i++; 
        }
    }

    T& at(const size_t& i) {
        return m_arr[i]; 
    }

    constexpr size_t size() const {
        return m_size; 
    }

    T front() {
        return m_arr[0]; 
    }

    T back() {
        return m_arr[S-1]; 
    }

    bool empty() {
        if(S == 0)
            return true; 
        
        return false; 
    }

    T& operator[](const size_t& i) {
        return m_arr[i]; 
    }

    constexpr T operator[](const size_t& i) const {
        return m_arr[i]; 
    }

    bool operator==(const array& other) {
        if(this->m_size != other.m_size)
            return false; 
        
        for(int i = 0; i < this->size(); i++) {
            if(this->m_arr[i] != other.m_arr[i])
                return false; 
        }

        return true; 
    }

    class Iterator {
        T* ptr; 

    public: 
        Iterator(T* ptr) 
        : ptr(ptr)
        {}

        T& operator*() {
            return *ptr; 
        }

        T operator->() {
            return ptr; 
        }

        Iterator& operator++() {
            ptr++;
            return *this; 
        }

        bool operator==(const Iterator& other) {
            return ptr = other.ptr; 
        }

        bool operator!=(const Iterator& other) {
            return ptr != other.ptr; 
        }
    }; 

    class const_iterator {
        const T* ptr; 

    public: 
        const_iterator(const T* ptr) 
        : ptr(ptr)
        {}

        constexpr const T& operator*() const {
            return *ptr; 
        }

        constexpr T operator->() const {
            return ptr; 
        }

        constexpr const_iterator& operator++() {
            ptr++;
            return *this; 
        }

        bool operator==(const const_iterator& other) {
            return ptr = other.ptr; 
        }

        constexpr bool operator!=(const const_iterator& other) const {
            return ptr != other.ptr; 
        }
    }; 

    Iterator begin() {
        return Iterator(m_arr); 
    }

    constexpr const_iterator begin() const noexcept {
        return const_iterator(m_arr); 
    }

    Iterator end() {
        return Iterator(m_arr + m_size); 
    }

    constexpr const_iterator end() const noexcept {
        return const_iterator(m_arr + m_size); 
    }
}; 

int main(void) {
    const array<int, 5> arr = {1, 2, 3, 4, 5}; 

    for(auto& i : arr)
        std::cout << i << '\n'; 

    std::cin.get(); 
} 