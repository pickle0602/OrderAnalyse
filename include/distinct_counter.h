#pragma once
#include <map>
#include <set>
#include <string>

#include "calculator.h"

namespace order_analyse {

class DistinctCounter : public Calculator {
 private:
 public:
<<<<<<< HEAD
  Sheet Calculate(const Sheet& sheet, const std::vector<std::string>& headers) {
    if (ArgumentsLimiter(headers.size())) {
      Sheet bad_sheet;
      bad_sheet.SetHeader(std::vector<std::string>{"Error:Bad_Sheet!"});
      bad_sheet.SetRow(std::vector<std::string>{"Error:Bad_Sheet!"});
      return bad_sheet;
=======
  std::map<std::string, int> Calculate(const OrderList& order_list,
                                       const std::vector<std::string> headers) {
    if (arguments_limiter(headers.size())) {
      std::map<std::string, int> error_map;
      error_map["Error"] = -1;
      return error_map;
    }
    std::set<std::string> contents;
    int index = order_list.index(headers[0]);
    for (const auto& order : order_list.orders()) {
      contents.insert(order.cell()[index]);
>>>>>>> origin/new-master
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
<<<<<<< HEAD
  bool ArgumentsLimiter(int vec_size) { return vec_size != 1; }
=======
  bool arguments_limiter(int vec_size) { return vec_size != 1; }
>>>>>>> origin/new-master
};

}  // namespace order_analyse
