#pragma once
#include <unordered_set>

#include "calculator.h"
#include "order.h"

namespace order_analyse {

class DistinctCounter : public Calculator {
 private:
 public:
  int Calculate(const std::vector<Order>& orders, const std::string& header) {
    std::unordered_set<std::string> contents;
    if (orders.begin()->header().count(header) == 1) {
      int code = orders.begin()->header()[header];
      for (auto order_ptr = orders.begin(); order_ptr != orders.end();
           order_ptr++)
        contents.insert(order_ptr->cell()[code]);
    }
    return contents.size();
  }
};

}  // namespace order_analyse
