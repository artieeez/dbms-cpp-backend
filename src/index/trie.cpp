

#include <cstring>
#include <algorithm>
#include <cctype>
#include <string>
#include <fstream>
#include <iostream>
#include <string>
#include "context.hpp"
#include "trie.hpp"
#include "logger.hpp"

extern Logger mainLogger;

bool hasChildren(std::streampos positions[MAX_CHILDREN])
{
  int count = 0;
  for (int i = 0; i < MAX_CHILDREN; ++i)
  {
    if (positions[i] >= 0)
    { // Considerando que um valor válido é um streampos não-negativo
      ++count;
    }
  }
  return count > 0;
}

std::string normalizeString(const char input[MAX_STRING_SIZE])
{
  std::string str(input); // Converte char array para std::string
  std::string normalizedString;

  for (char c : str)
  {
    if (std::isalnum(c))
    {
      if (std::islower(c))
      {
        normalizedString += std::toupper(c);
      }
      else
      {
        normalizedString += c;
      }
    }
  }

  return normalizedString;
}

int mapCharToIndex(char c)
{
  if (std::isalpha(c))
  {                 // Se for uma letra
    return c - 'A'; // Mapeia as letras maiúsculas de A-Z para as posições 0-25
  }
  else if (std::isdigit(c))
  {                      // Se for um dígito
    return c - '0' + 26; // Mapeia os dígitos de 0-9 para as posições 26-35
  }
  else
  {
    return -1; // Caractere inválido
  }
}

namespace Index
{
  /////////////////////////////////////////////////////////////////////

  Trie::Trie(const std::string &filePath) : filename(filePath) {
    mainLogger.pushScope("Trie::Trie");
    Database::Context<TrieNode> dbContext(filename);
    TrieNode parentNode;

    if (dbContext.isEmpty()) {
        mainLogger.log("Creating new trie root node: " + filename);
        dbContext.append(parentNode);
    } else {
        mainLogger.log("Trie root node already exists. Skipping root node creation: " + filename);
    }
    mainLogger.popScope();
}

  Trie::~Trie()
  {
    Database::Context<TrieNode> dbContext(filename);
  }

  void Trie::insertString(char input[MAX_STRING_SIZE], std::streampos address)
  {
    mainLogger.pushScope("Trie::insertString" + std::string(input) + " " + std::to_string(address) + " " + filename);
    std::string inputNormal = normalizeString(input);
    int stringCounter = 1;
    std::streampos currentPosition = 0;
    Database::Context<TrieNode> dbContext(filename);

    for (char ch : inputNormal)
    {
      int childIndex = mapCharToIndex(ch);

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

        if (stringCounter == inputNormal.size())
        {
          createdNode.value.address = address;
        }
        dbContext.save(createdNode);
      }
      else
      {
        currentPosition = currentNode.value.children[childIndex];
        if (stringCounter == inputNormal.size())
        {
          Database::Record<Index::TrieNode> childNode = dbContext.read(currentPosition);
          childNode.value.address = address;
          dbContext.save(childNode);
        }
      }

      dbContext.save(currentNode);
      stringCounter++;
    }
    mainLogger.popScope();
  }

  std::vector<std::streampos> Trie::searchString(char input[MAX_STRING_SIZE], int pageSize, int page)
  {
    std::string inputNormal = normalizeString(input);
    int startIndex = page <= 0 ? page : (page - 1) * pageSize;
    int endIndex = startIndex + pageSize;
    int counterPointer = 0;
    mainLogger.pushScope("Trie::searchString " + std::string(input) + " " + filename + " " + std::to_string(pageSize) + " " + std::to_string(page) + " " + std::to_string(startIndex) + " " + std::to_string(endIndex) + " " + std::to_string(counterPointer));


    int stringCounter = 1;
    std::vector<std::streampos> addresses;
    std::streampos currentPosition = 0;
    Database::Context<TrieNode> dbContext(filename);

    for (char ch : inputNormal)
    {
      int childIndex = mapCharToIndex(ch);

      Database::Record<Index::TrieNode> currentNode = dbContext.read(currentPosition);

      if (currentNode.value.children[childIndex] != -1)
      {
        if (stringCounter == inputNormal.size())
        {
          if (currentNode.value.address != -1)
          {
            if (startIndex <= counterPointer && counterPointer < endIndex)
            {
              addresses.push_back(currentNode.value.address);
            }
            else if (counterPointer == endIndex)
            {

              mainLogger.popScope();
              return addresses;
            }
            counterPointer++;
          }
        }
        currentPosition = currentNode.value.children[childIndex];
      }
      else
      {
        mainLogger.log("String not found", true);
        mainLogger.popScope();
        return addresses;
      }
      stringCounter++;
    }

    mainLogger.pushScope("Trie::searchString - recursiveSearch");
    recursiveSearch(currentPosition, &addresses, startIndex, endIndex, &counterPointer);
    mainLogger.popScope();

    // log adresses found
    mainLogger.pushScope("Trie::searchString - addresses found");
    for (auto address : addresses)
    {
      mainLogger.log("address: " + std::to_string(address));
    }
    if (addresses.size() == 0)
    {
      mainLogger.log("No addresses found");
    }
    mainLogger.popScope();

    mainLogger.popScope();
    return addresses;
  }

  void Trie::recursiveSearch(std::streampos currentPosition, std::vector<std::streampos> *addressList, int startIndex, int endIndex, int *counterPointer)
  {
    Database::Context<TrieNode> dbContext(filename);
    Database::Record<Index::TrieNode> currentNode = dbContext.read(currentPosition);
    if (currentNode.value.address != -1)
    {
      if (startIndex <= *counterPointer && *counterPointer < endIndex)
      {
        addressList->push_back(currentNode.value.address);
      }
      else if (*counterPointer == endIndex)
      {
        return;
      }
      (*counterPointer)++;
    }

    if (currentNode.value.isLeave == false)
    {
      for (int i = 0; i < MAX_CHILDREN; ++i)
      {
        if (currentNode.value.children[i] != -1)
        {
          recursiveSearch(currentNode.value.children[i], addressList, startIndex, endIndex, counterPointer);
        }
      }
    }
  }

  void Trie::deleteString(char input[MAX_STRING_SIZE])
  {
    mainLogger.pushScope("Trie::deleteString " + std::string(input) + " " + filename);
    std::string inputNormal = normalizeString(input);
    int stringCounter = 1;
    std::streampos currentPosition = 0;
    Database::Context<TrieNode> dbContext(filename);

    for (char ch : inputNormal)
    {
      int childIndex = mapCharToIndex(ch);

      Database::Record<Index::TrieNode> currentNode = dbContext.read(currentPosition);

      if (currentNode.value.children[childIndex] != -1)
      {
        if (stringCounter == inputNormal.size())
        {
          currentPosition = currentNode.value.children[childIndex];
          Database::Record<Index::TrieNode> nextNode = dbContext.read(currentPosition);

          nextNode.value.address = -1;
          nextNode.deleted = true;
          dbContext.save(nextNode);
          if (nextNode.value.isLeave == true)
          {
            currentNode.value.children[childIndex] = -1;
            if (hasChildren(currentNode.value.children) == false)
              currentNode.value.isLeave = true;
            dbContext.save(currentNode);
          }
          return;
        }
        else
        {
          currentPosition = currentNode.value.children[childIndex];
        }
      }
      else
      {
        mainLogger.log("Normalized string not found: " + std::string(inputNormal), true);
        mainLogger.popScope();
        return;
      }
      stringCounter++;
    }

    mainLogger.popScope();
  }
};
