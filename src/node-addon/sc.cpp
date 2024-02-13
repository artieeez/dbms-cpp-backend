#include "context.hpp"
#include "stateController.hpp"
#include "napi.h"
#include "stock.hpp"
#include <fstream>
#include <iomanip> // std::setw
#include <iostream>
#include <node_api.h>
#include <vector>

namespace SC {

Napi::Value getDatabaseState(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();
    Database::State dbState = Controller::State::getDatabaseState();

    Napi::Object result = Napi::Object::New(env);
    result.Set(Napi::String::New(env, "loaderPosition"), Napi::Number::New(env, dbState.loaderPosition));
    result.Set(Napi::String::New(env, "stockCount"), Napi::Number::New(env, dbState.stockCount));
    result.Set(Napi::String::New(env, "stockPriceCount"), Napi::Number::New(env, dbState.stockPriceCount));
    result.Set(Napi::String::New(env, "isFinished"), Napi::Boolean::New(env, dbState.isFinished));

    return result;
}

// reset the database
Napi::Value resetDatabase(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();
    Controller::State::resetDb();
    return Napi::String::New(env, "success");
}

// int loadDb(int pageSize)
Napi::Value loadDb(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();

    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Wrong number of arguments. Expected 1")
            .ThrowAsJavaScriptException();
        return env.Null();
    }

    if (!info[0].IsNumber()) {
        Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
        return env.Null();
    }

    int pageSize = info[0].As<Napi::Number>().Int32Value();
    Database::State dbState = Controller::State::loadDb(pageSize);

    Napi::Object result = Napi::Object::New(env);
    result.Set(Napi::String::New(env, "loaderPosition"), Napi::Number::New(env, dbState.loaderPosition));
    result.Set(Napi::String::New(env, "stockCount"), Napi::Number::New(env, dbState.stockCount));
    result.Set(Napi::String::New(env, "stockPriceCount"), Napi::Number::New(env, dbState.stockPriceCount));
    result.Set(Napi::String::New(env, "isFinished"), Napi::Boolean::New(env, dbState.isFinished));

    return result;
}
} // namespace TC