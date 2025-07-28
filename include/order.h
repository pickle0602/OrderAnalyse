#pragma once
#include <string>
#include <vector>

namespace order_analyse {

class Order {
 private:
 public:
  static Order FromRow(const std::vector<std::string>& header,
                       const std::vector<std::string>& row) {
    Order order;
    return order;
  }
};

}  // namespace order_analyse

