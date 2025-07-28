#pragma once
#include <string>
#include <vector>
#include <map>

namespace order_analyse {

class Order {
 private:
 std::map<std::string,std::string> cells_;
 public:
  static Order FromRow(const std::vector<std::string>& headers,
                       const std::vector<std::string>& row) {
    Order order(headers, row);
    return order;
  }
  Order(const std::vector<std::string>& headers,
        const std::vector<std::string>& row) {
    auto row_ptr = row.begin();
    for(auto header : headers) {
      std::string key = header;
      std::string vaule = *row_ptr;
      cells_.emplace(key,vaule);
      row_ptr++;

    }
  }
  std::map<std::string,std::string> cell() {
    return cells_;
  }
};

}  // namespace order_analyse

