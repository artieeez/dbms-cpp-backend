#include "napi.h"

namespace LC {

Napi::Value getStock(const Napi::CallbackInfo &info);
Napi::Value getStockList(const Napi::CallbackInfo &info);
Napi::Value getStockPriceList(const Napi::CallbackInfo &info);

} // namespace LC