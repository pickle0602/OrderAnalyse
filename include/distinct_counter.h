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
                                       const std::vector<std::string> headers) {
    if (arguments_limiter(headers.size())) {
      std::map<std::string, int> error_map;
      error_map["Error"] = -1;
      return error_map;
    }
    std::set<std::string> contents;
    int index = order_list.index(headers[0]);
    for (const auto& order : order_list.orders()) {
      contents.insert(order.cell().at(index));
    }
    std::map<std::string, int> map;
    map[headers[0]] = contents.size();
    return map;
  }
  bool arguments_limiter(int vec_size) { return vec_size != 1; }
};

}  // namespace order_analyse
