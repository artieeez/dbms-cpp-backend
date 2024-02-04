

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
        currentNode.value.parentAdress = currentPosition;
        currentPosition = newPosition;

        if (stringCounter == companyName.size())
        {
          std::clog << ch << " is the last character\n";
          currentNode.value.address = address;
        }
        dbContext.save(currentNode);
      }
      else
      {
        currentPosition = currentNode.value.children[childIndex];
      }
      
      printTrieNode(currentNode);
      stringCounter++;
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

    return addresses;
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
    std::clog << "================Print do nodo salvo========================"
              << "\n";
    std::clog << "Node position: " << currentNode.position << "\n";
    std::clog << "Node address: " << currentNode.value.address << "\n";
    std::clog << "Node parent: " << currentNode.value.parentAdress << "\n";
    for (int i = 0; i < MAX_CHILDREN; ++i)
    {
      if (currentNode.value.children[i] != -1)
        std::clog << "\nChild " << i << ": " << currentNode.value.children[i] << "\n";
    }
    std::clog << "========================================"
              << "\n";
  }

};
