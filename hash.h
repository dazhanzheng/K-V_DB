unsigned int Hash(char *str);		//BKDR hash function;

class HashMap{				//HashMap in KV-DB;
private:

    struct HashNode{
        uint32_t hash_code = 0;
        uint32_t offset = 0;
        HashNode* next = nullptr;
    };

    HashNode* table;
    //initialize a table

public:
    int set(const uint32_t dsthash,const uint32_t dstoffset);
    int search(const uint32_t dsthash,uint32_t* dstoffset);
};

