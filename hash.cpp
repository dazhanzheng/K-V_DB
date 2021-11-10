#include<iostream>
#include<unistd.h>
#include"hash.h"

int HashMap::set(const uint32_t dsthash,const uint32_t dstoffset){
    
    uint32_t dest = dsthash & 0x000001FF;
    HashNode *ptr = &table[dest];
    while(1){
        if(ptr->hash_code == 0){
            ptr->hash_code = dsthash;
            ptr->offset = dstoffset;
            return 0;
        }
        else{
            while(ptr->next!=nullptr){ptr = ptr->next;}
        }
    }
    return 0;
}

int HashMap::search(const uint32_t dsthash,uint32_t* dstoffset){

}


// BKDR Hash Function
unsigned int Hash(char *str)
{
    unsigned int seed = 131; // 31 131 1313 13131 131313 etc..
    unsigned int hash = 0;
 
    while (*str)
    {
        hash = hash * seed + (*str++);
    }
 
    return (hash & 0x7FFFFFFF);
}


