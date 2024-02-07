

#include <cstring>
#include <algorithm>
#include <cctype>
#include <string>
#include <fstream>
#include <iostream>
#include <string>
#include "context.hpp"
#include "trie.hpp"

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

void printStringList(std::vector<std::streampos> addresses)
{
  std::clog << "Address list:"
            << "\n";
  for (std::streampos a : addresses)
  {
    std::clog << a << "\n";
  }
}

std::string normalizeString(const std::string &str)
{
  std::string normalizedString;

  for (char c : str)
  {
    if (std::isalnum(c))
    { // Se for um caractere alfanumérico
      if (std::islower(c))
      {                                      // Se for minúsculo
        normalizedString += std::toupper(c); // Torna maiúsculo
      }
      else
      { // Se já for maiúsculo ou um dígito
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

  Trie::Trie(const std::string &filePath) : filename(filePath)
  {
    Database::Context<TrieNode> dbContext(filename);
    TrieNode parentNode;
    std::streampos first = dbContext.append(parentNode);
  }

  Trie::~Trie()
  {
    Database::Context<TrieNode> dbContext(filename);
  }

  void Trie::insertString(std::string input, std::streampos address)
  {
    std::clog << "insert: " << input << " " << filename << "\n";
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
  }

  std::vector<std::streampos> Trie::searchString(std::string input, int pageSize, int page)
  {
    std::string inputNormal = normalizeString(input);
    int startIndex = page <= 0 ? page : (page - 1) * pageSize;
    int endIndex = startIndex + pageSize;
    int counterPointer = 0;

    std::clog << "search: " << input << " " << filename << " " << pageSize << " " << page << "\n";
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
        if (stringCounter == input.size())
        {
          if (currentNode.value.address != -1)
          {
            if (startIndex <= counterPointer && counterPointer < endIndex)
            {
              addresses.push_back(currentNode.value.address);
            }
            else if (counterPointer == endIndex)
            {
              return addresses;
            }
            counterPointer++;
          }
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

    recursiveSearch(currentPosition, &addresses, startIndex, endIndex, &counterPointer);
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

  void Trie::deleteString(std::string input)
  {
    std::string inputNormal = normalizeString(input);
    std::clog << "delete: " << input << " " << filename << "\n";
    int stringCounter = 1;
    std::streampos currentPosition = 0;
    Database::Context<TrieNode> dbContext(filename);

    for (char ch : input)
    {
      int childIndex = mapCharToIndex(ch);

      Database::Record<Index::TrieNode> currentNode = dbContext.read(currentPosition);

      if (currentNode.value.children[childIndex] != -1)
      {
        if (stringCounter == input.size())
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
        std::clog << input << " not found" << std::endl;
        return;
      }
      stringCounter++;
    }
  }

  void Trie::printTrieNode(Database::Record<Index::TrieNode> currentNode)
  {

    std::clog << "Node address: " << currentNode.value.address << "\n";
    std::clog << "Node parent: " << currentNode.value.parentAdress << "\n";
    std::clog << "Node is leave: " << currentNode.value.isLeave << "\n";
    for (int i = 0; i < MAX_CHILDREN; ++i)
    {
      if (currentNode.value.children[i] != -1)
        std::clog << "\nChild " << i << ": " << currentNode.value.children[i] << "\n";
    }
    std::clog << "=============================================================="
              << "\n";
  }
};
