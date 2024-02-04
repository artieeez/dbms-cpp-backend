#include <iomanip> // std::setw
#include <iostream>
#include <vector>
#include "trie.hpp"


std::vector<int> v = {43, 5, 2, 1, 87};

int main()
{
    std::ofstream logFile("log.txt");
    std::streambuf *oldClog = std::clog.rdbuf(logFile.rdbuf());
    std::clog << "Testando o log\n";
    
    Index::Trie trie("disk_trie.bin");
    trie.insertString("ape", "000");
    std::string search = "ape";
    std::vector<std::string> addresses = trie.searchString(search);

    std::clog << "address"
              << "\n";
    for (std::string a : addresses)
    {
        std::clog << a << "\n";
    }

    std::clog.rdbuf(oldClog);
    return 0;
}