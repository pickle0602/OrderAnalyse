#pragma once
#include <map>
#include <string>
#include <vector>

namespace order_analyse {

class Order {
 private:
  std::map<std::string, std::string> cells_;

 public:
  Order(const std::vector<std::string>& headers,
        const std::vector<std::string>& row) {
    auto row_ptr = row.begin();
    for (auto header : headers) {
      cells_.emplace(header, *row_ptr);
      row_ptr++;
    }
  }
  std::map<std::string, std::string> cell() { return cells_; }
};

}  // namespace order_analyse
