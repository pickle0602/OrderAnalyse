#pragma once
#include "calculator.h"
#include "order.h"

namespace order_analyse {

class DistinctCounter : public Calculator {
 private:
 public:
  int Calculate(const std::vector<Order>& orders, const std::string& header) {
    int counter = 0;
    std::vector<std::string> contents;
    bool flag;
    for (auto order : orders) {
      flag = false;
      if (order.cell().count(header) == 1) {
        flag = true;
        for (auto content = contents.rbegin(); content != contents.rend();
             ++content) {
          if (order.cell()[header] == *content) {
            flag = false;
            break;
          }
        }
      }

      if (flag == true) {
        contents.emplace_back(order.cell()[header]);
        counter++;
      }
    }
    return counter;
  }
};

}  // namespace order_analyse
