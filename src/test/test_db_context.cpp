#include <iomanip> // std::setw
#include <iostream>
#include <vector>

#include "context.hpp"

std::vector<int> v = {43, 5, 2, 1, 87};

int main() {
    // -------- Instantiate the Context with a file path
    Database::Context<int> dbContext("intDb.db");

    // -------- append records to the database
    for (int i = 0; i < v.size(); i++) {
        dbContext.append(v[i]);
    }

    // Read a record from the database
    Database::Record<int> record = dbContext.read(0);

    // Update Record
    record.value = 100;

    // save the updated record to the database
    dbContext.save(record);

    // -------- Log the records and positions using the dbContext iterator
    dbContext.clearIterator();
    while (dbContext.next()) {
        std::cout << "Pos: " << std::setw(2) << std::right << dbContext.getCurrPosition()
                  << " Val: " << std::setw(2) << std::right << dbContext.curr.value << std::endl;
    };

    return 0;
}