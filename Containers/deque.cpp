#include <iostream>
#include <deque>

class deque {
    static const size_t BLOCK_SIZE = 8;

    struct Block { 
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

        size_t operator++(int) {
            Block temp; 
            temp.currentAmount = this->currentAmount; 
            this->currentAmount += 1; 
            
            return temp.currentAmount; 
        }

        size_t& operator--() {
            currentAmount -= 1; 
            return this->currentAmount;
        }

        size_t operator--(int) {
            Block temp; 
            temp.currentAmount = this->currentAmount; 
            this->currentAmount -= 1;

            return temp.currentAmount; 
        }

        ~Block() {
            delete[] m_arr; 
            std::cout << "Deleted Block Arr\n";
        }
    }; 

    Block** m_map; 

    size_t m_map_capacity; // current size of alloc memory
    size_t m_index_size; // all indexes that have values
    size_t m_first_block_offset; 

    size_t m_map_start; 
    size_t m_map_end;
    
    // allocs the m_map @ the constructor
    inline void AllocateMapArray(const size_t& mapSize = 1) {
        m_map = new Block*[mapSize]; 

        this->m_map_capacity = mapSize; 
        this->m_map_end = mapSize - 1; 
    }

    // copy alloc for m_map 
    // Problem for pop_front lies here!
    inline void ReallocateMapArray() {
        std::cout << "In ReallocateMapArray()\n"; 

        int newMapCapacity = 0;

        if(m_map_capacity == 1) {
            newMapCapacity = 3;
        }
        else {
            newMapCapacity = ReallocMapSize();
        }

        Block** newMap = new Block*[newMapCapacity]; 

        std::cout << "m_map_capacity: " << m_map_capacity << '\n'; 

        size_t left = 0;
        size_t right = newMapCapacity - 1; 
        size_t offset = left + (right - left / 2) - 1; 

        std::cout << "left: " << left << '\n';
        std::cout << "right: " << right << '\n';

        while(left < offset && right > (newMapCapacity - offset) - 1) {
            std::cout << "Entered the loop!\n";
            newMap[left] = nullptr; 
            newMap[right] = nullptr; 

            ++left, --right; 

            std::cout << "left: " << left << '\n';
            std::cout << "right: " << right << '\n';
        }

        for(size_t i = 0; i < m_map_capacity; i++) {
            newMap[offset + i] = m_map[i];  
        }
        delete[] m_map; 

        m_map_start = offset - left;  
        m_map_end = offset + m_map_capacity - 1; 
        m_map_capacity = newMapCapacity;

        m_map = newMap; 

        for(size_t i = 0; i < newMapCapacity; i++) {
            if(newMap[i] == nullptr) {
                std::cout << "nullptr" << '\n'; 
            }
            else {
                std::cout << i << '\n'; 
            }
        }

        std::cout << "m_map_start: " << m_map_start << '\n'; 
    }
    
    // allocs a new block
    inline Block* AllocateNewBlock() {
        return new Block; 
    }

    // copy alloc for a block
    inline Block* AllocateNewBlock(const size_t& indexRange, const int& newData) {
        std::cout << "\nAdding data into the block!\n";

        Block* newBlock = new Block; 

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

    inline constexpr size_t getBlockAmount(const size_t &indexSize) const {
        if(indexSize < BLOCK_SIZE - 1)
            return indexSize; 

        return BLOCK_SIZE - 1; 
    }
 
    inline size_t ReallocMapSize() {
        return m_map_capacity * 2; 
    }

    inline size_t ReallocMapSizeDifference() {
        return m_map_capacity / 2; 
    }
    
public: 
    // default constructor
    deque()
    : m_index_size(0), m_map_start(0), m_first_block_offset(0)
    {
        AllocateMapArray(); 
    }

    // deque(indexSize, arrData)
    deque(const size_t& indexSize, const int& arrData = 0) 
    : m_index_size(indexSize), m_map_start(0), m_first_block_offset(0)
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
    : m_index_size(list.size()), m_map_start(0), m_first_block_offset(0) 
    {
        AllocateMapArray(getMapSize(list.size())); 

        size_t currentMapIndex = 0; 
        Block* newBlock = AllocateNewBlock(); 

        for(const auto& data : list) {
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
        return m_map[m_map_end]->m_arr[m_map[m_map_end]->currentAmount];
    }

    void push_back(const int& newData) {
        std::cout << "m_index_size: " << m_index_size << "\n";
        std::cout << "m_map_start: " << m_map_start << '\n'; 
        std::cout << "m_map_end: " << m_map_end << '\n';

        if(m_index_size == 0) {
            m_map[m_map_start]->m_arr[0] = newData; 
        }
        else if(m_map[m_map_end]->currentAmount == BLOCK_SIZE - 1) {
            std::cout << "Need to realloc\n"; 

            ReallocateMapArray(); 
            m_map[m_map_end]->m_arr[0] = newData; 
        }
        else {
            size_t i = (m_map[m_map_end]->currentAmount); 
            m_map[m_map_end]->m_arr[i] = newData; 

            std::cout << "New Data: " << m_map[m_map_end]->m_arr[i] << '\n';
        }

        ++*(m_map[m_map_end]);
        ++m_index_size; 

        std::cout << "m_map[m_map_end]->currentAmount: " << m_map[m_map_end]->currentAmount << '\n'; 
        std::cout << "m_index_size: " << m_index_size << '\n';

        std::cout << "push_back was sucessful!\n\n"; 
    }

    // void emplace_back(int&& newData) {}

    void pop_back() {
        if(m_index_size == 0)
            return; 

        size_t& index = (m_map[m_map_end]->currentAmount);

        // m_map[m_map_end]->m_arr[index].~T(); <----- [This is for when we include templates]!

        --index; 
        --m_index_size; 

        if(index == 0) {
            delete m_map[m_map_end];
            --m_map_end;
        }
    }

    void push_front(const int& newData) {
        if(m_first_block_offset != 0) {
            size_t& index = (m_map[m_map_start]->currentAmount);

            m_first_block_offset = BLOCK_SIZE - index - 1; 
            m_map[m_map_start]->m_arr[m_first_block_offset] = newData; 
        }
        else {
            std::cout << "Time to realloc!\n";
            ReallocateMapArray();

            m_map[m_map_start] = AllocateNewBlock();
            m_first_block_offset = BLOCK_SIZE - 1; 

            m_map[m_map_start]->m_arr[m_first_block_offset] = newData; 
        }

        ++m_index_size; 
        ++*(m_map[m_map_start]);

        std::cout << "m_map[m_map_start]->currentAmount: " << m_map[m_map_start]->currentAmount << '\n'; 
        std::cout << "m_index_size: " << m_index_size << '\n';

        std::cout << "push_front was sucessful!\n\n"; 
    }

    // void emplace_front(int&& newData)

    void pop_front() {
        if(m_index_size == 0)
            return;

        size_t& index = (m_map[m_map_start]->currentAmount);

        // m_map[m_map_start]->m_arr[index].~T(); <----- [This is for when we include templates]!

        --index; 
        --m_index_size; 
        ++m_first_block_offset; 

        if(index == 0) {
            delete m_map[m_map_start]; 
            ++m_map_start; 
            --m_map_capacity;
        }
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
        if (index >= m_index_size || index < 0) 
            std::out_of_range("deque index out of range");


        size_t abs = m_first_block_offset + index;  
        size_t block = m_map_start + (abs / BLOCK_SIZE); 
        size_t block_index = abs % BLOCK_SIZE; 

        return m_map[block]->m_arr[block_index]; 
    }

    constexpr int operator[](const size_t &index) const {
        if (index >= m_index_size || index < 0)
            std::out_of_range("deque index out of range");

        size_t block = index / BLOCK_SIZE + m_map_start; 
        size_t block_index = index % BLOCK_SIZE; 

        return m_map[block]->m_arr[block_index]; 
    }

    ~deque() {
        for(size_t i = m_map_start; i < m_map_end + 1; i++)  {
            delete m_map[i]; 
            std::cout << "Deleted Block!\n";
        }
        
        delete m_map; 
        std::cout << "Deleted Map!\n";
    }
}; 

int main(void) {
    deque d = {1, 2, 3, 4};  

    d.push_front(-1); 
    d.push_front(5); 

    d.push_back(9);

    for(size_t i = -1; i < d.size(); i++) 
        std::cout << d[i] << "\n\n";

    std::cin.get();
}
