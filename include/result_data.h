#pragma once
#include <fstream>
#include <iostream>
#include <map>
#include <string>

#include "order_list.h"

namespace order_analyse {

class ResultData {
 private:
  std::map<std::string, std::map<std::string, int>> main2sub2size;
  std::string main_header_;
  std::vector<std::string> sub_headers_;

 public:
  ResultData(const std::string& main_name) { main_header_ = main_name; }
  std::string main_header() { return main_header_; }
  void insert(const std::string& sub_header,
              const std::map<std::string, int>& main2size) {
    sub_headers_.emplace_back(sub_header);
    for (const auto& m2s : main2size)
      main2sub2size[m2s.first][sub_header] = m2s.second;
  }
  bool OutPut(std::ostream& stream) {
    stream << main_header_;
    for (const auto& header : sub_headers_) {
      stream << "," << header;
    }
    stream << std::endl;
    for (const auto& map : main2sub2size) {
      stream << map.first;
      for (const auto& header : sub_headers_)
        stream << "," << map.second.find(header)->second;
      stream << std::endl;
    }

    return true;
  }
  bool compare(std::string header1, std::string header2) {
    bool correct = true;
    for (const auto& map : main2sub2size) {
      if (map.second.find(header1)->second < map.second.find(header2)->second) {
        std::cout << map.first << ":" << map.second.find(header1)->second << ","
                  << map.second.find(header2)->second << std::endl;
        correct = false;
      }
    }
    return correct;
  }
};

}  // namespace order_analyse
