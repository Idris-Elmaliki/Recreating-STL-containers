#include <iostream>

template<class T> 
class vector {
    T* m_data = nullptr; 
    size_t m_size = 0; 
    size_t m_capacity;  

    inline void Copy_Realloc(
        const size_t &newCapacity
    ) { 
        T* newDataArr = new T[newCapacity]; 

        for(int i = 0; i < m_size; i++) 
            newDataArr[i] = m_data[i]; 
        
        delete[] m_data;
        
        m_data = newDataArr; 
        m_capacity = newCapacity; 
    }

    inline void Move_Realloc(
        const size_t &newCapacity
    ) { 
        T* newDataArr = new T[newCapacity]; 

        for(int i = 0; i < m_size; i++) 
            newDataArr[i] = std::move(m_data[i]); 
        
        delete[] m_data;
        
        m_data = newDataArr; 
        m_capacity = newCapacity; 
    }

    inline void ReAlloc_Redef(const vector& other) {
        T* newDataArr = new T[other.m_capacity]; 

        for(int i = 0; i < other.m_capacity; i++)
            newDataArr[i] = other.m_data[i]; 

        delete[] this->m_data; 
        this->m_data = newDataArr; 

        m_capacity = other.m_capacity; 
        m_size = other.m_size; 
    }

    inline size_t ReallocCalculation() {
        return m_capacity + (m_capacity / 2); 
    }

public:
    vector(const size_t &start_capacity = 0) 
    : m_capacity(start_capacity)
    {
        m_data = new T[m_size]; 
    }

    vector(const size_t &start_capacity, const T& start_data) 
    : m_capacity(start_capacity)
    {
        m_size = start_capacity;     
        m_data = new T[m_size]; 
    
        for(int i = 0; i < start_capacity; i++)
            m_data[i] = start_data; 
    }

    void push_back(const T &newData) { 
        if(m_size == m_capacity) 
            Copy_Realloc(ReallocCalculation());     
        
        m_data[m_size] = newData; 
        m_size++; 
    }


    // there isn't a move contrustor that allows for more than one parameter lol
    template<typename... Args> 
    void emplace_back(Args&&... newData) {
        if(m_size == m_capacity) 
            Move_Realloc(ReallocCalculation());     
        
        m_data[m_size] = T(std::forward<Args>(newData)...);
        m_size++; 
    }

    void pop_back() { 
        if(size > 0) {
            m_data[m_size-1].~T();
            m_size--; 
        }
    }

    void clear() {
        for(int i = 0; i < m_size; i++)
            m_data[i].~T(); 

        m_size = 0; 
    }

    void shrink_to_fit() {
        if(m_capacity > m_size)
            Move_Realloc(m_size); 
    }

    void resize(const size_t &newSize, const T &newData = 0) {
        if(newSize > m_size) {
            for(int i = (m_size-1); i < newSize; i++)
                m_data[i] = newData; 
             
            m_size = newSize; 
            m_capacity = m_size; 
        }
        else if(newSize < m_size) {
            while(newSize < m_size) 
                pop_back(); 
        }
    }

    void reserve(const size_t &newCapacity) {
        if(m_size == 0) {
            int* newData = new int[newCapacity]; 
            delete[] m_data;  
            m_data = newData; 
        }
        else {
            Move_Realloc(newCapacity); 
        }
    }

    inline void insert(const size_t &position, const T &newData) {
        if(position > m_size)
            exit(1); 
        
        if(position == (m_size)) {
            emplace_back(std::move(newData)); 
            return; 
        }

        T* newDataArr = new T[ReallocCalculation()]; 

        int currentPosition = 0; 

        for(int i = 0; i < m_size; i++) {
            if(i == position) {
                newDataArr[currentPosition] = newData;   
                ++currentPosition; 
            }
            newDataArr[currentPosition] = m_data[i]; 
            ++currentPosition; 
        }

        delete[] m_data; 
        m_data = newDataArr; 
        m_size++;
    }

    inline T& at(const size_t& i) {
        return m_data[i];   
    }

    size_t size() {
        return m_size; 
    }

    size_t capacity() {
        return m_capacity; 
    }

    size_t front() {
        return m_data[0]; 
    }

    size_t back() {
        return m_data[m_size-1]; 
    }

    bool empty() {
        if(m_capacity == 0)
            return true;

        return false; 
    }

    T& operator[](const size_t &i) {
        if(i >= m_size) 
            exit(1); 

        return m_data[i]; 
    }

    constexpr T operator[](const size_t &i) const {
        if(i > m_size)
            exit(1);

        return m_data[i]; 
    }

    bool operator==(const vector& other) {
        if(this->m_size != other.m_size)
            return false;

        for(int i = 0; i < m_size; i++) {
            if(this->m_data[i] != other.m_data[i])
                return false; 
        }

        return true; 
    }

    inline void operator=(const vector& other) {
        if(this->m_capacity == other.m_capacity) {
            this->m_data = other.m_data; 
        }
        else {
            ReAlloc_Redef(other); 
        }
    }

    ~vector() {
        delete[] m_data; 
    }
}; 