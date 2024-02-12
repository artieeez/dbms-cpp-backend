#ifndef STATE_CONTROLLER_HPP
#define STATE_CONTROLLER_HPP

#include <vector>
#include "context.hpp"

namespace Controller
{
  namespace State
  {
    Database::State getDatabaseState();

    void resetDb();

    Database::State loadDb(int pageSize);
  }
}

#endif
