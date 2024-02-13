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

Napi::Value addStock(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();

    if (info.Length() < 4) {
        Napi::TypeError::New(env, "Wrong number of arguments. Expected 4")
            .ThrowAsJavaScriptException();
        return env.Null();
    }

    if (!info[0].IsString() || !info[1].IsString() || !info[2].IsString() ||
        !info[3].IsString()) {
        Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
        return env.Null();
    }

    Model::Stock stock;
    stock.stockId = info[0].As<Napi::String>().Utf8Value();
    stock.companyId = info[1].As<Napi::String>().Utf8Value();
    stock.min_date = info[2].As<Napi::String>().Utf8Value();
    stock.max_date = info[3].As<Napi::String>().Utf8Value();

    Controller::LinearSearch::addStock(stock);

    return Napi::String::New(env, "success");
}

Napi::Value deleteStock(const Napi::CallbackInfo &info) {
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

    std::string stockId = info[0].As<Napi::String>().Utf8Value();
    Controller::LinearSearch::deleteStock(stockId);

    return Napi::String::New(env, "success");
}

Napi::Value updateStock(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();

    if (info.Length() < 4) {
        Napi::TypeError::New(env, "Wrong number of arguments. Expected 4")
            .ThrowAsJavaScriptException();
        return env.Null();
    }

    if (!info[0].IsString() || !info[1].IsString() || !info[2].IsString() ||
        !info[3].IsString()) {
        Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
        return env.Null();
    }

    Model::Stock stock;
    stock.stockId = info[0].As<Napi::String>().Utf8Value();
    stock.companyId = info[1].As<Napi::String>().Utf8Value();
    stock.min_date = info[2].As<Napi::String>().Utf8Value();
    stock.max_date = info[3].As<Napi::String>().Utf8Value();

    Controller::LinearSearch::updateStock(stock);

    return Napi::String::New(env, "success");
}

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

    std::string stockId = info[0].As<Napi::String>().Utf8Value();
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

    if (info.Length() < 4) {
        Napi::TypeError::New(env, "Wrong number of arguments. Expected 4")
            .ThrowAsJavaScriptException();
        return env.Null();
    }

    if (!info[0].IsString() || !info[1].IsNumber() || !info[2].IsNumber() ||
        !info[3].IsString()) {
        Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
        return env.Null();
    }

    std::string prefix = info[0].As<Napi::String>().Utf8Value();
    int page = info[1].As<Napi::Number>().Int32Value();
    int pageSize = info[2].As<Napi::Number>().Int32Value();
    std::string orderBy = info[3].As<Napi::String>().Utf8Value();

    std::vector<Model::Stock> stocks = Controller::LinearSearch::getStockList(prefix, page, pageSize, orderBy);

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

} // namespace LC