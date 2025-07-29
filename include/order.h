#pragma once
#include <string>
#include <unordered_map>
#include <vector>

namespace order_analyse {

class Order {
 private:
  std::vector<std::string> cells_;

 public:
  Order(const std::vector<std::string>& row) { cells_ = row; }
  const auto& cell() { return cells_; }
};
}  // namespace order_analyse
