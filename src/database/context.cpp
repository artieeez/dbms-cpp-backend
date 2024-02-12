#include "context.hpp"
#include "blockStorage.hpp"
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
        _file = std::fstream(filePath, std::ios::in | std::ios::out | std::ios::trunc | std::ios::binary);
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
    record.error = true;

    try {
        _file.read(reinterpret_cast<char *>(&record), sizeof(Record<T>));
        if (!_file) {
            record.error = true;
            _file.clear();
        } else {
            record.error = false;
        }
    } catch (std::exception &e) {
        std::cerr << "Error reading file: " << e.what() << std::endl;
        record.error = true;
    }

    return record;
}

template <typename T>
std::vector<Record<T>> Context<T>::find(std::function<bool(Record<T>)> predicate, int pageSize, int page) {
    clearIterator();
    int recordCount = 0;
    int pageCount = 0;
    std::vector<Record<T>> results;
    while (next() && (pageSize == 0 || recordCount < pageSize * (page + 1))) {
        if (predicate(curr)) {
            recordCount++;
            if (pageSize > 0) {
                if (recordCount > pageSize * (pageCount + 1)) {
                    pageCount++;
                }
                if (pageCount == page) {
                    results.push_back(curr);
                }
            } else {
                results.push_back(curr);
            }
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
        position = getFileEnd();
    }

    Record<T> record;
    record.value = value;
    record.position = position;
    record.deleted = false;
    record.error = false;

    save(record);

    return position;
}

template <typename T>
void Context<T>::save(Record<T> &record) {
    _file.seekp(record.position);

    record.deleted = false;
    record.error = false;

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
std::streampos Context<T>::getFileEnd() {
    _file.seekp(0, std::ios_base::end);
    return _file.tellp();
}

template <typename T>
bool Context<T>::isEmpty() {
    return getFileEnd() == 0;
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

        auto fileEnd = getFileEnd();
        positionIsValid = (_currPos <= fileEnd) && (fileEnd > 0);

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
template class Database::Context<std::streampos>;
template class Database::Context<Model::Stock>;
template class Database::Context<Model::StockPrice>;
template class Database::Context<Index::TrieNode>;
template class Database::Context<Index::Block>;
