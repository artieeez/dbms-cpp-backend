#include <string>
#include <vector>
#include <queue>
#include <array>
#include <cassert>
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
      std::vector<std::streampos> positions = trie.searchString(stockId, 1, 0);
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
      std::vector<std::streampos> positions = trie.searchString(payload.stockId, 1, 0);
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
      std::vector<std::streampos> positions = trie.searchString(stockId, 1, 0);
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

    std::vector<Model::Stock> getStockList(std::string prefix, int pageSize, int page)
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
      std::vector<std::streampos> positions = trieStockBlock.searchString(payload.stockId, 1, 0);
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
      std::vector<std::streampos> trieStockPricesAddress = trieStockPrices.searchString(stockPriceId, 1, 0);
      // Busca na trie de stockBlock o endereço do bloco que contém o item
      std::vector<std::streampos> trieStockBlockAddress = trieStockBlock.searchString(stockId, 1, 0);

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

    std::vector<Model::StockPrice> getStockPriceList(std::string stockId, int pageSize, int page)
    {
      Database::Context<Model::StockPrice> dbContext(STOCK_PRICE_DB_FILE);
      Index::Trie trieStockBlock(STOCK_BLOCK_TRIE_FILE);
      Index::BlockStorage blockStorage(STOCK_PRICE_BLOCK_FILE);

      // Busca na trie de stockBlock o endereço do bloco que contém o item
      std::vector<std::streampos> trieStockBlockAddress = trieStockBlock.searchString(stockId, pageSize, page);
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
      std::vector<std::streampos> positions = trieStockPrices.searchString(stockPriceId, 1, 0);
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

    void sortStockPriceList(std::vector<Model::StockPrice>& stockPriceList) {
    std::array<std::queue<Model::StockPrice>, 10> buckets;

    // sort by day
    for (int i = 0; i < 2; i++) {
        for (auto sPrice : stockPriceList)
            buckets.at(sPrice.date.at(9 - i) - '0').push(sPrice);

        int p = 0;
        for (int j = 0; j < 10; j++) {
            while (!buckets.at(j).empty()) {
                stockPriceList.at(p) = buckets.at(j).front();
                buckets.at(j).pop();
                p++;
            }
        }
    }

    // sort by month
    for (int i = 0; i < 2; i++) {
        for (auto sPrice : stockPriceList)
            buckets.at(sPrice.date.at(6 - i) - '0').push(sPrice);

        int p = 0; // position in vector
        for (int j = 0; j < 10; j++) {
            while (!buckets.at(j).empty()) {
                stockPriceList.at(p) = buckets.at(j).front();
                buckets.at(j).pop();
                p++;
            }
        }
    }

    // sort by year
    for (int i = 0; i < 4; i++) {
        for (auto sPrice : stockPriceList)
            buckets.at(sPrice.date.at(3 - i) - '0').push(sPrice);

        int p = 0; // position in vector
        for (int j = 0; j < 10; j++) {
            while (!buckets.at(j).empty()) {
                stockPriceList.at(p) = buckets.at(j).front();
                buckets.at(j).pop();
                p++;
            }
        }
    }
}

// return the number of lines read.
int loadDb(int pageSize) {
    std::ifstream fileStockPrice {"bovespa_stocks.csv"};
    assert(fileStockPrice.is_open());

    Database::Context<std::streampos> loaderDb (Controller::IndexSearch::LOADER_DB_FILE_PATH);
    Database::Context<int> lineCountDb (Controller::IndexSearch::LINE_COUNT_DB_FILE_PATH);

    loaderDb.append(0);
    auto recPos = loaderDb.read(0);

    loaderDb.append(0);
    auto recPosLineCount = lineCountDb.read(0);

    std::cout << recPos.value << std::endl;

    fileStockPrice.seekg(recPos.value);
    for (int i = 0; i < pageSize; i++) {
        recPosLineCount.value++;
        Model::StockPrice sPrice;
        std::string line;

        // finished to read the file
        if (fileStockPrice.eof()) {
            recPos.value = fileStockPrice.tellg();
            loaderDb.save(recPos);
            return 0;
        }

        std::getline(fileStockPrice, line);

        sPrice = getStockPriceFromLine(line);

        std::vector<Model::Stock> sList = Controller::IndexSearch::getStockList(sPrice.stockId, 1, 0);

        if (sList.size() == 0) {
            Model::Stock s;
            s.stockId = sPrice.stockId;
            Controller::IndexSearch::addStock(s);
        }

        Controller::IndexSearch::addStockPrice(sPrice);
    }

    recPos.value = fileStockPrice.tellg();

    std::cout << recPos.value << std::endl;

    loaderDb.save(recPos);
    lineCountDb.save(recPosLineCount);

    // check if is end of file
    if (fileStockPrice.eof())
        return 0;

    return recPosLineCount.value;
}

inline std::string getSymbolFromLine(std::string line) {
    int i {11};
    int j {i};
    while (line.at(j) != ',')
        j++;

    return line.substr(i, j - i);
}

inline std::string getDateFromLine(std::string line) {
    return line.substr(0, 10);
}

Model::StockPrice getStockPriceFromLine(std::string line) {
    // string position starts in 11 to skip date
    int begin {11};
    int end = {begin};
    Model::StockPrice sPrice;

    sPrice.date = getDateFromLine(line);
    sPrice.stockId = getSymbolFromLine(line);
    sPrice.stockPriceId = sPrice.stockId + sPrice.date;

    // skip symbol
    while (line.at(end) != ',')
        end++;

    end++;
    begin = end;
    while (line.at(end) != ',')
        end++;
    sPrice.adj = std::stof(line.substr(begin, end - begin));

    end++;
    begin = end;
    while (line.at(end) != ',')
        end++;
    sPrice.close = std::stof(line.substr(begin, end - begin));

    end++;
    begin = end;
    while (line.at(end) != ',')
        end++;
    sPrice.high = std::stof(line.substr(begin, end - begin));

    end++;
    begin = end;
    while (line.at(end) != ',')
        end++;
    sPrice.low = std::stof(line.substr(begin, end - begin));

    end++;
    begin = end;
    while (line.at(end) != ',')
        end++;
    sPrice.open = std::stof(line.substr(begin, end - begin));

    sPrice.volume = std::stof(line.substr(end + 1, line.size() - end - 1));

    /*
    std::cout << "symbol: " << sPrice.stockId << std::endl;
    std::cout << "adj: " << sPrice.adj << std::endl;
    std::cout << "close: " << sPrice.close << std::endl;
    std::cout << "high: " << sPrice.high << std::endl;
    std::cout << "low: " << sPrice.low << std::endl;
    std::cout << "open: " << sPrice.open << std::endl;
    std::cout << "volume: " << sPrice.volume << std::endl;
    */

    return sPrice;
}

  }
}