#pragma once
#include "calculator.h"
#include "order.h"

namespace order_analyse {

class DistinctCounter : public Calculator {
 private:
 public:
   int Calculate(const std::vector<Order>& orders , std::string header) { 
    int counter;
    std::vector<std::string> contents;
    for(auto order : orders) {
      std::cout<<order.cell()[header]<<std::endl;
      bool flag=false;
      for(auto content = contents.rend(); content != contents.rbegin(); --content) {
        if(order.cell()[header] == *content) {
          contents.emplace_back(order.cell()[header]);
          break;
        }
        if(flag==true)
          counter++;
      }
    }
    return counter; 
  }
};

}  // namespace order_analyse

