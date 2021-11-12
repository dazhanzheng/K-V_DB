/*-----------------------------
	
    Github:dazhanzheng
    Email:1365950682@qq.com

-------------------------------*/

#include<iostream>

class HashMap{					//HashMap in KV-DB;
private:

    class HashNode{  				//Internal node struct for hash check;
        private:
	uint32_t hash_code;		
        uint32_t offset;
        HashNode* next;
	std::string key;
	
	public:
	HashNode(){
	    hash_code = 0;
	    offset = 0;
	    next = nullptr;
	    key = 
	}
    };

    int fd;
    uint32_t MapSize,Occupancy;		
    HashNode* MapHead;				//Entry of Map;
    
    int expand();				//Check when insert;
						//If the Occupancy is big enough,call expand;

    int make();					//Remake the Map based on MapSize,reset Occupancy;

public:
    HashMap(int filedescripter);
    ~HashMap();
    int set( const std::string &key, const uint32_t dstoffset);
    int search( const std::string &key, uint32_t* offset);
     
};

unsigned int Hash(const char* str) ;	//BKDR hash function;


