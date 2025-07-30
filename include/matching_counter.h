#pragma once
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <vector>

#include "order_list.h"


namespace order_analyse {

class MatchingCounter {
  struct set_package_ {
    std::string main;
    std::set<std::string> sub1;
    std::set<std::string> sub2;
  };

 private:
  std::vector<set_package_> set_packages_;
  std::string main_header_;
  std::string sub1_header_;
  std::string sub2_header_;

 public:
  MatchingCounter(const OrderList& order_list, const std::string& main_name,
                  const std::string& sub1_name, const std::string& sub2_name) {
    int main_index = 0;
    int sub1_index = 0;
    int sub2_index = 0;
    int temp_index = 0;
    main_header_ = main_name;
    sub1_header_ = sub1_name;
    sub2_header_ = sub2_name;
    for (const auto& header : order_list.header()) {
      if (header == main_header_) main_index = temp_index;
      if (header == sub1_header_) sub1_index = temp_index;
      if (header == sub2_header_) sub2_index = temp_index;
      temp_index++;
    }
    bool flag;
    for (const auto& order : order_list.orders()) {
      flag = false;
      for (auto package = set_packages_.rbegin();
           package != set_packages_.rend(); package++) {
        if (package->main == order.cell()[main_index]) {
          flag = true;
          package->sub1.insert(order.cell()[sub1_index]);
          package->sub2.insert(order.cell()[sub2_index]);
          break;
        }
      }
      if (flag == false) {
        set_package_ temp_package;
        temp_package.main = order.cell()[main_index];
        temp_package.sub1.insert(order.cell()[sub1_index]);
        temp_package.sub2.insert(order.cell()[sub2_index]);
        set_packages_.emplace_back(temp_package);
      }
    }
  }
  void out_put_test() {
    std::cout << main_header_ << "," << sub1_header_ << "," << sub2_header_
              << std::endl;
    for (const auto& package : set_packages_) {
      std::cout << package.main << "," << package.sub1.size() << ","
                << package.sub2.size() << std::endl;
    }
  }
  bool out_put() {
    std::ofstream out_put_file("OutPut.csv");
    if (!out_put_file.is_open()) {
      std::cerr << "Opening Failed" << std::endl;
      return false;
    }

    out_put_file << main_header_ << "," << sub1_header_ << "," << sub2_header_
                 << std::endl;
    for (const auto& package : set_packages_) {
      out_put_file << package.main << "," << package.sub1.size() << ","
                   << package.sub2.size() << std::endl;
    }
  }
};

}  // namespace order_analyse
