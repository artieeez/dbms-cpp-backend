#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cassert>

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

void test_open_file () {
    constexpr char path_data_file [] = {"./../data/bovespa_stocks.csv"};
    std::ifstream data_file {path_data_file};

    assert(data_file.is_open());

    for (int i = 0; i < 100; i++) {
        std::string line {};
        std::getline(data_file, line);
        std::cout << line << std::endl;
    }
}

int main(int argc, char* argv []) {

    test_open_file();

    return 0;
}