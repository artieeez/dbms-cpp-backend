#include <string>
#include <vector>
#include <queue>
#include <array>
#include "context.hpp"
#include "trie.hpp"
#include "stock.hpp"
#include "blockStorage.hpp"
#include "indexController.hpp"

namespace Controller
{
  namespace IndexSearch
  {
    void addStock(Model::Stock payload)
    {
      Database::Context<Model::Stock> dbContext(STOCK_DB_FILE);
      Index::Trie trie(STOCK_TRIE_FILE);
      std::streampos position = dbContext.append(payload);
      trie.insertString(payload.stockId, position);
    }

    void deleteStock(std::string stockId)
    {
      Database::Context<Model::Stock> dbContext(STOCK_DB_FILE);
      Index::Trie trie(STOCK_TRIE_FILE);
      std::vector<std::streampos> positions = trie.searchString(stockId, 1, 1);
      if (positions.size() > 0)
      {
        std::streampos position = positions[0];
        Database::Record<Model::Stock> stocks = dbContext.read(position);
        if (stocks.value.stockId == stockId)
        {
          dbContext.remove(position);
          trie.deleteString(stockId);
        }
      }
    }

    void updateStock(Model::Stock payload)
    {
      Database::Context<Model::Stock> dbContext(STOCK_DB_FILE);
      Index::Trie trie(STOCK_TRIE_FILE);
      std::vector<std::streampos> positions = trie.searchString(payload.stockId, 1, 1);
      if (positions.size() > 0)
      {
        std::streampos position = positions[0];
        Database::Record<Model::Stock> stock = dbContext.read(position);
        if (stock.value.stockId == payload.stockId)
        {
          stock.value = payload;
          dbContext.save(stock);
        }
      }
    }

    Model::Stock getStock(std::string stockId)
    {
      Database::Context<Model::Stock> dbContext(STOCK_DB_FILE);
      Index::Trie trie(STOCK_TRIE_FILE);
      std::vector<std::streampos> positions = trie.searchString(stockId, 1, 1);
      if (positions.size() > 0)
      {
        std::streampos position = positions[0];
        Database::Record<Model::Stock> stock = dbContext.read(position);
        if (stock.value.stockId == stockId)
        {
          return stock.value;
        }
      }
      return Model::Stock();
    }

    std::vector<Model::Stock> getStockList(std::string prefix, int page, int pageSize)
    {
      Database::Context<Model::Stock> dbContext(STOCK_DB_FILE);
      Index::Trie trie(STOCK_TRIE_FILE);
      std::vector<std::streampos> positions = trie.searchString(prefix, pageSize, page);
      std::vector<Model::Stock> stocks;
      for (std::streampos position : positions)
      {
        Database::Record<Model::Stock> stock = dbContext.read(position);
        stocks.push_back(stock.value);
      }
      return stocks;
    }

    ///////////////////STOCKPRICE////////////////////

    void addStockPrice(Model::StockPrice payload)
    {
      Database::Context<Model::StockPrice> dbContext(STOCK_PRICE_DB_FILE);
      Index::BlockStorage blockStorage(STOCK_PRICE_BLOCK_FILE);
      Index::Trie trieStockBlock(STOCK_BLOCK_TRIE_FILE);
      Index::Trie trieStockPrices(STOCK_PRICE_TRIE_FILE);
      // adiciona no banco
      std::streampos dbPosition = dbContext.append(payload);

      // procura o stockId no trie que armazena endereços de blocos
      std::vector<std::streampos> positions = trieStockBlock.searchString(payload.stockId, 1, 1);
      if (positions.size() > 0)
      {
        // se encontrou, já há um bloco então basta adicionar um item no bloco
        std::streampos triePosition = positions[0];
        blockStorage.insertBlock(triePosition, dbPosition);
      }
      else
      {
        std::streampos blockPosition = blockStorage.startChain(dbPosition);
        trieStockBlock.insertString(payload.stockId, blockPosition);
      }
      trieStockPrices.insertString(payload.stockPriceId, dbPosition);
    }

    void deleteStockPrice(std::string stockPriceId, std::string stockId)
    {
      Database::Context<Model::StockPrice> dbContext(STOCK_PRICE_DB_FILE);
      Index::Trie trieStockBlock(STOCK_BLOCK_TRIE_FILE);
      Index::Trie trieStockPrices(STOCK_PRICE_TRIE_FILE);
      Index::BlockStorage blockStorage(STOCK_PRICE_BLOCK_FILE);

      // Busca na trie de stockPrices o endereço do item no db
      std::vector<std::streampos> trieStockPricesAddress = trieStockPrices.searchString(stockPriceId, 1, 1);
      // Busca na trie de stockBlock o endereço do bloco que contém o item
      std::vector<std::streampos> trieStockBlockAddress = trieStockBlock.searchString(stockId, 1, 1);

      // Se encontrou o item na trie de stockPrices
      if (trieStockPricesAddress.size() > 0)
      {
        std::streampos dbAddress = trieStockPricesAddress[0];
        // Se encontrou o bloco na trie de stockBlock
        if (trieStockBlockAddress.size() > 0)
        {
          std::streampos blockAddress = trieStockBlockAddress[0];
          // Remove o item do bloco e salva o novo endereço do bloco
          std::streampos startPositionBlock = blockStorage.removeBlockItem(blockAddress, dbAddress);
          // se o endereco mudou, atualiza na trie
          if (startPositionBlock != blockAddress)
          {
            trieStockBlock.insertString(stockId, startPositionBlock);
          }

          dbContext.remove(dbAddress);
          trieStockPrices.deleteString(stockPriceId);
        }
      }
    }

    std::vector<Model::StockPrice> getStockPriceList(std::string stockId, int page, int pageSize)
    {
      Database::Context<Model::StockPrice> dbContext(STOCK_PRICE_DB_FILE);
      Index::Trie trieStockBlock(STOCK_BLOCK_TRIE_FILE);
      Index::BlockStorage blockStorage(STOCK_PRICE_BLOCK_FILE);

      // Busca na trie de stockBlock o endereço do bloco que contém o item
      std::vector<std::streampos> trieStockBlockAddress = trieStockBlock.searchString(stockId, page, pageSize);
      if (trieStockBlockAddress.size() > 0)
      {
        std::streampos blockAddress = trieStockBlockAddress[0];
        // Busca o bloco que contém os endereços no db
        std::vector<std::streampos> dbAddressess = blockStorage.retrieveBlock(blockAddress);
        // Busca os itens no db e salva em stockPrices
        std::vector<Model::StockPrice> stockPrices;
        for (std::streampos address : dbAddressess)
        {
          Database::Record<Model::StockPrice> stock = dbContext.read(address);
          stockPrices.push_back(stock.value);
        }
        return stockPrices;
      }
      return std::vector<Model::StockPrice>();
    }

    Model::StockPrice getStockPrice(std::string stockPriceId)
    {
      Database::Context<Model::StockPrice> dbContext(STOCK_PRICE_DB_FILE);
      Index::Trie trieStockPrices(STOCK_PRICE_TRIE_FILE);
      // Busca na trie de stockPrices o endereço do item no db
      std::vector<std::streampos> positions = trieStockPrices.searchString(stockPriceId, 1, 1);
      if (positions.size() > 0)
      {
        std::streampos position = positions[0];
        Database::Record<Model::StockPrice> stockPrice = dbContext.read(position);
        if (stockPrice.value.stockPriceId == stockPriceId)
        {
          return stockPrice.value;
        }
      }
      return Model::StockPrice();
    }

    void resetDb()
    {

      Database::Context<Model::StockPrice> dbContextStock(STOCK_PRICE_DB_FILE);
      Database::Context<Model::Stock> dbContextStockTrie(STOCK_TRIE_FILE);
      Database::Context<Model::Stock> dbContextStockPrice(STOCK_DB_FILE);
      Database::Context<Model::Stock> dbContextStockBlock(STOCK_PRICE_BLOCK_FILE);
      Database::Context<Model::Stock> dbContextStockPriceTrie(STOCK_PRICE_TRIE_FILE);
      Database::Context<Model::Stock> dbContextStockPriceTrieBlock(STOCK_BLOCK_TRIE_FILE);

      dbContextStock.reset();
      dbContextStockPrice.reset();
      dbContextStockBlock.reset();
      dbContextStockPriceTrie.reset();
      dbContextStockPriceTrieBlock.reset();
      dbContextStockTrie.reset();
    }

  }
}