#include <iostream>
#include <string>
#include <fstream>
#include <cassert>
#include "indexController.hpp"
#include "stockPrice.hpp"

/*
Table 1 - Stocks listed on B3

stock_id | stock_name | min_date | max_date

stock_id:       int (4 bytes)
stock_name:     char [20]
min_date:       char [9]
max_date:       char [9]

------------------------------------------------------------------

Table 2 - Historical price
stock_id (FK) | stock_price_id | date | ajd | close | high | low | open | volume

stock_id:       int (4 bytes)
stock_price_id: int (4 bytes)
date:           char [9]
adj:            float (4 bytes)
close:          float
high:           float
low:            float
open:           float
volume:         unsigned int (4 bytes)
*/

constexpr char path_data_file [] = {"../raw_data/bovespa_stocks.csv"};

void printStockPrice(Model::StockPrice sPrice) {
    std::cout << "sPriceID:\t" << sPrice.stockPriceId << std::endl;
    std::cout << "sId:\t" << sPrice.stockId << std::endl;
    std::cout << "date:\t" << sPrice.date << std::endl;
    std::cout << "adj:\t" << sPrice.adj << std::endl;
    std::cout << "close:\t" << sPrice.close << std::endl;
    std::cout << "high:\t" << sPrice.high << std::endl;
    std::cout << "low:\t" << sPrice.low << std::endl;
    std::cout << "open:\t" << sPrice.open << std::endl;
    std::cout << "volume:\t" << sPrice.volume << std::endl;
}


void test_loader() {
    std::ifstream fRawData {path_data_file};
    std::vector<std::string> vecLines (10);
    assert(fRawData.is_open());

    for (auto& i : vecLines) {
        std::string line;
        std::getline(fRawData, line);
        i = line;
    }

    std::vector<Model::StockPrice> vecStockPrice (vecLines.size());

    for (int i = 0; i < vecStockPrice.size(); i++)
        vecStockPrice.at(i) = Controller::IndexSearch::getStockPriceFromLine(vecLines.at(i));

    Controller::IndexSearch::loadDb(0);
    Controller::IndexSearch::loadDb(10);

    std::vector<Model::StockPrice> sPriceList = Controller::IndexSearch::getStockPriceList(vecStockPrice.at(1).stockId, 10, 0);

    for (auto i : sPriceList)
        printStockPrice(i);
}