#ifndef DB_CONTEXT_HPP
#define DB_CONTEXT_HPP

#include <fstream>
#include <ios>
#include <iostream>

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
    std::fstream _file;
    std::streampos _currPos = -1;

   public:
    Record<T> curr;

    Context(const std::string& filePath);

    // Context<T>::~Context();

    Record<T> read(std::streampos position);

    std::streampos append(const T& value);

    void save(const Record<T>& record);

    void remove(std::streampos position);

    std::streampos getLastPosition();

    void move(std::streampos position, std::streampos newPosition);

    bool next();

    void clearIterator();

    std::streampos getCurrPosition();
};

}  // namespace Database

#endif  // DB_CONTEXT_HPP
