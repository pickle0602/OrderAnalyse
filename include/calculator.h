#pragma once
#include "order.h"

namespace order_analyse {

class Calculator {
 private:
 public:
   virtual int Calculate(const std::vector<Order>& orders) = 0;
};

}  // namespace order_analyse


