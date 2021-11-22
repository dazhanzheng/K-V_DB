/*-----------------------------
	
    Github:dazhanzheng
    Email:1365950682@qq.com

-------------------------------*/

#include<iostream>
#include<unistd.h>
#include"hash.h"
#define IniSize 512
#define ExpandRate 0.7

HashMap::HashMap(int fd_ini){		
    fd = fd_ini;
    MapSize = IniSize;
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
    return true;
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
    		
    uint32_t endOF = lseek(fd,0,SEEK_END);	//record the final offset of the file;
    lseek(fd,0,SEEK_SET);
		
    while(lseek(fd,0,SEEK_CUR) < endOF){
       	uint32_t len_key,len_value;
	uint32_t offset_cur;	

 	read(fd,&len_key,sizeof(len_key));	//read necessary parameters to read;
	std::unique_ptr<char[]> c_key(new char[len_key]);
	read(fd,c_key.get(),len_key);			
	const std::string key = c_key.get();		
	offset_cur = lseek(fd,0,SEEK_CUR);

	read(fd,&len_value,sizeof(len_value));

	if(len_value == -1){
	    del(key);
	    lseek(fd,sizeof(uint32_t),SEEK_CUR);
	}
	else{
	    set(key,offset_cur);
	    lseek(fd,len_value,SEEK_CUR);	//jump to the next record;
 	}

	if(Occupancy >= ExpandRate*MapSize)	//when necessary,expand;	
	{
	    expand();
	    return true;
	}
    }
    return true;
}

int HashMap::set(const std::string &key,const uint32_t dstOffset){
    
    uint32_t dsthash = Hash(key.c_str());
    uint32_t destIndex = dsthash & (MapSize-1);
    HashNode* ptr = MapHead + destIndex;
    
    while(ptr->next != nullptr){		//try to find and edit the offset
	if(ptr->next->hash_code == dsthash)
	    if(ptr->next->key == key){
	    	ptr->next->offset = dstOffset;
		return true;			//exit;
	    }
	ptr = ptr->next;
    }
    ptr->next = new HashNode;			//if not found
    ptr->next->hash_code = dsthash;
    ptr->next->offset = dstOffset;
    ptr->next->key = key;
    Occupancy++;
    return true;
}

int HashMap::del(const std::string& key){
    
    uint32_t dsthash = Hash(key.c_str());
    uint32_t destIndex = dsthash & (MapSize-1);
    HashNode* ptr = MapHead + destIndex;
    HashNode* temp = nullptr;
    
    while(ptr->next != nullptr){		//try to find and edit the offset
	if(ptr->next->hash_code == dsthash)
	    if(ptr->next->key == key){
	    	temp = ptr->next;		//exit;
	    	ptr->next = ptr->next->next;
		delete temp;
		return true;
	    }
	ptr = ptr->next;
    }
    
    return false;				//if not found;
}
 
int HashMap::search(const std::string& key,uint32_t* offset){
    uint32_t dsthash = Hash(key.c_str());
    uint32_t destIndex = dsthash & (MapSize-1);
    HashNode* ptr = MapHead + destIndex;
    
    while(ptr->next != nullptr){		//try to find and edit the offset
	if(ptr->next->hash_code == dsthash)
	    if(ptr->next->key == key){
	    	offset = ptr->next->offset;
		return true;			//exit;
	    }
	ptr = ptr->next;
    }
    return false;
}


// BKDR Hash Function
unsigned int HashMap::Hash(const char *str)
{
    unsigned int seed = 131; // 31 131 1313 13131 131313 etc..
    unsigned int hash = 0;
 
    while (*str)
    {
        hash = hash * seed + (*str++);
    }
 
    return (hash & 0x7FFFFFFF);
}


