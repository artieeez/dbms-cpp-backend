#ifndef STOCK_HPP
#define STOCK_HPP

#include <string>

namespace Model {

struct Stock
{
    std::string stockId;
    std::string companyId;
    std::string min_date;
    std::string max_date;
};

}  // namespace Model

#endif  // STOCK_HPP
