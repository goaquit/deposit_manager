#include "broker.h"

namespace deposit_manager {

Broker::Broker(double tax_broker, double tax_exchange)
    : _tax_broker(tax_broker), _tax_exchange(tax_exchange) {}

double Broker::tax() const { return _tax_broker + _tax_exchange; }

}  // namespace deposit_manager
