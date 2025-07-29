#pragma once
#include <string>
#include <unordered_map>
#include <vector>

namespace order_analyse {

class Order {
 private:
  std::unordered_map<int, std::string> cells_;
  std::unordered_map<std::string, int> headers_;

 public:
  Order(const std::vector<std::string>& headers,
        const std::vector<std::string>& row) {
    auto row_ptr = row.begin();
    int code = 0;
    for (auto header : headers) {
      headers_.emplace(header, code);
      cells_.emplace(code, *row_ptr);
      row_ptr++;
      code++;
    }
  }
  std::unordered_map<int, std::string> cell() const { return cells_; }
  std::unordered_map<std::string, int> header() const { return headers_; }
};

}  // namespace order_analyse
