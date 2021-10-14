//
// Created by dazhanzheng on 2021/10/10.
//

#include<unistd.h>
#include<fcntl.h>
#include<iostream>



class KVDBHandler{
public:
    char* PATH;
    int fd;
    FILE* fp;
    //Constructor, creates DB handler
    //@param db_file {const std::string&} path of the append-only file for database.
    KVDBHandler(const std::string& db_file);
    //Closes DB handler
    ~KVDBHandler(){
        close(fd);
        fclose(fp);
    }
    int get(const std::string& key, std::string& value) const;
    int set(const std::string& key,const std::string& value);
    int del(const std::string& key);
};



int main(){
    std::string key,value;
    std::cin >> key >> value;
    const std::string ke=key,va = value;
    KVDBHandler test("../K-V_DB/test.txt");
    //test.set(ke, va);
    std::string strout;
    test.get(ke, strout);
    std::cout << strout;
    return 0;
}

KVDBHandler::KVDBHandler(const std::string &db_file) {
    int len = db_file.length();
    const char* path = db_file.c_str();
    
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
    //no legality check
    const char* c_key = new char[32];
    const char* c_value = new char[256];
    c_key = key.c_str();
    c_value = value.c_str();
    const int len_key = key.length();
    const int len_value = value.length();
    write(fd,&len_key,4);
    write(fd,&len_value,4);
    write(fd,c_key,len_key);
    write(fd,c_value,len_value);
    return true;
}



int KVDBHandler::get(const std::string& key, std::string& value) const{
    //allocate space for the buffer (XD)
    char* sub_key = new char[32];
    char* sub_value = new char[256];
    int index_endl = lseek(fd,0,SEEK_END);
    lseek(fd,0,SEEK_SET);
    int len_key,len_value,len_result=0;
    do{
        if(read(fd,&len_key,4)==4)
        std::cout << "read len_key succeeded\n";
        if(read(fd,&len_value,4)==4)
        std::cout << "read len_value succeeded\n";
        if(read(fd,sub_key,len_key)==len_key)
        std::cout << "read sub_key succeeded\n";
        if((std::string)sub_key == key){
            if(len_value!=-1){
                if(read(fd,sub_value,len_value)==len_value)
                std::cout << "read sub_value succeeded\n";
                len_result = len_value;
            }
            else{
                len_result = 0;
                lseek(fd,4,SEEK_CUR);
                std::cout << "well, this is empty\n";
            }
        }
        else{
            if(len_value!=-1)
            lseek(fd,len_value,SEEK_CUR);
            else lseek(fd,4,SEEK_CUR);
            std::cout << "NOT THIS!!!\n";
        }
    }
    while(lseek(fd,0,SEEK_CUR)!=index_endl);
    if(len_result!=0){   
        value.assign(sub_value,len_result);
        return true;
    }
    else{
        return false;
    }
}

int KVDBHandler::del(const std::string& key){
    //no legality check
    const char* c_key = new char[32];
    const char* c_value = new char[4];
    c_key = key.c_str();
    const int len_key = key.length();
    const int len_value = -1;
    write(fd,&len_key,4);
    write(fd,&len_value,4);
    write(fd,c_key,len_key);
    write(fd,c_value,4);
    return true;
}