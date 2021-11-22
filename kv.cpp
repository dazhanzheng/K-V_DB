#include<unistd.h>
#include<fcntl.h>
#include<iostream>
#include"kv.h"
#include"hash.h"

KVDBHandler::KVDBHandler(const std::string &db_file) {
    const char* path = db_file.c_str();
    //if not exist,create it and open
    fd = open(path,O_RDWR|O_APPEND);
    if(fd==-1){
        fd = creat(path,0);
        if(fd==-1)
        {
            std::cout << "PATH WRONG!\n";
        }
    }
    map = new HashMap(fd);
}

int KVDBHandler::set(const std::string &key, const std::string &value){
    
    lseek(fd,0,SEEK_END);			 //initialize the offset;
    const char* c_value = value.c_str();	 //convrt const string to const char*;
    const char* c_key = key.c_str();
    
    uint32_t len_key = key.length();
    uint32_t len_value = value.length();
    uint32_t offset;

    write(fd,&len_key,sizeof(len_key));		//write into the file;
    write(fd,c_key,len_key);

    offset = lseek(fd,0,SEEK_CUR); 		//get the index offset;
    
    write(fd,&len_value,sizeof(len_value));
    write(fd,c_value,len_value);
	    
    map.set(key,offset);			//update the map;
    return true;
}



int KVDBHandler::get(const std::string& key, std::string& value) const{
    uint32_t* offset = nullptr;
    uint32_t len_value;
    char* c_value  = value.c_str();

    map.search(key,offset);			//get the offset of value;
    
    read(fd,&len_value,sizeof(len_value));	//read according to offset;
    read(fd,c_value,len_value);
    return true;
}

int KVDBHandler::del(const std::string& key){
    
    lseek(fd,0,SEEK_END);			//initialize the offset;
    const char* c_key = key.c_str();
    
    uint32_t len_key = key.length();
    uint32_t len_value = -1;

    write(fd,&len_key,sizeof(len_key));
    write(fd,c_key,len_key);
    write(fd,&len_value,sizeof(len_value));
    lseek(fd,sizeof(uint32_t),SEEK_CUR);
    
    return true;
}

int KVDBHandler::merge(){
    return true;   
}


KVDBHandler::~KVDBHandler(){
    close(fd);
    delete map;
}
