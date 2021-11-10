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
}

int KVDBHandler::set(const std::string &key, const std::string &value){
    //NEED: legality check

    //convert const string to const char*
    const char* c_value = value.c_str();
    const char* c_key = key.c_str();
    
    uint32_t len_key = key.length();
    uint32_t len_value = value.length();
    uint32_t hash_code = Hash((char*)c_key);

    write(fd,&hash_code,sizeof(hash_code));
    write(fd,&len_value,sizeof(len_value));
    write(fd,c_value,len_value);

    return true;
}



int KVDBHandler::get(const std::string& key, std::string& value) const{
   //TODO:change the char* to an RAII class
   return false;  
}

int KVDBHandler::check(const std::string& key) const{
   return false;
}

int KVDBHandler::del(const std::string& key){
    //no legality check
    uint32_t hash_code = Hash((char*)key.c_str());

    const int len_key = key.length();
    const int len_value = -1;

    write(fd,&hash_code,sizeof(hash_code));
    write(fd,&len_value,sizeof(len_value));
    write(fd,&hash_code,sizeof(hash_code)); //just 32bit space

    return true;
}

int KVDBHandler::merge(){
    //open an file-temp to store 
    int fd = open("temp.txt",O_RDWR|O_APPEND);
    if(fd==-1){
        fd = creat("temp",0);
        if(fd==-1)
        {
            std::cout << "PATH WRONG!\n";
        }
    } 
    close(fd);
}


KVDBHandler::~KVDBHandler(){
    close(fd);
}
