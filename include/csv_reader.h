#pragma once
#include <fstream>
#include <iostream>
#include <string>

#include "sheet.h"

namespace order_analyse {

class CsvReader {
 private:
  std::ifstream& file_;

 public:
  CsvReader(std::ifstream& fin) : file_(fin) {}
  Sheet Read() {
    file_.seekg(0, std::ios::beg);
    Sheet sheet;
    std::string str;
    std::getline(file_, str);
    sheet.SetHeader(SplitRow(str));
    while (std::getline(file_, str))
      if (!sheet.SetRow(SplitRow(str))) {
        Sheet bad_sheet;
        bad_sheet.SetHeader(std::vector<std::string>{"Error:Bad Sheet!"});
        bad_sheet.SetRow(std::vector<std::string>{"Error:Bad Sheet!"});
        return bad_sheet;
      }
    return sheet;
  }
  std::vector<std::string> SplitRow(const std::string& str_row) {
    int pos1 = 0;
    int pos2 = 0;
    std::string s;
    std::vector<std::string> row;
    while (std::string::npos != pos2) {
      pos2 = str_row.find(",", pos2);
      s = str_row.substr(pos1, pos2 - pos1);
      row.push_back(s);
      if (pos2 != std::string::npos) pos2++;
      pos1 = pos2;
    }
    return row;
  }
};

}  // namespace order_analyse
