#include <iostream>

class deque {
    static const size_t BLOCK_SIZE = 8;

    class Block { 
    public:
        int* m_arr;  
        size_t currentAmount; // always set to 0, NOT 1

        Block() 
        : currentAmount(0) {
            m_arr = new int[BLOCK_SIZE]; 
        }

        // These two operations (x++ && ++x) need to be fixed!
        size_t& operator++() {
            currentAmount += 1;
            return this->currentAmount; 
        }

        Block operator++(int) {
            Block temp; 
            temp.currentAmount = this->currentAmount; 
            ++(this->currentAmount); 
            
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

    size_t m_map_capacity; // current size of alloc memory
    size_t m_index_size; // all indexes that have values

    size_t m_map_start; 
    size_t m_map_end;
    
    // allocs the m_map @ the constructor
    inline void AllocateMapArray(const size_t& mapSize) {
        m_map = new Block*[mapSize]; 

        this->m_map_capacity = mapSize; 
        this->m_map_end = mapSize - 1; 
    }

    // copy alloc for m_map 
    inline void ReallocateMapArray() {
        Block** newMap = new Block*[ReallocMapSize()]; 
        
        size_t left = 0;
        size_t right = m_map_capacity - 1; 
        size_t offset = m_map_capacity / 2; 

        while(left != offset && right != (m_map_capacity - offset)) {
            newMap[left] = nullptr; 
            newMap[right] = nullptr; 

            ++left, --right; 
        }

        for(size_t i = 0; i < m_map_capacity; i++) 
            newMap[offset + i] = m_map[i];  
        
        delete[] m_map; 

        m_map_start = left; 
        m_map_end = offset + m_map_capacity - 1; 

        m_map = newMap; 
        m_map_capacity = ReallocMapSize(); 

        for(size_t i = 0; i < m_map_capacity; i++) {
            if(newMap[i] == nullptr) {
                std::cout << "NULL" << '\n'; 
            }
            else {
                std::cout << i << '\n'; 
            }
        }
    }
    
    // allocs a new block
    inline Block* AllocateNewBlock() {
        return new Block; 
    }

    // copy alloc for a block
    inline Block* AllocateNewBlock(const size_t& indexRange, const int& newData) {
        std::cout << "\nAdding data into the block!\n";

        Block* newBlock = new Block; 

        // this for loop condition checks two things:
            // 1.) whether
            // 2.) 
        for(size_t i = 0; i < indexRange || i < BLOCK_SIZE; i++) {
            newBlock->m_arr[i] = newData; 

            std::cout << "New data @ index " << i << ": " << newBlock->m_arr[i] << '\n';
            newBlock++; 
        }

        std::cout << "Done with this block!\n\n";

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
    // default constructor
    deque()
    : m_map_capacity(1), m_index_size(0), m_map_start(0)
    {
        AllocateMapArray(1); 
    }

    // deque(indexSize, arrData)
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

    // deque = {1, 2, 3, ..., n}
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
                ++currentMapIndex; 
            }

            std::cout << "newBlock->currentAmount: " << newBlock->currentAmount << "\n\n";

            newBlock->m_arr[(newBlock->currentAmount)] = data; 
            std::cout << "newBlock->m_arr[(newBlock->currentAmount)]: " << newBlock->m_arr[(newBlock->currentAmount)] << '\n';

            ++(*newBlock); // overloaded operation
        }

        std::cout << '\n';
        m_map[currentMapIndex] = newBlock;
    }

    int front() {
        // we get the first block and the first index (0) of the block
        return m_map[m_map_start]->m_arr[0]; 
    }

    int back() {
        // we get the last block, and the last available index in the block
        return m_map[m_map_end]->m_arr[m_map[m_map_end]->currentAmount - 1];
    }

    void push_back(const int& newData) {
        // something wrong in push_back!!!

        if(m_index_size == 0) {
            m_map[m_map_start]->m_arr[0] = newData; 
        }
        else if(m_map[m_map_end]->currentAmount == BLOCK_SIZE) {
            std::cout << "Need to realloc\n"; 

            ReallocateMapArray(); 
            m_map[m_map_end]->m_arr[0] = newData; 
        }
        else {
            size_t i = (m_map[m_map_end]->currentAmount); 
            m_map[m_map_end]->m_arr[i] = newData; 

            std::cout << "New Data: " << m_map[m_map_end]->m_arr[i] << '\n';
        }

        m_map[m_map_end]->currentAmount++; // fixed for now
        ++m_index_size; 

        std::cout << "m_map[m_map_end]->currentAmount: " << m_map[m_map_end]->currentAmount << '\n'; 
        std::cout << "m_index_size: " << m_index_size << '\n';

        std::cout << "push_back was sucessful!\n\n"; 
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

    size_t max_size() {
        return (m_map_end - m_map_start) * BLOCK_SIZE; 
    }

    bool empty() {
        if(m_index_size != 0)
            return false; 
        
        return true; 
    }

    int& operator[](const size_t &index) {
        std::cout << "m_index: " << m_index_size << "\nindex: " << index << '\n';

        size_t block = (m_index_size + index) / BLOCK_SIZE; 
        size_t block_index = index % BLOCK_SIZE; 

        std::cout << "Address of: " << std::addressof(m_map[block]->m_arr[block_index]) << "\n"; 

        return m_map[block]->m_arr[block_index]; 
    }

    constexpr int operator[](const size_t &index) const {
        size_t block = (m_index_size + index) / BLOCK_SIZE;  
        size_t block_index = index % BLOCK_SIZE; 

        return m_map[block]->m_arr[block_index]; 
    }

    ~deque() {
        for(size_t i = 0; i < m_map_capacity; i++)  {
            std::cout << "Deleted!\n";
            delete m_map[i]; 
        }
        
        delete m_map; 
    }
}; 

int main(void) {
    deque d = {1, 2, 3, 4};  

    for(size_t i = 0; i < d.size(); i++) 
        std::cout << d[i] << "\n\n";

    std::cout << std::endl; 

    d.push_back(9);

}
