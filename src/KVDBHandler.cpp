#include "../include/KVDBHandler.hpp"
#include <unistd.h>
#include <fcntl.h>
#include "../include/log.hpp"

// Constructor, creates DB handler
// @param db_name {const std::string&} path of the append-only file for database.
KVDBHandler::KVDBHandler(const std::string& db_name)
{
    // Open the file
    this->fd = open(db_name.c_str(), O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    // Check if file is opened
    if (this->fd == -1)
    {
        std::cout << "Error opening file" << std::endl;
        log("Error opening file");
    }
    return;
}

// Close the DB handler
KVDBHandler::~KVDBHandler()
{
    // Close the file
    close(this->fd);
    return;
}   