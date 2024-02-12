#ifndef DB_CONTEXT_HPP
#define DB_CONTEXT_HPP

#include <fstream>
#include <ios>
#include <iostream>
#include <vector>
#include <functional>

namespace Database {

namespace PATH {
    namespace DB {
        const std::string STOCK = "db_stock";
        const std::string STOCK_PRICE = "db_stock_price";
    }
    namespace TRIE {
        const std::string STOCK_ID_TO_STOCK = "trie_stock_id_stock_adress";
        const std::string STOCK_PRICE_ID_TO_STOCK_PRICE = "trie_stock_price_id_stock_price_adress";
        const std::string STOCK_ID_TO_STOCK_PRICE_BLOCK = "trie_stock_id_stock_price_block_adress";
    }
    namespace BLOCK {
        const std::string STOCK_ID_TO_STOCK_PRICE = "block_stock_price";
    }

    namespace LOADER {
        const std::string STATE = "loader_state";
    }
}

struct State {
    std::streampos loaderPosition;
    int stockCount;
    int stockPriceCount;
    bool isFinished;
};

template <typename T>
struct Record {
    T value;
    std::streampos position;
    bool deleted;
    bool error;
};

template <typename T>
class Context {
private:
    std::string _filePath;
    std::fstream _file;
    std::streampos _currPos = -1;
    std::vector<std::streampos> _empty;

public:
    Record<T> curr;

    Context(const std::string &filePath);

    ~Context();

    Record<T> read(std::streampos position);

    std::vector<Record<T>> find(std::function<bool(Record<T>)> predicate, int pageSize = 0, int page = 0);

    std::streampos append(const T &value);

    void save(Record<T> &record);

    void remove(std::streampos position);

    std::streampos getFileEnd();

    bool isEmpty();

    void reset();

    bool next(bool skipDeleted = true);

    void clearIterator();

    std::streampos getCurrPosition();
};

} // namespace Database

#endif // DB_CONTEXT_HPP
