#include <iostream>

class deque {
    static const size_t BLOCK_SIZE = 8;

    class Block { 
    public:
        int* m_arr;  
        size_t currentAmount; 

        Block() 
        : currentAmount(0) {
            m_arr = new int[BLOCK_SIZE]; 
        }

        Block& operator++() {
            currentAmount += 1;
            return *this; 
        }

        Block operator++(int) {
            Block temp; 
            temp.currentAmount = this->currentAmount; 
            ++(*this); 
            
            return temp; 
        }

        Block& operator--() {
            currentAmount -= 1; 
            return *this;
        }

        Block operator--(int) {
            Block temp; 
            temp.currentAmount = this->currentAmount; 
            --(*this);

            return temp; 
        }

        ~Block() {
            delete[] m_arr; 
        }
    }; 

    Block** m_map; 

    size_t m_map_capacity;
    size_t m_index_size;  

    size_t m_map_start; 
    size_t m_map_end;
    
    inline void AllocateMapArray(const size_t& mapSize) {
        m_map = new Block*[mapSize]; 

        this->m_map_capacity = mapSize; 
        this->m_map_end = mapSize - 1; 
    }

    inline void ReallocateMapArray() {
        Block** newMap = new Block*[ReallocMapSize()]; 
        
        size_t left = 0;
        size_t right = m_map_capacity - 1; 
        size_t offset = m_map_capacity / 2; 

        while(left != offset && right != (m_map_capacity - offset)) {
            newMap[left++] = nullptr; 
            newMap[right--] = nullptr; 
        }

        for(size_t i = 0; i < m_map_capacity; i++) 
            newMap[offset + i] = m_map[i];  
        
        delete[] m_map; 

        m_map_start = left; 
        m_map_end = offset + m_map_capacity - 1; 

        m_map = newMap; 
        m_map_capacity = ReallocMapSize(); 
    }
    
    inline Block* AllocateNewBlock() {
        return new Block; 
    }

    inline Block* AllocateNewBlock(const size_t& indexRange, const int& newData) {
        Block* newBlock = new Block; 

        for(size_t i = 0; i < indexRange || newBlock->currentAmount != BLOCK_SIZE; i++) {
            newBlock->m_arr[i] = newData; 
            newBlock++; 
        }

        return newBlock; 
    }

    inline size_t getMapSize(const size_t &indexSize) {
        return (indexSize/BLOCK_SIZE) + 1; 
    }

    size_t ReallocMapSize() {
        return m_map_capacity * 2; 
    }

    size_t ReallocMapSizeDifference() {
        return m_map_capacity / 2; 
    }
    
public: 
    deque()
    : m_map_capacity(1), m_index_size(0), m_map_start(0)
    {
        AllocateMapArray(1); 
    }

    deque(const size_t& indexSize, const int& arrData = 0) 
    : m_index_size(indexSize), m_map_start(0)
    {
        AllocateMapArray(getMapSize(indexSize));  
        
        size_t index = indexSize; 

        for(size_t i = 0; i < m_map_capacity; i++) { 
            Block* temp = AllocateNewBlock(index, arrData);
            m_map[i] = temp; 

            index -= BLOCK_SIZE; 
        }
    }

    deque(std::initializer_list<int> list) 
    : m_index_size(list.size()), m_map_start(0)
    {
        AllocateMapArray(getMapSize(list.size())); 

        size_t currentMapIndex = 0; 
        Block* newBlock = AllocateNewBlock(); 

        for(auto data : list) {
            if(newBlock->currentAmount == BLOCK_SIZE) {
                m_map[currentMapIndex] = newBlock; 
                newBlock = AllocateNewBlock(); 
                currentMapIndex++; 
            }

            newBlock->m_arr[(newBlock->currentAmount)] = data; 
            (*newBlock)++; 
        }

        m_map[currentMapIndex] = newBlock;
    }

    void push_back(const int& newData) {
        if(m_index_size == 0) {
            m_map[m_map_start]->m_arr[0] = newData; 
        }
        else if(m_map[m_map_end]->currentAmount == BLOCK_SIZE) {
            ReallocateMapArray(); 
            m_map[m_map_end]->m_arr[0] = newData; 
        }
        else {
            size_t i = (m_map[m_map_end]->currentAmount); 
            m_map[m_map_end]->m_arr[i] = newData; 
        }

        m_map[m_map_end]++; 
        m_index_size++; 
    }

    void clear() {
        for(size_t i = 0; i < m_map_capacity; i++) {
            Block* arr = m_map[i]; 
            delete[] arr; 
        }
        AllocateMapArray(m_map_capacity); 

        m_index_size = 0; 
    }

    size_t size() {
        return m_index_size; 
    }

    bool empty() {
        if(m_index_size != 0)
            return false; 
        
        return true; 
    }

    int& operator[](const size_t &index) {
        size_t block = (m_index_size + index) / BLOCK_SIZE; 
        size_t block_index = index % BLOCK_SIZE; 

        return m_map[block]->m_arr[block_index]; 
    }

    constexpr int operator[](const size_t &index) const {
        size_t block = (m_index_size + index) / BLOCK_SIZE; 
        size_t block_index = index % BLOCK_SIZE; 

        return m_map[block]->m_arr[block_index]; 
    }

    ~deque() {
        for(size_t i = 0; i < m_map_capacity; i++)  
            delete m_map[i]; 
        
        delete[] m_map; 
    }
}; 

int main(void) {
    deque d = {1, 2, 3, 4};  

    for(size_t i = 0; i < d.size(); i++) 
        std::cout << d[i] << '\n';  

    d.push_back(9); 

    for(size_t i = 0; i < d.size(); i++) 
        std::cout << d[i] << '\n';  

    std::cin.get(); 
}
