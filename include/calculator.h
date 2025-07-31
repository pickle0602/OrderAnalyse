#pragma once
#include "order_list.h"

namespace order_analyse {

class Calculator {
 private:
 public:
  virtual std::map<std::string, int> Calculate(const OrderList& order_list,
                                               const std::string& header1,
                                               const std::string& header2) = 0;
};

}  // namespace order_analyse
