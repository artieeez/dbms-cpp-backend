#include <iostream>
#include "db_context.hpp"
#include <vector>


std::vector<int> v = {43, 5, 2, 1, 87};

int main() {
    // Instantiate the DbContext with a file path
    DatabaseManagementSystem::Database::DbContext<int> dbContext("database");

    // vector of positions
    std::vector<std::streampos> positions;

    // write records to the database
    for (int i = 0; i < v.size(); i++) {
        std::streampos position = dbContext.write(v[i]);
        positions.push_back(position);
    }

    // Log the positions
    for (int i = 0; i < positions.size(); i++) {
        std::cout << "Position: " << positions[i] << std::endl;
    }

    // read records from the database
    for (int i = 0; i < positions.size(); i++) {
        int retrievedRecord = dbContext.read(positions[i]);
        std::cout << "Retrieved record: " << retrievedRecord << std::endl;
    }

    std::cout << std::endl;

    // Delete an element using a postion
    dbContext.remove(positions[0]);

    // Log the records
    for (int i = 0; i < positions.size(); i++) {
        int retrievedRecord = dbContext.read(positions[i]);
        std::cout << "Retrieved record: " << retrievedRecord << std::endl;
    }

    return 0;
}