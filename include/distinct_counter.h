#pragma once
#include <map>
#include <set>
#include <string>

#include "calculator.h"

namespace order_analyse {

class DistinctCounter : public Calculator {
 private:
 public:
  Sheet Calculate(const Sheet& sheet, const std::vector<std::string>& headers) {
    if (ArgumentsLimiter(headers.size())) {
      Sheet bad_sheet;
      bad_sheet.SetHeader(std::vector<std::string>{"Error:Bad_Sheet!"});
      bad_sheet.SetRow(std::vector<std::string>{"Error:Bad_Sheet!"});
      return bad_sheet;
    }
    std::set<std::string> contents;
    int index = sheet.index(headers[0]);
    if (index == -1) {
      Sheet bad_sheet;
      bad_sheet.SetHeader(std::vector<std::string>{"0"});
      bad_sheet.SetRow(std::vector<std::string>{"0"});
      return bad_sheet;
    }

    for (const auto& row : sheet.rows()) {
      contents.insert(row[index]);
    }
    Sheet result;
    result.SetHeader(headers);
    result.SetRow(std::vector<std::string>{std::to_string(contents.size())});
    return result;
  }
  bool ArgumentsLimiter(int vec_size) { return vec_size != 1; }
};

}  // namespace order_analyse
