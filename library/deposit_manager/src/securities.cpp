#include "securities.h"

namespace deposit_manager {

Securities::Securities(double price) : _price(price) {}

Securities::Securities(double price, const Broker &broker)
    : _price(price), _broker(broker) {}

double Securities::CalculateCost(int volume) const {
  return BuyPrice() * volume;
}

double Securities::Price() const { return _price; }

double Securities::BuyPrice() const { return _price * (1 + _broker.tax()); }

double Securities::SellPrice() const { return _price * (1 - _broker.tax()); }

int Securities::lotSize() const { return _lot_size; }

void Securities::SetLotSize(int size) noexcept { _lot_size = size; }

const Broker &Securities::broker() const { return _broker; }

}  // namespace deposit_manager
