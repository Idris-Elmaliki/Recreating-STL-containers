#include <iostream>

template<typename T, size_t S> 
class array {
    T m_arr[S] = {};  
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
        return S; 
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
}; 