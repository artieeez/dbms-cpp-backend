#include <iomanip>  // std::setw
#include <iostream>
#include <vector>

#include "db_context.hpp"

std::vector<int> v = {43, 5, 2, 1, 87};

int main() {
    // Instantiate the DbContext with a file path
    DatabaseManagementSystem::Database::DbContext<int> dbContext("intDb.db");

    // write records to the database
    // for (int i = 0; i < v.size(); i++) {
    //     dbContext.write(v[i]);
    // }

    // dbContext.write(77);
    // dbContext.remove(positions[0]);
    // dbContext.move(0, 4);

    // log the records and positions formatted using dbContext iterator
    dbContext.clearIterator();
    while (dbContext.next()) {
        std::cout << "Pos: " << std::setw(2) << std::right << dbContext.getCurrPosition()
                  << " Val: " << std::setw(2) << std::right << dbContext.curr << std::endl;
    };

    return 0;
}