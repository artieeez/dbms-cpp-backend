#include <string>
#include <vector>
#include <queue>
#include <array>
#include "linearSearchController.hpp"
#include "context.hpp"
#include "trie.hpp"
#include "stock.hpp"
#include "company.hpp"
#include "blockStorage.hpp"

const std::string STOCK_TRIE_FILE = "stock_trie.db";
const std::string STOCK_PRICE_TRIE_FILE = "stock_price_trie.db";
const std::string COMPANY_TRIE_FILE = "company_trie.db";
const std::string STOCK_PRICE_BLOCK_FILE = "stock_price_block.db";

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
      std::streampos position = positions[0];
      Database::Record<Model::Stock> stocks = dbContext.read(position);
      if (stocks.value.stockId == stockId)
      {
        dbContext.remove(position);
        trie.deleteString(stockId);
      }
    }

    void updateStock(Model::Stock payload)
    {
      Database::Context<Model::Stock> dbContext(STOCK_DB_FILE);
      Index::Trie trie(STOCK_TRIE_FILE);
      std::vector<std::streampos> positions = trie.searchString(payload.stockId, 1, 1);
      std::streampos position = positions[0];
      Database::Record<Model::Stock> stock = dbContext.read(position);
      if (stock.value.stockId == payload.stockId)
      {
        stock.value = payload;
        dbContext.save(stock);
      }
    }

    Model::Stock getStock(std::string stockId)
    {
      Database::Context<Model::Stock> dbContext(STOCK_DB_FILE);
      Index::Trie trie(STOCK_TRIE_FILE);
      std::vector<std::streampos> positions = trie.searchString(stockId, 1, 1);
      std::streampos position = positions[0];
      Database::Record<Model::Stock> stock = dbContext.read(position);
      if (stock.value.stockId == stockId)
      {
        return stock.value;
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
      Index::Trie trie(STOCK_PRICE_TRIE_FILE);
      Index::BlockStorage blockStorage(STOCK_PRICE_BLOCK_FILE);
      std::vector<std::streampos> positions = trie.searchString(payload.stockId, 1, 1);
      std::streampos dbPosition = dbContext.append(payload);
      if (positions.size() > 0)
      {
        std::streampos triePosition = positions[0];
        blockStorage.insertBlock(triePosition, dbPosition);
      }
      else
      {
        std::streampos blockPosition = blockStorage.startChain(dbPosition);
        trie.insertString(payload.stockId, blockPosition);
      }
    }
    
    void resetDb()
    {
      Database::Context<Model::Stock> companyDbContext(STOCK_DB_FILE);
      companyDbContext.reset();
    }

  }
}