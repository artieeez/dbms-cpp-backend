#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cassert>
#include "model/stockPrice.hpp"
#include <queue>
#include <array>

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
 int begin {11};
    int end = {begin};
    Model::StockPrice sPrice;

    sPrice.date = getDateFromLine(line);
    sPrice.stockPriceId = getSymbolFromLine(line);

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


    std::cout << "symbol: " << sPrice.stockPriceId << std::endl;
    std::cout << "adj: " << sPrice.adj << std::endl;
    std::cout << "close: " << sPrice.close << std::endl;
    std::cout << "high: " << sPrice.high << std::endl;
    std::cout << "low: " << sPrice.low << std::endl;
    std::cout << "open: " << sPrice.open << std::endl;
    std::cout << "volume: " << sPrice.volume << std::endl;

    return sPrice;
}

void test_open_file () {
    std::ifstream data_file {path_data_file};

    std::string line;

    // Skip firts line
    std::getline(data_file, line);

    std::string lastStockName;
    std::getline(data_file, lastStockName);
    for (int i = 0; i < 100; i++) {
        getline(data_file, line);
        std::cout << line << std::endl;
        getStockPriceFromLine(line);
    }

    data_file.close();
}

int main() {

    test_open_file();
    return 0;
}