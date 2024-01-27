#ifndef DB_CONTEXT_HPP
#define DB_CONTEXT_HPP

#include <fstream>
#include <ios>
#include <iostream>

namespace DatabaseManagementSystem {
namespace Database {

template <typename T>
class DbContext {
   private:
    std::fstream _file;
    std::streampos _currPos = -1;

   public:
    T curr;

    DbContext(const std::string& filePath);

    // DbContext<T>::~DbContext();

    T read(std::streampos position);

    std::streampos append(const T& record);

    void write(const T& record, std::streampos position);

    std::streampos remove(std::streampos position);

    std::streampos getLastPosition();

    void move(std::streampos position, std::streampos newPosition);

    bool next();

    void clearIterator();

    std::streampos getCurrPosition();
};

}  // namespace Database
}  // namespace DatabaseManagementSystem

#endif  // DB_CONTEXT_HPP
