#ifndef TRIE_HPP
#define TRIE_HPP

#include <fstream>
#include <string>
#include "context.hpp"

constexpr int MAX_CHILDREN = 36;
constexpr int MAX_STRING_SIZE = 20;

namespace Index
{
  struct TrieNode
  {
    std::streampos children[MAX_CHILDREN];
    std::streampos  address;
    std::streampos parentAdress;
    bool isLeave;

    TrieNode() : address(-1), parentAdress(-1), isLeave(true)
    {
      for (int i = 0; i < MAX_CHILDREN; ++i)
      {
        children[i] = std::streampos(-1);
      }
    }
  };
  class Trie
  {
  private:
    std::fstream file;
    const std::string filename;
    void recursiveSearch(std::streampos currentPosition, std::vector<std::streampos> *addressList,  int startIndex, int endIndex, int* counterPointer);

  public:
    Trie(const std::string &filePath);
    ~Trie();
    void insertString(char input[MAX_STRING_SIZE], std::streampos  address);
    std::vector<std::streampos> searchString(char input[MAX_STRING_SIZE], int pageSize, int page);
    void deleteString(char input[MAX_STRING_SIZE]);
  };

} // namespace Index
#endif // TRIE_HPP