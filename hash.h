/*-----------------------------
	
    Github:dazhanzheng
    Email:1365950682@qq.com

-------------------------------*/
#ifndef HASH_H
#define HASH_H

#include<iostream>

class HashMap{					//HashMap in KV-DB;
private:

    class HashNode{  				//Internal node struct for hash check;
    public:
	 uint32_t hash_code;		
	 uint32_t offset;
       	 HashNode* next;
	 std::string key;

	 HashNode(){
	    hash_code = 0;
	    offset = 0;
	    next = nullptr;
	    key = "NULL";
	 }
    };

    int fd;
    uint32_t MapSize,Occupancy;		
    HashNode* MapHead;				//Entry of Map;
    
    int expand();				//Check when insert;
						//If the Occupancy is big enough,call expand;

    int make();					//Remake the Map based on MapSize,reset Occupancy;

public:
    HashMap(int);
    ~HashMap();
    
    //API1
    int set( const std::string&, const uint32_t);
    int search( const std::string&, uint32_t* );
    int del( const std::string&);

    //internal hash_value function; 
    unsigned int Hash( const char*) ;		//BKDR hash function;
};

#endif
