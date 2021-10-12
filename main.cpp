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
    PATH = new char[len];
    for(int i=0;i<len;i++){
        PATH[i] = db_file[i];
    }
    /*fd = open(PATH,O_RDWR|O_CREAT|O_APPEND);
    if(fd==-1){
        fd = creat(PATH,0);
        if(fd==-1)
        {
            std::cout << "PATH WRONG!\n";
        }
    }*/
    fp = fopen(PATH,"a+");
}

int KVDBHandler::set(const std::string &key, const std::string &value){
    //no legality check
    const char* c_key = new char[32];
    const char* c_value = new char[256];
    c_key = key.c_str();
    c_value = value.c_str();
    const int len_key = key.length();
    const int len_value = value.length();
    fwrite(&len_key,4,1,fp);
    fwrite(&len_value,4,1,fp);
    fwrite(c_key,len_key,1,fp);
    fwrite(c_value,len_value,1,fp);
    return true;
}

int KVDBHandler::get(const std::string& key, std::string& value) const{
    char* sub_key = new char[32];
    char* sub_value = new char[256];
    fseek(fp,0,SEEK_SET);
    int len_key,len_value;
    do{
        if(fread(&len_key,4,1,fp)!=4){
            std::cout << "read key length succeeded\n";
        }
        else std::cout << "fuckfuckfuck!\n";
        if(fread(&len_value,4,1,fp)!=4){
            std::cout << "read value length succeeded\n";
        }
        if(fread(sub_key,len_key,1,fp)!=len_key){
            std::cout << "read key succeeded\n";
        }
        if((std::string)sub_key == key){
            fread(sub_value,len_value,1,fp);
            std::cout << "read value succeeded\n";
            break;
        }
        else {
            fseek(fp,len_value,SEEK_CUR);
            std::cout << "NOT THIS!!!\n";
        }
    }
    while(1);
    value = (std::string)sub_value;
    return true;
}