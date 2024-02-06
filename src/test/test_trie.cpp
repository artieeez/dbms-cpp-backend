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
    trie.insertString("abc", 100);
    trie.insertString("abcd", 101);
    trie.insertString("ab", 102);
    trie.insertString("abcde", 103);
    trie.insertString("abcdef", 104);
    trie.insertString("abcdg", 105);

    std::clog << "========================\n";
    // deve mostrar 102 100 101
    std::vector<std::streampos> addresses = trie.searchString("ab", 3, 1);
    // deve mostrar 103 104 105
    addresses = trie.searchString("ab", 3, 2);
    std::clog << "========================\n";
    trie.deleteString("abd");
    trie.deleteString("abc"); //100
    trie.deleteString("abcdef"); //104
    std::clog << "========================\n";
    // deve mostrar 102 101 103
    addresses = trie.searchString("ab", 3, 1);
    // deve mostrar 105
    addresses = trie.searchString("ab", 3, 2);

    std::clog.rdbuf(oldClog);
    return 0;
}