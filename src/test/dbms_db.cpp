#include <iomanip>  // std::setw
#include <iostream>
#include <vector>

#include "db_context.hpp"

std::vector<int> v = {43, 5, 2, 1, 87};

int main() {
    // -------- Instantiate the DbContext with a file path
    DatabaseManagementSystem::Database::DbContext<int> dbContext("intDb.db");

    // -------- append records to the database
    for (int i = 0; i < v.size(); i++) {
        dbContext.append(v[i]);
    }

    // dbContext.write(77);
    // dbContext.remove(0);
    // dbContext.move(0, 4);

    // -------- Invert the order of the records
    // dbContext.clearIterator();
    // while (dbContext.next() && dbContext.getCurrPosition() < (dbContext.getLastPosition() / 2)){
    //     std::streampos computedPosition = dbContext.getLastPosition() - dbContext.getCurrPosition();
    //     dbContext.move(computedPosition, dbContext.getCurrPosition());
    //     dbContext.write(dbContext.curr, computedPosition);
    // };

    // -------- Remove all records
    // dbContext.clearIterator();
    // while (dbContext.next()) {
    //     dbContext.remove(dbContext.getCurrPosition());
    // };

    // -------- Log the records and positions using the dbContext iterator
    dbContext.clearIterator();
    while (dbContext.next()) {
        std::cout << "Pos: " << std::setw(2) << std::right << dbContext.getCurrPosition()
                  << " Val: " << std::setw(2) << std::right << dbContext.curr << std::endl;
    };

    return 0;
}