//
// Created by dazhanzheng on 2021/10/10.
//

#include<iostream>
#include"kv.h"

int main(){
    std::string key,value;
    std::cin >> key >> value;
    const std::string ke=key,va = value;
    KVDBHandler test("test.txt");
    //test.set(ke, va);
    std::string strout;
    //test.del(key);
    if(test.get(ke, strout))
    std::cout << strout;
    else std::cout << "didn't find it" << std::endl;
    
    return 0;
}

