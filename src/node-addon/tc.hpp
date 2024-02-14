#include "napi.h"

namespace TC {

Napi::Value addStock(const Napi::CallbackInfo &info);
Napi::Value deleteStock(const Napi::CallbackInfo &info);
Napi::Value updateStock(const Napi::CallbackInfo &info);
Napi::Value getStock(const Napi::CallbackInfo &info);
Napi::Value getStockList(const Napi::CallbackInfo &info);

Napi::Value addStockPrice(const Napi::CallbackInfo &info);
Napi::Value deleteStockPrice(const Napi::CallbackInfo &info);
Napi::Value getStockPriceList(const Napi::CallbackInfo &info);

} // namespace LC