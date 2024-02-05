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

    // Esse exemplo busca ab, que deve retornar 100, 101 e 102
    // Depois tenta deletar abd que não existe
    // Depois deleta abc, e busca ab novamente, que deve retornar 101 e 102
    std::vector<std::streampos> addresses = trie.searchString("ab");
    trie.deleteString("abd");
    trie.deleteString("abc");
    addresses = trie.searchString("ab");
    
    std::clog.rdbuf(oldClog);
    return 0;
}