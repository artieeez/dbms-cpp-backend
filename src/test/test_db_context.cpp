#include <iomanip> // std::setw
#include <iostream>
#include <vector>
//#include "trie.hpp"
#include "context.hpp"

std::vector<int> v = {43, 5, 2, 1, 87};

int main()
{
    std::ofstream logFile("log.txt");
    std::streambuf *oldClog = std::clog.rdbuf(logFile.rdbuf());
    std::clog << "Testando o log\n";
    
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
    while (dbContext.next()) {
        std::cout << "Pos: " << std::setw(2) << std::right << dbContext.getCurrPosition()
                  << "Node .position: " << std::setw(2) << std::right << dbContext.curr.position
                  << " Val: " << std::setw(2) << std::right << dbContext.curr.value << std::endl;
    };

    // -------- Reset the dbContext
    // dbContext.reset();

    // // Restaura std::clog para seu buffer original quando terminar
    std::clog.rdbuf(oldClog);
    return 0;
}