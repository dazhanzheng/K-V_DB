#include <iostream>

// File Handler for KV-DB
class KVDBHandler
{
private:
    // File Descriptor
    int fd = -1;
    // Constructor, creates DB handler 
    // @param db_name {const std::string&} path of the append-only file for database.
    KVDBHandler(const std::string& db_name);
    
    // Close the DB handler
    ~KVDBHandler();

};