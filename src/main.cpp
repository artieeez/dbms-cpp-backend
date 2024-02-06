#include <iostream>
#include <string>
#include <fstream>
#include "controller/linearSearchController.hpp"
#include "model/stockPrice.hpp"

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

constexpr char path_data_file [] = {"./raw_data/bovespa_stocks.csv"};


int main() {
    std::fstream fRawData {path_data_file};
    std::vector<std::string> vecLines (10);

    for (auto& i : vecLines) {
        std::string line;
        std::getline(fRawData, line);
        i = line;
    }


    for (auto s : vecLines)
        std::cout << s << std::endl;

    std::vector<Model::StockPrice> vecStockPrice (vecLines.size());

    for (int i = 0; i < vecStockPrice.size(); i++) {
        vecStockPrice.at(i) = Controller::LinearSearch::getStockPriceFromLine(vecLines.at(i));
        std::cout << "symbol:\t" << vecStockPrice.at(i).stockPriceId << std::endl;
        std::cout << "date:\t" << vecStockPrice.at(i).date << std::endl;
        std::cout << "adj:\t" << vecStockPrice.at(i).adj << std::endl;
        std::cout << "close:\t" << vecStockPrice.at(i).close << std::endl;
        std::cout << "high:\t" << vecStockPrice.at(i).high << std::endl;
        std::cout << "low:\t" << vecStockPrice.at(i).low << std::endl;
        std::cout << "open:\t" << vecStockPrice.at(i).open << std::endl;
        std::cout << "volume:\t" << vecStockPrice.at(i).volume << std::endl;
    }

    return 0;
}