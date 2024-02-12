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
    Napi::Object linearSearchController = Napi::Object::New(env);
    linearSearchController.Set(Napi::String::New(env, "addStock"), Napi::Function::New(env, LC::addStock));
    linearSearchController.Set(Napi::String::New(env, "deleteStock"), Napi::Function::New(env, LC::deleteStock));
    linearSearchController.Set(Napi::String::New(env, "updateStock"), Napi::Function::New(env, LC::updateStock));
    linearSearchController.Set(Napi::String::New(env, "getStock"), Napi::Function::New(env, LC::getStock));
    linearSearchController.Set(Napi::String::New(env, "getStockList"), Napi::Function::New(env, LC::getStockList));

    // Register indexController methods
    Napi::Object indexController = Napi::Object::New(env);
    indexController.Set(Napi::String::New(env, "addStock"), Napi::Function::New(env, TC::addStock));
    indexController.Set(Napi::String::New(env, "deleteStock"), Napi::Function::New(env, TC::deleteStock));
    indexController.Set(Napi::String::New(env, "updateStock"), Napi::Function::New(env, TC::updateStock));
    indexController.Set(Napi::String::New(env, "getStock"), Napi::Function::New(env, TC::getStock));
    indexController.Set(Napi::String::New(env, "getStockList"), Napi::Function::New(env, TC::getStockList));
    indexController.Set(Napi::String::New(env, "getStockPriceList"), Napi::Function::New(env, TC::getStockPriceList));

    // Register stateController methods
    Napi::Object stateController = Napi::Object::New(env);
    stateController.Set(Napi::String::New(env, "getDatabaseState"), Napi::Function::New(env, SC::getDatabaseState));
    stateController.Set(Napi::String::New(env, "resetDatabase"), Napi::Function::New(env, SC::resetDatabase));
    stateController.Set(Napi::String::New(env, "loadDb"), Napi::Function::New(env, SC::loadDb));
    

    // Register linearSearchController
    exports.Set(Napi::String::New(env, "linearSearch"), linearSearchController);
    exports.Set(Napi::String::New(env, "indexController"), indexController);
    exports.Set(Napi::String::New(env, "stateController"), stateController);

    return exports;
}

NODE_API_MODULE(hello, Init)
