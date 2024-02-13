#ifndef STOCK_HPP
#define STOCK_HPP

#include <string>
constexpr int MAX_SIZE_STOCK = 10;
constexpr int MAX_SIZE_DATE = 15;

namespace Model
{

    struct Stock
    {
        char stockId [MAX_SIZE_STOCK];
        char companyId [MAX_SIZE_STOCK];
        char min_date [MAX_SIZE_DATE];
        char max_date [MAX_SIZE_DATE];
    };

} // namespace Model

#endif // STOCK_HPP
