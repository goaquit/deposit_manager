#pragma once

#include "broker.h"

namespace deposit_manager {

class Securities {
 public:
  Securities(double BuyPrice);
  Securities(double BuyPrice, const Broker& broker);

  double CalculateCost(int volume) const;

  double Price() const;
  double BuyPrice() const;
  double SellPrice() const;

  int lotSize() const;

  void SetLotSize(int size) noexcept;

  const Broker& broker() const;

 private:
  double _price = 0.0;
  Broker _broker;
  int _lot_size = 1;
};

}  // namespace deposit_manager
