#pragma once

#include "calculator.h"
#include "order.h"

namespace order_analyse {

class OrderList {
 private:
  std::vector<std::string> headers_;
  std::vector<Order> orders_;

 public:
  void InitHeaders(const std::vector<std::string>& headers) {
    headers_ = headers;
  }
  OrderList(const std::vector<std::vector<std::string>>& rows,
            const std::vector<std::string>& headers) {
    headers_ = headers;
    for (auto row : rows) {
      orders_.emplace_back(Order(row));
    }
  }
  const auto& header() const { return headers_; }
  const auto& orders() const { return orders_; }
};

}  // namespace order_analyse
