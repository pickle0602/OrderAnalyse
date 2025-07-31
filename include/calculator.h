#pragma once
#include "order_list.h"

namespace order_analyse {

class Calculator {
 private:
 public:
  virtual int Calculate(const OrderList& order_list,
                        const std::string& header) {};
};

}  // namespace order_analyse
