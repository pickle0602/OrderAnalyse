#pragma once
#include <set>

#include "calculator.h"
#include "order_list.h"

namespace order_analyse {

class DistinctCounter : public Calculator {
 private:
 public:
  int Calculate(const OrderList& order_list, const std::string& header) {
    std::set<std::string> contents;
    int index = order_list.index(header);
    for (const auto& order : order_list.orders()) {
      contents.insert(order.cell().at(index));
    }
    return contents.size();
  }
};

}  // namespace order_analyse
