#include "napi.h"

namespace SC {

Napi::Value getDatabaseState(const Napi::CallbackInfo &info);
Napi::Value resetDatabase(const Napi::CallbackInfo &info);
Napi::Value loadDb(const Napi::CallbackInfo &info);

} // namespace LC