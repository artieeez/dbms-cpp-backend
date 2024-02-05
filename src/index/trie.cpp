

#include <cstring>
#include <algorithm>
#include <cctype>
#include <string>
#include <fstream>
#include <iostream>
#include <string>
#include "context.hpp"
#include "trie.hpp"

std::string toLowercase(std::string str)
{
  std::transform(str.begin(), str.end(), str.begin(),
                 [](unsigned char c)
                 { return std::tolower(c); });
  return str;
}
namespace Index
{
  /////////////////////////////////////////////////////////////////////

  Trie::Trie(const std::string &filePath) : filename(filePath)
  {
    Database::Context<TrieNode> dbContext(filename);
    TrieNode parentNode;
    std::streampos first = dbContext.append(parentNode);
    printTrieNode(dbContext.read(first));
  }

  Trie::~Trie()
  {
    Database::Context<TrieNode> dbContext(filename);
    dbContext.reset();
  }

  void Trie::insertString(std::string companyName, std::streampos address)
  {
    std::clog << "insert: " << companyName << "\n";
    std::string companyNameLower = toLowercase(companyName);
    int stringCounter = 1;
    std::streampos currentPosition = 0;
    Database::Context<TrieNode> dbContext(filename);

    for (char ch : companyNameLower)
    {
      int childIndex = ch - 'a';
      Database::Record<Index::TrieNode> currentNode = dbContext.read(currentPosition);

      if (currentNode.value.children[childIndex] == -1)
      {
        TrieNode node;
        std::streampos newPosition = dbContext.append(node);
        currentNode.value.children[childIndex] = newPosition;
        currentNode.value.isLeave = false;

        Database::Record<Index::TrieNode> createdNode = dbContext.read(newPosition);
        createdNode.value.parentAdress = currentPosition;
        currentPosition = newPosition;

        if (stringCounter == companyNameLower.size())
        {
          createdNode.value.address = address;
        }

        dbContext.save(createdNode);
      }
      else
      {
        currentPosition = currentNode.value.children[childIndex];
        if (stringCounter == companyNameLower.size())
        {
          Database::Record<Index::TrieNode> childNode = dbContext.read(currentPosition);
          childNode.value.address = address;
          dbContext.save(childNode);
        }
      }

      dbContext.save(currentNode);
      stringCounter++;
    }
  }

  std::vector<std::streampos> Trie::searchString(std::string companyName)
  {
    std::clog << "search: " << companyName << "\n";
    int stringCounter = 1;
    std::vector<std::streampos> addresses;
    std::streampos currentPosition = 0;
    Database::Context<TrieNode> dbContext(filename);

    for (char ch : companyName)
    {
      int childIndex = ch - 'a';
      Database::Record<Index::TrieNode> currentNode = dbContext.read(currentPosition);

      if (currentNode.value.children[childIndex] != -1)
      {
        if (stringCounter == companyName.size())
        {
          if (currentNode.value.address != -1)
            addresses.push_back(currentNode.value.address);
        }
        currentPosition = currentNode.value.children[childIndex];
      }
      else
      {
        std::clog << "String not found" << std::endl;
        return addresses;
      }
      stringCounter++;
    }
    recursiveSearch(currentPosition, &addresses);
    return addresses;
  }

  void Trie::recursiveSearch(std::streampos currentPosition, std::vector<std::streampos> *addressList)
  {
    Database::Context<TrieNode> dbContext(filename);
    Database::Record<Index::TrieNode> currentNode = dbContext.read(currentPosition);
    if (currentNode.value.address != -1)
      addressList->push_back(currentNode.value.address);
    if (currentNode.value.isLeave == false)
    {
      for (int i = 0; i < MAX_CHILDREN; ++i)
      {
        if (currentNode.value.children[i] != -1)
        {
          recursiveSearch(currentNode.value.children[i], addressList);
        }
      }
    }
  }

  void Trie::deleteString(std::string companyName)
  {
    std::clog << "delete: " << companyName << "\n";
    int stringCounter = 1;
    std::streampos currentPosition = 0;
    Database::Context<TrieNode> dbContext(filename);

    for (char ch : companyName)
    {
      int childIndex = ch - 'a';
      Database::Record<Index::TrieNode> currentNode = dbContext.read(currentPosition);

      if (currentNode.value.children[childIndex] != -1)
      {
        if (stringCounter == companyName.size())
        {
          currentNode.value.address = -1;
          currentNode.deleted = true;
          dbContext.save(currentNode);
          return;
        }
        else
        {
          currentPosition = currentNode.value.children[childIndex];
        }
      }
      else
      {
        std::cout << "String not found" << std::endl;
        return;
      }
      stringCounter++;
    }
  }

  void Trie::printTrieNode(Database::Record<Index::TrieNode> currentNode)
  {

    std::clog << "Node address: " << currentNode.value.address << "\n";
    std::clog << "Node parent: " << currentNode.value.parentAdress << "\n";
    for (int i = 0; i < MAX_CHILDREN; ++i)
    {
      if (currentNode.value.children[i] != -1)
        std::clog << "\nChild " << i << ": " << currentNode.value.children[i] << "\n";
    }
    std::clog << "=============================================================="
              << "\n";
  }
};
