#pragma once

#include "broker.h"

namespace deposit_manager {

class Securities {
 public:
  Securities(double price);
  Securities(double price, const Broker &broker);

  double CalculateCost(int volume) const;

  double price() const;

  int lotSize() const;

  void SetLotSize(int size) noexcept;

 private:
  double _price = 0.0;
  Broker _broker;
  int _lot_size = 1;
};

}  // namespace deposit_manager
