#ifndef STOCK_HPP
#define STOCK_HPP

#include <string>

namespace Model
{

    struct Stock
    {
        char stockId [30];
        char companyId [30];
        char min_date [30];
        char max_date [30];
    };

} // namespace Model

#endif // STOCK_HPP
