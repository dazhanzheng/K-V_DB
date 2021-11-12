/*-----------------------------
	
    Github:dazhanzheng
    Email:1365950682@qq.com

-------------------------------*/

#include<iostream>
#include<unistd.h>
#include"hash.h"
#define IniSize 512


HashMap::HashMap(int filedescripter){		//i
    
    fd = filedescripter;
    Mapsize = IniSize;
    Occupancy = 0;
    MapHead = nullptr;
    make();
}

HashMap::~HashMap(){				//release occupation;
   
    for(int i=0;i<MapSize;i++){
	
	HashNode* ptr = (MapHead+i)->next;
	HashNode* temp = nullptr;
	
	while(ptr != nullptr){
	    temp = ptr;
	    ptr = ptr->next;
	    delete temp;
	}
    }
    delete[] MapHead;
}

int HashMap::expand(){				//call make() to distribute new resources; 
    MapSize *= 2;
    make();
}

int HashMap::make(){
   
     if(MapHead != nullptr){			//if MapHead exist,release;
        for(int i=0;i<MapSize;i++){
	
	    HashNode* ptr = (MapHead+i)->next;
	    HashNode* temp = nullptr;
	
	    while(ptr != nullptr){
	        temp = ptr;
	        ptr = ptr->next;
	        delete temp;
	    }
        }
        delete[] MapHead;
    }

    MapHead = new HashNode[MapSize];		//distribute space in heap;
    //TODO: Traverse whole file and set;
    
}

int HashMap::set(const std::string &key,const uint32_t dstOffset){
    
    uint32_t desthash = Hash(key.c_str());
    uint32_t destIndex = dsthash & (MapSize-1);
    HashNode* ptr = MapHead + destIndex;
    
    while(ptr->next != nullptr){
	if(ptr->next->hash_code == desthash)
	    if(ptr->next->key == key){
	    	ptr->next->offset = dstOffset;
		return true;
	    }
	ptr = ptr->next;
    }
    ptr->next = new HashNode;
    ptr->next->init();    
}

int HashMap::search(const std::string &key,uint32_t* offsetArr,uint32_t* length){
		
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


