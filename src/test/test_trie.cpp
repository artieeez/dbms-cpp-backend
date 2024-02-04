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
     std::clog << "insert: abc\n";
    trie.insertString("abc", "000");
    std::clog << "insert: abd\n";
    trie.insertString("abd", "001");
    std::clog << "insert: abcd\n";
    trie.insertString("abcd", "002");
    std::clog << "insert: ab\n";
    trie.insertString("ab", "003");

    std::clog << "search: ab\n";
    std::vector<std::string> addresses = trie.searchString("ab");
    for (std::string a : addresses)
    {
        std::clog << a << "\n";
    }

    // std::clog << "delete abc 000\n";
    // trie.deleteString("abc");

    // std::clog << "search: abcde\n";
    // addresses = trie.searchString("abcde");
    // for (std::string a : addresses)
    // {
    //     std::clog << a << "\n";
    // }
    
    std::clog.rdbuf(oldClog);
    return 0;
}