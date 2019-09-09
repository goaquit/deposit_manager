#pragma once

namespace deposit_manager {

class Broker {
 public:
  Broker() = default;
  Broker(double tax_broker, double tax_exchange);

  double tax() const;

 private:
  double _tax_broker = 0.0;
  double _tax_exchange = 0.0;
};

}  // namespace deposit_manager
