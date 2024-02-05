#ifndef TRIE_HPP
#define TRIE_HPP

#include <fstream>
#include <string>
#include "context.hpp"

constexpr int MAX_CHILDREN = 26;

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

  public:
    Trie(const std::string &filePath);
    ~Trie();
    void insertString(std::string companyName, std::streampos  address);
    std::vector<std::streampos> searchString(std::string companyName);
    void deleteString(std::string companyName);
    void printTrieNode(Database::Record<Index::TrieNode> currentNode);
    void recursiveSearch(std::streampos currentPosition, std::vector<std::streampos> *addressList);
  };

} // namespace Index
#endif // TRIE_HPP