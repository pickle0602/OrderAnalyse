#pragma once
#include "order_list.h"

namespace order_analyse {

class Calculator {
 private:
 public:
  virtual std::map<std::string, int> Calculate(
      const OrderList& order_list, const std::vector<std::string> headers) = 0;
  virtual bool arguments_limiter(int vec_size) = 0;
};

}  // namespace order_analyse
