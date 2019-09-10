#include <iostream>

#include "deposit_manager.h"

using DepositManager = deposit_manager::DepositManager;

int main(int /*argc*/, char* /*argv*/[]) {
  DepositManager deposit_manager;

  deposit_manager.Run();

  return 0;
}
