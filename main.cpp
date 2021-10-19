//
// Created by dazhanzheng on 2021/10/10.
//


#include<iostream>
#include<unistd.h>
#include<fcntl.h>




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
    }
    // wait to create API2
    //if key not found ,return false;
    int get(const std::string& key, std::string& value) const;
    int check(const std::string& key) const;
    //
    int set(const std::string& key,const std::string& value);
    //
    int del(const std::string& key);
};



int main(){
    std::string key,value;
    std::cin >> key >> value;
    const std::string ke=key,va = value;
    KVDBHandler test("../K-V_DB/test.txt");
    //test.set(ke, va);
    std::string strout;
    //test.del(key);
    if(test.get(ke, strout))
    std::cout << strout;
    else std::cout << "didn't find it" << std::endl;
    return 0;
}

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
    const char* c_key = new char[32];
    const char* c_value = new char[256];
    c_key = key.c_str();
    c_value = value.c_str();

    int32_t len_key = key.length();
    int32_t len_value = value.length();

    write(fd,&len_key,sizeof(len_key));
    write(fd,&len_value,sizeof(len_value));
    write(fd,c_key,len_key);
    write(fd,c_value,len_value);

    return true;
}



int KVDBHandler::get(const std::string& key, std::string& value) const{
    //TODO:change the char* to an RAII class
    char* sub_key = new char[32];
    char* sub_value = new char[256];
    
    int eof = lseek(fd,0,SEEK_END);
    lseek(fd,0,SEEK_SET);
    int32_t len_key,len_value,len_result=0;
    
    while(lseek(fd,0,SEEK_CUR)!=eof){
        /*if(read(fd,&len_key,4)==4)
         *std::cout << "read len_key succeeded\n";

         *if(read(fd,&len_value,4)==4)
         *std::cout << "read len_value succeeded\n";

         *if(read(fd,sub_key,len_key)==len_key)
         *std::cout << "read sub_key succeeded\n";
        */
        read(fd,&len_key,sizeof(len_key));
        read(fd,&len_value,sizeof(len_value));
        read(fd,sub_key,len_key);
        if((std::string)sub_key == key){
            if(len_value!=-1){
                read(fd,sub_value,len_value);
                //std::cout << "read sub_value succeeded\n";
                len_result = len_value;
            }
            else{
                len_result = false;
                lseek(fd,sizeof(int32_t),SEEK_CUR);
                //std::cout << "well, this is empty\n";
            }
        }
        else{
            if(len_value!=-1)
            lseek(fd,len_value,SEEK_CUR);
            else lseek(fd,sizeof(int32_t),SEEK_CUR);
            //std::cout << "NOT THIS!!!\n";
        }
    }
    if(len_result!=false){   
        value.assign(sub_value,len_result);
        return true;
    }
    else{
        return false;
    }
}

int KVDBHandler::check(const std::string& key) const{
    //allocate space for the buffer (XD),but  useless
    char* sub_key = new char[32];
    char* sub_value = new char[256];
    //store the end of file;
    int32_t eof = lseek(fd,0,SEEK_END);
    lseek(fd,0,SEEK_SET);
    int32_t len_key,len_value,len_result=0;
    
    while(lseek(fd,0,SEEK_CUR)!=eof){
        /*if(read(fd,&len_key,4)==4)
         *std::cout << "read len_key succeeded\n";

         *if(read(fd,&len_value,4)==4)
         *std::cout << "read len_value succeeded\n";

         *if(read(fd,sub_key,len_key)==len_key)
         *std::cout << "read sub_key succeeded\n";
        */
        read(fd,&len_key,sizeof(len_key));
        read(fd,&len_value,sizeof(len_value));
        read(fd,sub_key,len_key);
        if((std::string)sub_key == key){
            if(len_value!=-1){
                lseek(fd,len_value,SEEK_CUR);
                //std::cout << "read sub_value succeeded\n";
                len_result = len_value;
            }
            else{
                len_result = false;
                lseek(fd,sizeof(int32_t),SEEK_CUR);
                //std::cout << "well, this is empty\n";
            }
        }
        else{
            if(len_value!=-1)
            lseek(fd,len_value,SEEK_CUR);
            else lseek(fd,sizeof(int32_t),SEEK_CUR);
            //std::cout << "NOT THIS!!!\n";
        }
    }
    if(len_result!=false){
        return true;
    }
    else{
        return false;
    }
}

int KVDBHandler::del(const std::string& key){
    //no legality check
    const char* c_key = new char[32];
    const char* c_value = new char[256];
    c_key = key.c_str();
    const int len_key = key.length();
    const int len_value = -1;
    write(fd,&len_key,sizeof(len_key));
    write(fd,&len_value,sizeof(len_value));
    write(fd,c_key,len_key);
    write(fd,c_value,sizeof(int32_t));
    return true;
}