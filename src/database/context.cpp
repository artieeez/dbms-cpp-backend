#include "context.hpp"
#include "stock.hpp"
#include "stockPrice.hpp"
#include "trie.hpp"
#include <filesystem>

namespace Database {

template <typename T>
Context<T>::Context(const std::string &filePath) {

    _filePath = filePath;

    if (std::filesystem::exists(filePath)) {
        _file = std::fstream(filePath, std::ios::in | std::ios::out | std::ios::binary);
    } else {
        _file = std::fstream(filePath, std::ios::in | std::ios::out | std::ios::app | std::ios::binary);
    }

    // Check if the file is open
    if (!_file.is_open()) {
        std::cerr << "Error opening or creating file: " << filePath << std::endl;
        // Handle the error as needed
    }

    _empty = std::vector<std::streampos>();
    // // Linear search file for empty records
    // clearIterator();
    // while (next()) {
    //     if (curr.deleted) {
    //         _empty.push_back(_currPos);
    //         // _empty.push_back(curr.position);
    //     }
    // };
}

template <typename T>
Context<T>::~Context() {
    _file.close();
}

template <typename T>
Record<T> Context<T>::read(std::streampos position) {
    _file.seekg(position);
    Record<T> record;
    _file.read(reinterpret_cast<char *>(&record), sizeof(Record<T>));

    return record;
}

template <typename T>
std::vector<Record<T>> Context<T>::find(std::function<bool(Record<T>)> predicate) {
    clearIterator();
    std::vector<Record<T>> results;
    while (next()) {
        if (predicate(curr)) {
            results.push_back(curr);
        }
    }
    return results;
}

template <typename T>
std::streampos Context<T>::append(const T &value) {
    // position file pointer either at an empty position or at end of file
    std::streampos position;
    if (_empty.size() > 0) {
        position = _empty.back();
        _empty.pop_back();
        _file.seekp(position);
    } else {
        int lastPos = (int)getLastPosition();
        if (lastPos < 0) { // Empty file
            position = 0;
        } else {
            position = lastPos + (int)sizeof(Record<T>);
        }
    }

    Record<T> record;
    record.value = value;
    record.position = position;
    record.deleted = false;

    _file.write(reinterpret_cast<const char *>(&record), sizeof(Record<T>));

    return position;
}

template <typename T>
void Context<T>::save(const Record<T> &record) {
    _file.seekp(record.position);

    _file.write(reinterpret_cast<const char *>(&record), sizeof(Record<T>));
}

template <typename T>
void Context<T>::remove(std::streampos position) {

    Record<T> record = read(position);
    record.deleted = true;
    save(record);
    _empty.push_back(position);
}

template <typename T>
std::streampos Context<T>::getLastPosition() {
    _file.seekg(0, std::ios::end);
    std::streampos endPosition = _file.tellg();

    if (endPosition == 0 || endPosition < sizeof(Record<T>)) {
        return -1; // Empty file
    }

    // Calculate the position of the last record
    return (int)endPosition - (int)sizeof(Record<T>);
}

template <typename T>
void Context<T>::move(std::streampos position, std::streampos newPosition) {
    // Check if the positions are valid
    if (position < 0 || newPosition < 0) {
        std::cerr << "Invalid positions for record relocation." << std::endl;
        return;
    }

    // Check if the file is large enough to contain records at the specified positions
    _file.seekg(0, std::ios::end);
    std::streampos endPosition = _file.tellg();

    if (endPosition < (int)std::max(position, newPosition) + sizeof(Record<T>)) {
        std::cerr << "File is too small to contain records at the specified positions." << std::endl;
        return;
    }

    // Move the get pointer to the position of the record to be relocated
    _file.seekg(position);

    // Read the record from the original position
    Record<T> record;
    _file.read(reinterpret_cast<char *>(&record), sizeof(Record<T>));

    // Move the put pointer to the new position
    _file.seekp(newPosition);

    // Write the record to the new position
    _file.write(reinterpret_cast<const char *>(&record), sizeof(Record<T>));
}

template <typename T>
void Context<T>::reset() {
    _file.close();
    _file.open(_filePath, std::ios::out | std::ios::trunc);
    _file.close();
    _file.open(_filePath, std::ios::in | std::ios::out | std::ios::app | std::ios::binary);
    _empty.clear();
    _currPos = -1;
}

// Iterator
template <typename T>
bool Context<T>::next(bool skipDeleted) {
    bool positionIsValid = false;
    do {
        _currPos = ((int)_currPos < 0)
                       ? (int)static_cast<std::streampos>(0)
                       : (int)_currPos + (int)sizeof(Record<T>);

        positionIsValid = _currPos <= getLastPosition();

        if (positionIsValid) {
            curr = read(_currPos);
        }

    } while (positionIsValid && (curr.deleted && skipDeleted));

    return positionIsValid;
}

template <typename T>
void Context<T>::clearIterator() {
    _currPos = -1;
}

template <typename T>
std::streampos Context<T>::getCurrPosition() {
    return _currPos;
}

} // namespace Database

template class Database::Context<int>;
template class Database::Context<Model::Stock>;
template class Database::Context<Model::StockPrice>;
template class Database::Context<Index::TrieNode>;
