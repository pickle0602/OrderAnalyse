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
  size_t index(std::string header) const {
    size_t index = 0;
    for (const auto& temp_header : headers_) {
      if (temp_header == header) return index;
      index++;
    }
    return -1;
  }
  const auto& header() const { return headers_; }
  const auto& orders() const { return orders_; }
};

}  // namespace order_analyse
