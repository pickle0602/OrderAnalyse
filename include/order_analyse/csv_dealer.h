#pragma once
#include <iostream>
#include <string>

#include "sheet.h"

namespace order_analyse {

class CsvDealer {
 private:
 public:
  Sheet InPut(std::istream& stream) {
    Sheet sheet;
    std::string str;
    if (std::getline(stream, str)) sheet.SetHeader(SplitRow(str));
    while (std::getline(stream, str))
      if (!sheet.SetRow(SplitRow(str))) {
        Sheet bad_sheet;
        bad_sheet.SetHeader(std::vector<std::string>{"Error:Bad_Sheet!"});
        bad_sheet.SetRow(std::vector<std::string>{"Error:Bad_Sheet!"});
        return bad_sheet;
      }
    return sheet;
  }

  bool OutPut(std::ostream& stream, const Sheet& sheet) {
    if (sheet.headers()[0] == "Error:Bad_Sheet!") return false;
    int i = 0;
    for (const auto& cell : sheet.headers()) {
      if (i > 0)
        stream << ",";
      else
        i++;
      stream << cell;
    }
    stream << std::endl;
    for (const auto& row : sheet.rows()) {
      i = 0;
      for (const auto& cell : row) {
        if (i > 0)
          stream << ",";
        else
          i++;
        stream << cell;
      }
      stream << std::endl;
    }
    return true;
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
