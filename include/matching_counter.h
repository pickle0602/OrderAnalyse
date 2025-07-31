#pragma once
#include <iostream>
#include <map>
#include <set>
#include <string>

#include "order_list.h"

namespace order_analyse {

class MatchingCounter : public Calculator {
 private:
 public:
  std::map<std::string, int> Calculate(const OrderList& order_list,
                                       const std::vector<std::string> headers) {
    size_t main_index = order_list.index(headers[0]);
    size_t sub_index = order_list.index(headers[1]);
    std::map<std::string, std::set<std::string>> main2set;
    for (const auto& order : order_list.orders())
      main2set[order.cell()[main_index]].insert(order.cell()[sub_index]);
    std::map<std::string, int> main2size;
    for (const auto& m2s : main2set) main2size[m2s.first] = m2s.second.size();
    return main2size;
  }
};

}  // namespace order_analyse
