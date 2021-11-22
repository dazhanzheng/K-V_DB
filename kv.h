/*-----------------------------
	
    Github:dazhanzheng
    Email:1365950682@qq.com

-------------------------------*/
#ifndef KV_H
#define KV_H

#include<iostream>
#include"hash.h"

class KVDBHandler{

private:

    char* PATH;
    int fd;
    HashMap* map;
    int make_index();

public:
    
    //API1
    KVDBHandler(const std::string& db_file);
    ~KVDBHandler();

    //API2
    int get(const std::string& key, std::string& value) const;
    int set(const std::string& key,const std::string& value);
    int del(const std::string& key);
    
    //API3
    int merge();
};

#endif

