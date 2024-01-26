#include <iostream>
#include "db_context.hpp"

int main() {
    // Instantiate the DbContext with a file path
    DatabaseManagementSystem::Database::DbContext<int> dbContext("database.bin");

    // Example record
    int record = 42;

    // Write the record and get the stream position
    std::streampos position = dbContext.write(record);
    std::cout << "Record written at position: " << position << std::endl;

    // Read the record at the specified position
    int retrievedRecord = dbContext.readByStreamPosition(position);
    std::cout << "Retrieved record: " << retrievedRecord << std::endl;

    // Delete the record at the specified position
    dbContext.deleteRecord(position);
    std::cout << "Record at position " << position << " deleted." << std::endl;

    return 0;
}