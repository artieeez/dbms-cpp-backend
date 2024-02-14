#include "context.hpp"
#include "linearSearchController.hpp"
#include "napi.h"
#include "stock.hpp"
#include <fstream>
#include <iomanip> // std::setw
#include <iostream>
#include <node_api.h>
#include <vector>
#include "lc.hpp"
#include "tc.hpp"
#include "sc.hpp"

Napi::Object Init(Napi::Env env, Napi::Object exports) {

    // Register linearSearchController methods
    Napi::Object linear = Napi::Object::New(env);
    linear.Set(Napi::String::New(env, "getStock"), Napi::Function::New(env, LC::getStock));
    linear.Set(Napi::String::New(env, "getStockList"), Napi::Function::New(env, LC::getStockList));
    linear.Set(Napi::String::New(env, "getStockPriceList"), Napi::Function::New(env, LC::getStockPriceList));

    // Register indexController methods
    Napi::Object trie = Napi::Object::New(env);
    trie.Set(Napi::String::New(env, "addStock"), Napi::Function::New(env, TC::addStock));
    trie.Set(Napi::String::New(env, "deleteStock"), Napi::Function::New(env, TC::deleteStock));
    trie.Set(Napi::String::New(env, "updateStock"), Napi::Function::New(env, TC::updateStock));
    trie.Set(Napi::String::New(env, "getStock"), Napi::Function::New(env, TC::getStock));
    trie.Set(Napi::String::New(env, "getStockList"), Napi::Function::New(env, TC::getStockList));
    trie.Set(Napi::String::New(env, "addStockPrice"), Napi::Function::New(env, TC::addStockPrice));
    trie.Set(Napi::String::New(env, "deleteStockPrice"), Napi::Function::New(env, TC::deleteStockPrice));
    trie.Set(Napi::String::New(env, "getStockPriceList"), Napi::Function::New(env, TC::getStockPriceList));

    // Register stateController methods
    Napi::Object state = Napi::Object::New(env);
    state.Set(Napi::String::New(env, "getDatabaseState"), Napi::Function::New(env, SC::getDatabaseState));
    state.Set(Napi::String::New(env, "resetDatabase"), Napi::Function::New(env, SC::resetDatabase));
    state.Set(Napi::String::New(env, "loadDb"), Napi::Function::New(env, SC::loadDb));
    
    exports.Set(Napi::String::New(env, "linear"), linear);
    exports.Set(Napi::String::New(env, "trie"), trie);
    exports.Set(Napi::String::New(env, "state"), state);

    return exports;
}

NODE_API_MODULE(hello, Init)
