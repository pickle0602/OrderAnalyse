#pragma once
#include <unordered_set>

#include "calculator.h"
#include "order.h"

namespace order_analyse {

class DistinctCounter : public Calculator {
 private:
 public:
  int Calculate(const OrderList& order_list, const std::string& header) {
    std::unordered_set<std::string> contents;
    if (orders.begin()->header().count(header) == 1) {
      int code = orders.begin()->header()[header];
      for (const auto& order : orders) contents.insert(order.cell().at(code));
    }
    return contents.size();
  }
};

}  // namespace order_analyse
