#include "linearSearchController.hpp"
#include "context.hpp"
#include "stock.hpp"
#include "stockPrice.hpp"
#include <array>
#include <cassert>
#include <fstream>
#include <queue>
#include <string>
#include <vector>

namespace Controller
{

    namespace LinearSearch
    {

        void addStock(Model::Stock payload)
        {
            Database::Context<Model::Stock> dbContext(Database::PATH::DB::STOCK);
            dbContext.append(payload);
        }

        void deleteStock(char stockId[MAX_SIZE_STOCK])
        {
            Database::Context<Model::Stock> dbContext(Database::PATH::DB::STOCK);
            std::vector<Database::Record<Model::Stock>> stocks = dbContext.find([stockId](Database::Record<Model::Stock> record)
                                                                                { return strcmp(record.value.stockId, stockId) == 0; });

            if (stocks.size() > 0)
            {
                dbContext.remove(stocks[0].position);
            }
        }

        void updateStock(Model::Stock payload)
        {
            Database::Context<Model::Stock> dbContext(Database::PATH::DB::STOCK);
            std::vector<Database::Record<Model::Stock>> stocks = dbContext.find([payload](Database::Record<Model::Stock> record)
                                                                                { return strcmp(record.value.stockId, payload.stockId) == 0; });

            if (stocks.size() > 0)
            {
                Database::Record<Model::Stock> stock = stocks[0];
                stock.value = payload;
                dbContext.save(stock);
            }
        }

        Model::Stock getStock(char stockId[MAX_SIZE_STOCK])
        {
            Database::Context<Model::Stock> dbContext(Database::PATH::DB::STOCK);
            std::vector<Database::Record<Model::Stock>> stocks = dbContext.find([stockId](Database::Record<Model::Stock> record)
                                                                                { return strcmp(record.value.stockId, stockId) == 0; });

            if (stocks.size() > 0)
            {
                return stocks[0].value;
            }

            return Model::Stock();
        }

        std::vector<Model::Stock> getStockList(std::string prefix, int page, int pageSize, std::string orderBy)
        {
            std::string prefixStr(prefix);
            Database::Context<Model::Stock> dbContext(Database::PATH::DB::STOCK);
            std::vector<Database::Record<Model::Stock>> records = dbContext.find(
                [prefixStr](Database::Record<Model::Stock> record)
                {
                    return strncmp(record.value.stockId, prefixStr.c_str(), MAX_SIZE_STOCK) == 0;
                },
                pageSize, page);

            // map records to stocks
            std::vector<Model::Stock> result;
            for (Database::Record<Model::Stock> record : records)
            {
                result.push_back(record.value);
            }

            return result;
        }

        void addStockPrice(Model::StockPrice payload)
        {
            Database::Context<Model::StockPrice> dbContext(Database::PATH::DB::STOCK_PRICE);
            dbContext.append(payload);
        }

        void deleteStockPrice(std::string stockPriceId)
        {
            Database::Context<Model::StockPrice> dbContext(Database::PATH::DB::STOCK_PRICE);
            std::vector<Database::Record<Model::StockPrice>> stockPrices = dbContext.find([stockPriceId](Database::Record<Model::StockPrice> record)
                                                                                          { return record.value.stockPriceId == stockPriceId; });

            if (stockPrices.size() > 0)
            {
                dbContext.remove(stockPrices[0].position);
            }
        }

        void updateStockPrice(Model::StockPrice payload)
        {
            Database::Context<Model::StockPrice> dbContext(Database::PATH::DB::STOCK_PRICE);
            std::vector<Database::Record<Model::StockPrice>> stockPrices = dbContext.find([payload](Database::Record<Model::StockPrice> record)
                                                                                          { return record.value.stockPriceId == payload.stockPriceId; });

            if (stockPrices.size() > 0)
            {
                Database::Record<Model::StockPrice> stockPrice = stockPrices[0];
                stockPrice.value = payload;
                dbContext.save(stockPrice);
            }
        }

        std::vector<Model::StockPrice> getStockPriceList(char stockId[MAX_SIZE_STOCK], std::string minDate, std::string maxDate, std::string orderBy)
        {

            Database::Context<Model::StockPrice> dbContext(Database::PATH::DB::STOCK_PRICE);
            std::vector<Database::Record<Model::StockPrice>> records = dbContext.find([stockId](Database::Record<Model::StockPrice> record)
                                                                                      { return record.value.stockId == stockId; });

            // map records to stock prices
            std::vector<Model::StockPrice> result;
            for (Database::Record<Model::StockPrice> record : records)
            {
                result.push_back(record.value);
            }

            sortStockPriceList(result);

            return result;
        }

        void sortStockPriceList(std::vector<Model::StockPrice> &stockPriceList)
        {
            std::array<std::queue<Model::StockPrice>, 10> buckets;

            // sort by day
            for (int i = 0; i < 2; i++)
            {
                for (auto sPrice : stockPriceList)
                {
                    std::string date(sPrice.date);
                    buckets.at(date.at(9 - i) - '0').push(sPrice);
                }

                int p = 0;
                for (int j = 0; j < 10; j++)
                {
                    while (!buckets.at(j).empty())
                    {
                        stockPriceList.at(p) = buckets.at(j).front();
                        buckets.at(j).pop();
                        p++;
                    }
                }
            }

            // sort by month
            for (int i = 0; i < 2; i++)
            {
                for (auto sPrice : stockPriceList)
                {
                    std::string date(sPrice.date);
                    buckets.at(date.at(6 - i) - '0').push(sPrice);
                }

                int p = 0; // position in vector
                for (int j = 0; j < 10; j++)
                {
                    while (!buckets.at(j).empty())
                    {
                        stockPriceList.at(p) = buckets.at(j).front();
                        buckets.at(j).pop();
                        p++;
                    }
                }
            }

            // sort by year
            for (int i = 0; i < 4; i++)
            {
                for (auto sPrice : stockPriceList)
                {
                    std::string date(sPrice.date);
                    buckets.at(date.at(3 - i) - '0').push(sPrice);
                }

                int p = 0; // position in vector
                for (int j = 0; j < 10; j++)
                {
                    while (!buckets.at(j).empty())
                    {
                        stockPriceList.at(p) = buckets.at(j).front();
                        buckets.at(j).pop();
                        p++;
                    }
                }
            }
        }

    } // namespace LinearSearch

} // namespace Controller
