#ifndef DB_CONTEXT_HPP
#define DB_CONTEXT_HPP

#include <fstream>
#include <ios>
#include <iostream>
#include <vector>

namespace Database {

template <typename T>
struct Record {
    T value;
    std::streampos position;
    bool deleted;
};

template <typename T>
class Context {
private:
#include <vector>

    std::string _filePath;
    std::fstream _file;
    std::streampos _currPos = -1;
    std::vector<std::streampos> _empty;

public:
    Record<T> curr;

    Context(const std::string &filePath);

    ~Context();

    Record<T> read(std::streampos position);

    std::vector<Record<T>> find(bool (*callback)(Record<T>));

    std::streampos append(const T &value);

    void save(const Record<T> &record);

    void remove(std::streampos position);

    std::streampos getLastPosition();

    void move(std::streampos position, std::streampos newPosition);

    void reset();

    bool next(bool skipDeleted = true);

    void clearIterator();

    std::streampos getCurrPosition();
};

} // namespace Database

#endif // DB_CONTEXT_HPP
