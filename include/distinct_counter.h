#pragma once
#include <map>
#include <set>

#include "calculator.h"
#include "order_list.h"

namespace order_analyse {

class DistinctCounter : public Calculator {
 private:
 public:
  std::map<std::string, int> Calculate(const OrderList& order_list,
                                       const std::string& header,
                                       const std::string& place_holder = "") {
    std::set<std::string> contents;
    int index = order_list.index(header);
    for (const auto& order : order_list.orders()) {
      contents.insert(order.cell().at(index));
    }
    std::map<std::string, int> map;
    map[header] = contents.size();
    return map;
  }
};

}  // namespace order_analyse
