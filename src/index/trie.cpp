

#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include "context.hpp"
#include "trie.hpp"

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

  void Trie::insertString(std::string companyName, std::string address)
  {
    int stringCounter = 1;
    std::streampos currentPosition = 0;
    Database::Context<TrieNode> dbContext(filename);

    for (char ch : companyName)
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

        if (stringCounter == companyName.size())
        {
          std::clog << ch << " is the last character\n";
          createdNode.value.address = address;
        }

        dbContext.save(createdNode);
      }
      else
      {
        currentPosition = currentNode.value.children[childIndex];
        if (stringCounter == companyName.size())
        {
          std::clog << ch << " is the last character\n";
          currentNode.value.address = address;
        }
      }

      dbContext.save(currentNode);
      stringCounter++;
      std::clog << ch << " "
                << "Nodo pai==================================================\n";
      printTrieNode(currentNode);
    }
  }

  std::vector<std::string> Trie::searchString(std::string companyName)
  {
    std::vector<std::string> addresses;
    std::streampos currentPosition = 0;
    Database::Context<TrieNode> dbContext(filename);

    for (char ch : companyName)
    {
      int childIndex = ch - 'a';
      Database::Record<Index::TrieNode> currentNode = dbContext.read(currentPosition);

      if (currentNode.value.children[childIndex] != -1)
      {
        if (currentNode.value.address != "")
          addresses.push_back(currentNode.value.address);
        currentPosition = currentNode.value.children[childIndex];
      }
      else
      {
        std::clog << "String not found" << std::endl;
        return addresses;
      }
    }

    recursiveSearch(currentPosition, &addresses);
    return addresses;
  }

  void Trie::recursiveSearch(std::streampos currentPosition, std::vector<std::string> *addressList)
  {
    Database::Context<TrieNode> dbContext(filename);
    Database::Record<Index::TrieNode> currentNode = dbContext.read(currentPosition);
    if (currentNode.value.address != "")
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
          currentNode.value.address = "";
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
