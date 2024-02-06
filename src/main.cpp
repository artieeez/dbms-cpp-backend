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

// get start of next string and its size by reference
void getNextString(std::string line, int &begin, int &size) {
    int i = begin;
    while (line.at(i) != ',' && line.at(i) != '\0' && line.at(i) != '\n') {
        // std::cout << line.at(i) << std::endl;
        i++;
    }
    size = i - begin;
    begin = i;
    std::cout << "\n" << std::endl;
}

Model::StockPrice getStockPriceInfoFromLine(std::string line) {

    // 2010-01-04,MMXM3,1599.1749267578125,1599.1749267578125,1599.1749267578125,1555.1378173828125,1556.39599609375,20984.0

    // string position starts in 11 to skip date
    int begin {11};
    int end = 0;
    int size = 0;
    Model::StockPrice sPrice;

    // skip symbol
    getNextString(line, begin, size);
    std::cout << "begin: " << begin << " size: " << size << std::endl;

    getNextString(line, begin, size);

    std::cout << line.substr(begin - size, size);

    sPrice.close = std::stof(line.substr(begin - size, size - 1));

    return sPrice;

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

    return sPrice;
}

void test_open_file () {
    std::ifstream data_file {path_data_file};

    std::string line;

    // Skip firts line
    std::getline(data_file, line);

    std::string lastStockName;
    std::getline(data_file, lastStockName);
    for (int i = 0; i < 1; i++) {
        getline(data_file, line);
        getStockPriceInfoFromLine(line);
    }

    data_file.close();
}

int main() {

    test_open_file();
    return 0;
}