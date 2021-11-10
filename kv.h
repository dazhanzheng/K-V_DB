/*-----------------------------
	
    github:dazhanzheng
    email:1365950682@qq.com

-------------------------------*/

#include<iostream>
#include<unistd.h>
#include<fcntl.h>


class KVDBHandler{

private:
    char* PATH;
    int fd;
    int make_index();
public:
    
    //API1
    KVDBHandler(const std::string& db_file);
    ~KVDBHandler();

    //API2
    int get(const std::string& key, std::string& value) const;
    int check(const std::string& key) const;
    int set(const std::string& key,const std::string& value);
    int del(const std::string& key);
    
    //API3
    int merge();
};



