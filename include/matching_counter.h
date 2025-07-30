#pragma once
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>

#include "order_list.h"

namespace order_analyse {

class MatchingCounter : public Calculator {
 private:
  std::map<std::string, std::map<std::string, std::set<std::string>>> set_map_;
  std::string main_header_;
  std::vector<std::string> sub_headers_;

 public:
  MatchingCounter(const std::string& main_name) { main_header_ = main_name; }
  int Calculate(const OrderList& order_list, const std::string& sub_name) {
    sub_headers_.emplace_back(sub_name);
    size_t main_index = order_list.index(main_header_);
    size_t sub_index = order_list.index(sub_name);
    for (const auto& order : order_list.orders())
      set_map_[order.cell()[main_index]][sub_name].insert(
          order.cell()[sub_index]);
  }
  bool OutPut(std::string file_name = "") {
    auto cout_buf = std::cout.rdbuf();
    std::ofstream OutFile(file_name);
    if (file_name != "") {
      if (!OutFile) {
        std::cerr << "Open Failed！" << std::endl;
        return false;
      }
      std::cout.rdbuf(OutFile.rdbuf());
    }
    std::cout << main_header_;
    for (const auto& header : sub_headers_) {
      std::cout << "," << header;
    }
    std::cout << std::endl;
    for (const auto& map : set_map_) {
      std::cout << map.first;
      for (const auto& header : sub_headers_)
        std::cout << "," << map.second.at(header).size();
      std::cout << std::endl;
    }
    std::cout.rdbuf(cout_buf);
    return true;
  }
  bool compare(std::string header1, std::string header2) {
    bool correct = true;
    for (const auto& map : set_map_) {
      if (map.second.at(header1).size() < map.second.at(header2).size()) {
        std::cout << map.first << ":" << map.second.at(header1).size() << ","
                  << map.second.at(header2).size() << std::endl;
        correct = false;
      }
    }
    return correct;
  }
};

}  // namespace order_analyse
