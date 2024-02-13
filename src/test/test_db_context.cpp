#include <iomanip> // std::setw
#include <iostream>
#include <vector>
//#include "trie.hpp"
#include "logger.hpp"
#include "context.hpp"
#include <string>

std::vector<int> v = {43, 5, 2, 1, 87};

int main()
{
    Logger logger("test_db_context");
    logger.pushScope("MAIN");
    
    // -------- Instantiate the Context with a file path
    Database::Context<int> dbContext("int");

    // -------- append records to the database
    // for (int i = 0; i < v.size(); i++) {
    //     dbContext.append(v[i]);
    // }

    // Read a record from the database
    std::streampos position = dbContext.getFileEnd();
    Database::Record<int> record;
    if (position > sizeof(int)) {
        record = dbContext.read((int)position - sizeof(int));
    }

    logger.log("Record value: " + std::to_string(record.value));
    logger.log("Record position: " + std::to_string(record.position));

    // Update Record
    record.value = 2332;

    // save the updated record to the database
    dbContext.save(record);

    // Delete Record
    dbContext.remove(40); // pos 40 is the second record (5) when using Record<int>

    // append record (should be at position 40, after value 43)
    dbContext.append(5000);

    // -------- Log the records and positions using the dbContext iterator
    dbContext.clearIterator();
    logger.pushScope("Iterating through the database");
    while (dbContext.next()) {
        logger.log("Pos: " + std::to_string(dbContext.getCurrPosition()) + " Node .position: " + std::to_string(dbContext.curr.position) + " Val: " + std::to_string(dbContext.curr.value));
        std::cout << "Pos: " << std::setw(2) << std::right << dbContext.getCurrPosition()
                  << "Node .position: " << std::setw(2) << std::right << dbContext.curr.position
                  << " Val: " << std::setw(2) << std::right << dbContext.curr.value << std::endl;
    };
    logger.popScope();

    logger.pushScope("Some function");
    logger.log("Address is invalid", true);
    logger.popScope();

    logger.log("Final log");

    // -------- Reset the dbContext
    // dbContext.reset();

    return 0;
}