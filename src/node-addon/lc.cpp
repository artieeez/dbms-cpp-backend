#include "context.hpp"
#include "linearSearchController.hpp"
#include "napi.h"
#include "stock.hpp"
#include <fstream>
#include <iomanip> // std::setw
#include <iostream>
#include <node_api.h>
#include <vector>

namespace LC {
Napi::Value getStock(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();

    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Wrong number of arguments. Expected 1")
            .ThrowAsJavaScriptException();
        return env.Null();
    }

    if (!info[0].IsString()) {
        Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
        return env.Null();
    }

    char stockId[MAX_SIZE_STOCK];
    strcpy_s(stockId, info[0].As<Napi::String>().Utf8Value().c_str());
    Model::Stock stock = Controller::LinearSearch::getStock(stockId);

    Napi::Object result = Napi::Object::New(env);
    result.Set(Napi::String::New(env, "stockId"), Napi::String::New(env, stock.stockId));
    result.Set(Napi::String::New(env, "companyId"), Napi::String::New(env, stock.companyId));
    result.Set(Napi::String::New(env, "min_date"), Napi::String::New(env, stock.min_date));
    result.Set(Napi::String::New(env, "max_date"), Napi::String::New(env, stock.max_date));

    return result;
}

Napi::Value getStockList(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();

    if (info.Length() < 3) {
        Napi::TypeError::New(env, "Wrong number of arguments. Expected 4")
            .ThrowAsJavaScriptException();
        return env.Null();
    }

    if (!info[0].IsString() || !info[1].IsNumber() || !info[2].IsNumber()) {
        Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
        return env.Null();
    }

    char stockId[MAX_SIZE_STOCK] = {0};
    strcpy_s(stockId, info[0].As<Napi::String>().Utf8Value().c_str());
    int pageNumber = info[1].As<Napi::Number>().Int32Value();
    int pageSize = info[2].As<Napi::Number>().Int32Value();

    std::vector<Model::Stock> stocks = Controller::LinearSearch::getStockList(stockId, pageNumber, pageSize);

    Napi::Array result = Napi::Array::New(env, stocks.size());
    for (int i = 0; i < stocks.size(); i++) {
        Napi::Object stock = Napi::Object::New(env);
        stock.Set(Napi::String::New(env, "stockId"), Napi::String::New(env, stocks[i].stockId));
        stock.Set(Napi::String::New(env, "companyId"), Napi::String::New(env, stocks[i].companyId));
        stock.Set(Napi::String::New(env, "min_date"), Napi::String::New(env, stocks[i].min_date));
        stock.Set(Napi::String::New(env, "max_date"), Napi::String::New(env, stocks[i].max_date));
        result.Set(i, stock);
    }

    return result;
}

Napi::Value getStockPriceList(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();

    if (info.Length() < 3) {
        Napi::TypeError::New(env, "Wrong number of arguments. Expected 3")
            .ThrowAsJavaScriptException();
        return env.Null();
    }

    if (!info[0].IsString() || !info[1].IsNumber() || !info[2].IsNumber()) {
        Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
        return env.Null();
    }

    char stockId[MAX_SIZE_STOCK] = {0};
    strcpy_s(stockId, info[0].As<Napi::String>().Utf8Value().c_str());
    int pageNumber = info[1].As<Napi::Number>().Int32Value();
    int pageSize = info[2].As<Napi::Number>().Int32Value();

    std::vector<Model::StockPrice> stockPriceList = Controller::LinearSearch::getStockPriceList(stockId, pageNumber, pageSize);

    Napi::Array result = Napi::Array::New(env, stockPriceList.size());
    for (int i = 0; i < stockPriceList.size(); i++) {
        Napi::Object stockPrice = Napi::Object::New(env);
        stockPrice.Set(Napi::String::New(env, "stockId"),Napi::String::New(env, stockPriceList[i].stockId));
        stockPrice.Set(Napi::String::New(env, "stockPriceId"),Napi::String::New(env, stockPriceList[i].stockPriceId));
        stockPrice.Set(Napi::String::New(env, "date"),Napi::String::New(env, stockPriceList[i].date));
        stockPrice.Set(Napi::String::New(env, "adj"),Napi::Number::New(env, stockPriceList[i].adj));
        stockPrice.Set(Napi::String::New(env, "close"),Napi::Number::New(env, stockPriceList[i].close));
        stockPrice.Set(Napi::String::New(env, "high"),Napi::Number::New(env, stockPriceList[i].high));
        stockPrice.Set(Napi::String::New(env, "low"),Napi::Number::New(env, stockPriceList[i].low));
        stockPrice.Set(Napi::String::New(env, "open"),Napi::Number::New(env, stockPriceList[i].open));
        stockPrice.Set(Napi::String::New(env, "volume"),Napi::Number::New(env, stockPriceList[i].volume));
        result.Set(i, stockPrice);
    }
}

} // namespace LC