#include <iomanip> // std::setw
#include <iostream>
#include <vector>
#include "context.hpp"
#include "trie.hpp"

std::vector<int> v = {43, 5, 2, 1, 87};

int main()
{
    std::ofstream logFile("log.txt");
    std::streambuf *oldClog = std::clog.rdbuf(logFile.rdbuf());
    std::clog << "Testando o log\n";
    
    Index::Trie trie("disk_trie.bin");
    trie.insertString("abc", 0);
    trie.insertString("abd", 1);
    trie.insertString("abc", 3);
    trie.insertString("abcd", 4);

    std::vector<std::streampos> addresses = trie.searchString("abc");
    for (std::streampos  a : addresses)
    {
        std::clog << a << "\n";
    }

    // trie.deleteString("abc");
    // addresses = trie.searchString("abc");
    // for (std::string a : addresses)
    // {
    //     std::clog << a << "\n";
    // }
    
    std::clog.rdbuf(oldClog);
    return 0;
}