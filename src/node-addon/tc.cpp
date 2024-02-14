#include "context.hpp"
#include "indexController.hpp"
#include "logger.hpp"
#include "napi.h"
#include "stock.hpp"
#include <fstream>
#include <iomanip> // std::setw
#include <iostream>
#include <node_api.h>
#include <vector>

extern Logger mainLogger;

namespace TC {

Napi::Value addStock(const Napi::CallbackInfo &info) {
    mainLogger.pushScope("addStock");
    Napi::Env env = info.Env();

    if (info.Length() < 1) {
        mainLogger.log("Wrong number of arguments. Expected 1", LogType::ERROR);
        Napi::TypeError::New(env, "Wrong number of arguments. Expected 1")
            .ThrowAsJavaScriptException();
        return env.Null();
    }

    if (!info[0].IsObject()) {
        mainLogger.log("Wrong arguments", LogType::ERROR);
        Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
        return env.Null();
    }

    Model::Stock stock;
    Napi::Object payload = info[0].As<Napi::Object>();
    strcpy_s(stock.stockId, payload.Get("stockId").As<Napi::String>().Utf8Value().c_str());
    strcpy_s(stock.companyId, payload.Get("companyId").As<Napi::String>().Utf8Value().c_str());
    strcpy_s(stock.min_date, payload.Get("min_date").As<Napi::String>().Utf8Value().c_str());
    strcpy_s(stock.max_date, payload.Get("max_date").As<Napi::String>().Utf8Value().c_str());

    mainLogger.popScope();
    Controller::IndexSearch::addStock(stock);
}

Napi::Value deleteStock(const Napi::CallbackInfo &info) {
    mainLogger.pushScope("deleteStock");
    Napi::Env env = info.Env();

    if (info.Length() < 1) {
        mainLogger.log("Wrong number of arguments. Expected 1", LogType::ERROR);
        Napi::TypeError::New(env, "Wrong number of arguments. Expected 1")
            .ThrowAsJavaScriptException();
        return env.Null();
    }

    if (!info[0].IsString()) {
        mainLogger.log("Wrong arguments", LogType::ERROR);
        Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
        return env.Null();
    }

    char stockId[MAX_SIZE_STOCK];
    strcpy_s(stockId, info[0].As<Napi::String>().Utf8Value().c_str());
    Controller::IndexSearch::deleteStock(stockId);

    mainLogger.popScope();
    return Napi::String::New(env, "success");
}

Napi::Value updateStock(const Napi::CallbackInfo &info) {
    mainLogger.pushScope("updateStock");
    Napi::Env env = info.Env();

    if (info.Length() < 1) {
        mainLogger.log("Wrong number of arguments. Expected 1", LogType::ERROR);
        Napi::TypeError::New(env, "Wrong number of arguments. Expected 1")
            .ThrowAsJavaScriptException();
        return env.Null();
    }

    if (!info[0].IsObject()) {
        mainLogger.log("Wrong arguments", LogType::ERROR);
        Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
        return env.Null();
    }

    Model::Stock stock;
    Napi::Object payload = info[0].As<Napi::Object>();
    strcpy_s(stock.stockId, payload.Get("stockId").As<Napi::String>().Utf8Value().c_str());
    strcpy_s(stock.companyId, payload.Get("companyId").As<Napi::String>().Utf8Value().c_str());
    strcpy_s(stock.min_date, payload.Get("min_date").As<Napi::String>().Utf8Value().c_str());
    strcpy_s(stock.max_date, payload.Get("max_date").As<Napi::String>().Utf8Value().c_str());

    Controller::IndexSearch::updateStock(stock);

    mainLogger.popScope();
    return Napi::String::New(env, "success");
}

Napi::Value getStock(const Napi::CallbackInfo &info) {
    mainLogger.pushScope("getStock");
    Napi::Env env = info.Env();

    if (info.Length() < 1) {
        mainLogger.log("Wrong number of arguments. Expected 1", LogType::ERROR);
        Napi::TypeError::New(env, "Wrong number of arguments. Expected 1")
            .ThrowAsJavaScriptException();
        return env.Null();
    }

    if (!info[0].IsString()) {
        mainLogger.log("Wrong arguments", LogType::ERROR);
        Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
        return env.Null();
    }

    char stockId[MAX_SIZE_STOCK];
    strcpy_s(stockId, info[0].As<Napi::String>().Utf8Value().c_str());
    Model::Stock stock = Controller::IndexSearch::getStock(stockId);

    Napi::Object result = Napi::Object::New(env);
    result.Set(Napi::String::New(env, "stockId"), Napi::String::New(env, stock.stockId));
    result.Set(Napi::String::New(env, "companyId"), Napi::String::New(env, stock.companyId));
    result.Set(Napi::String::New(env, "min_date"), Napi::String::New(env, stock.min_date));
    result.Set(Napi::String::New(env, "max_date"), Napi::String::New(env, stock.max_date));

    mainLogger.popScope();
    return result;
}

Napi::Value getStockList(const Napi::CallbackInfo &info) {
    mainLogger.pushScope("getStockList");
    Napi::Env env = info.Env();

    if (info.Length() < 3) {
        mainLogger.log("Wrong number of arguments. Expected 3", LogType::ERROR);
        Napi::TypeError::New(env, "Wrong number of arguments. Expected 3")
            .ThrowAsJavaScriptException();
        return env.Null();
    }

    if (!info[0].IsString() || !info[1].IsNumber() || !info[2].IsNumber()) {
        mainLogger.log("Wrong arguments", LogType::ERROR);
        Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
        return env.Null();
    }

    char stockId[MAX_SIZE_STOCK];
    strcpy_s(stockId, info[0].As<Napi::String>().Utf8Value().c_str());
    int page = info[1].As<Napi::Number>().Int32Value();
    int pageSize = info[2].As<Napi::Number>().Int32Value();

    std::vector<Model::Stock> stocks = Controller::IndexSearch::getStockList(stockId, page, pageSize);

    Napi::Array result = Napi::Array::New(env, stocks.size());
    for (int i = 0; i < stocks.size(); i++) {
        Napi::Object stock = Napi::Object::New(env);
        stock.Set(Napi::String::New(env, "stockId"), Napi::String::New(env, stocks[i].stockId));
        stock.Set(Napi::String::New(env, "companyId"), Napi::String::New(env, stocks[i].companyId));
        stock.Set(Napi::String::New(env, "min_date"), Napi::String::New(env, stocks[i].min_date));
        stock.Set(Napi::String::New(env, "max_date"), Napi::String::New(env, stocks[i].max_date));
        result.Set(i, stock);
    }

    mainLogger.popScope();
    return result;
}

Napi::Value addStockPrice(const Napi::CallbackInfo &info) {
    mainLogger.pushScope("addStockPrice");
    Napi::Env env = info.Env();

    if (info.Length() < 1) {
        mainLogger.log("Wrong number of arguments. Expected 1", LogType::ERROR);
        Napi::TypeError::New(env, "Wrong number of arguments. Expected 1")
            .ThrowAsJavaScriptException();
        return env.Null();
    }

    if (!info[0].IsObject()) {
        mainLogger.log("Wrong arguments", LogType::ERROR);
        Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
        return env.Null();
    }

    // serialize Napi::Object to Model::StockPrice
    Napi::Object payload = info[0].As<Napi::Object>();
    Model::StockPrice stockPrice;
    strcpy_s(stockPrice.stockPriceId, payload.Get("stockPriceId").As<Napi::String>().Utf8Value().c_str());
    strcpy_s(stockPrice.stockId, payload.Get("stockId").As<Napi::String>().Utf8Value().c_str());
    strcpy_s(stockPrice.date, payload.Get("date").As<Napi::String>().Utf8Value().c_str());
    stockPrice.adj = payload.Get("adj").As<Napi::Number>().DoubleValue();
    stockPrice.close = payload.Get("close").As<Napi::Number>().DoubleValue();
    stockPrice.high = payload.Get("high").As<Napi::Number>().DoubleValue();
    stockPrice.low = payload.Get("low").As<Napi::Number>().DoubleValue();
    stockPrice.open = payload.Get("open").As<Napi::Number>().DoubleValue();
    stockPrice.volume = payload.Get("volume").As<Napi::Number>().DoubleValue();

    Controller::IndexSearch::addStockPrice(stockPrice);

    mainLogger.popScope();
    return Napi::String::New(env, "success");
}

Napi::Value deleteStockPrice(const Napi::CallbackInfo &info) {
    mainLogger.pushScope("deleteStockPrice");
    Napi::Env env = info.Env();

    if (info.Length() < 2) {
        mainLogger.log("Wrong number of arguments. Expected 2", LogType::ERROR);
        Napi::TypeError::New(env, "Wrong number of arguments. Expected 2")
            .ThrowAsJavaScriptException();
        return env.Null();
    }

    if (!info[0].IsString() || !info[1].IsString()) {
        mainLogger.log("Wrong arguments", LogType::ERROR);
        Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
        return env.Null();
    }

    char stockPriceId[MAX_SIZE_SP];
    strcpy_s(stockPriceId, info[0].As<Napi::String>().Utf8Value().c_str());
    char stockId[MAX_SIZE_STOCK];
    strcpy_s(stockId, info[1].As<Napi::String>().Utf8Value().c_str());
    Controller::IndexSearch::deleteStockPrice(stockPriceId, stockId);

    mainLogger.popScope();
    return Napi::String::New(env, "success");
}

Napi::Value getStockPriceList(const Napi::CallbackInfo &info) {
    mainLogger.pushScope("getStockPriceList");
    Napi::Env env = info.Env();

    if (info.Length() < 3) {
        mainLogger.log("Wrong number of arguments. Expected 3", LogType::ERROR);
        Napi::TypeError::New(env, "Wrong number of arguments. Expected 3")
            .ThrowAsJavaScriptException();
        return env.Null();
    }

    if (!info[0].IsString() || !info[1].IsNumber() || !info[2].IsNumber()) {
        mainLogger.log("Wrong arguments", LogType::ERROR);
        Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
        return env.Null();
    }

    char stockId[MAX_SIZE_STOCK];
    strcpy_s(stockId, info[0].As<Napi::String>().Utf8Value().c_str());
    int page = info[1].As<Napi::Number>().Int32Value();
    int pageSize = info[2].As<Napi::Number>().Int32Value();

    std::vector<Model::StockPrice> stockPrices = Controller::IndexSearch::getStockPriceList(stockId, page, pageSize);

    Napi::Array result = Napi::Array::New(env, stockPrices.size());
    for (int i = 0; i < stockPrices.size(); i++) {
        Napi::Object stockPrice = Napi::Object::New(env);
        stockPrice.Set(Napi::String::New(env, "stockPriceId"), Napi::String::New(env, stockPrices[i].stockPriceId));
        stockPrice.Set(Napi::String::New(env, "stockId"), Napi::String::New(env, stockPrices[i].stockId));
        stockPrice.Set(Napi::String::New(env, "date"), Napi::String::New(env, stockPrices[i].date));
        stockPrice.Set(Napi::String::New(env, "adj"), Napi::Number::New(env, stockPrices[i].adj));
        stockPrice.Set(Napi::String::New(env, "close"), Napi::Number::New(env, stockPrices[i].close));
        stockPrice.Set(Napi::String::New(env, "high"), Napi::Number::New(env, stockPrices[i].high));
        stockPrice.Set(Napi::String::New(env, "low"), Napi::Number::New(env, stockPrices[i].low));
        stockPrice.Set(Napi::String::New(env, "open"), Napi::Number::New(env, stockPrices[i].open));
        stockPrice.Set(Napi::String::New(env, "volume"), Napi::Number::New(env, stockPrices[i].volume));

        result.Set(i, stockPrice);
    }

    mainLogger.popScope();
    return result;
}

} // namespace TC
