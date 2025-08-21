#pragma once
#include <map>
#include <set>
#include <string>
#include <vector>

#include "calculator.h"
#include "sheet.h"

namespace order_analyse {

class MatchingCounter : public Calculator {
 private:
 public:
  Sheet Calculate(const Sheet& sheet, const std::vector<std::string>& headers) {
    if (ArgumentsLimiter(headers.size())) {
      Sheet bad_sheet;
      bad_sheet.SetHeader(std::vector<std::string>{"Error:Bad_Sheet!"});
      bad_sheet.SetRow(std::vector<std::string>{"Error:Bad_Sheet!"});
      return bad_sheet;
    }
    Sheet result;
    result.SetHeader(headers);
    std::vector<size_t> indexs;
    for (const auto& header : headers) {
      int temp = sheet.index(header);
      if (temp == -1) {
        Sheet bad_sheet;
        bad_sheet.SetHeader(std::vector<std::string>{"Error:Bad_Sheet!"});
        bad_sheet.SetRow(std::vector<std::string>{"Error:Bad_Sheet!"});
        return bad_sheet;
      }
      indexs.emplace_back(temp);
    }
    std::vector<std::vector<std::string>> temp_sheet;
    bool main_set = false;
    int j = 0;
    for (int i = 1; i < headers.size(); i++) {
      std::map<std::string, std::set<std::string>> main2set;
      for (const auto& row : sheet.rows())
        main2set[row[indexs[0]]].insert(row[indexs[i]]);

      std::map<std::string, std::string> main2size;
      if (!main_set)
        for (const auto& m2s : main2set) {
          temp_sheet.emplace_back(std::vector<std::string>{m2s.first});
          j++;
        }
      main_set = true;
      j = 0;
      for (const auto& m2s : main2set) {
        temp_sheet[j].emplace_back(std::to_string(m2s.second.size()));
        j++;
      }
    }
    for (const auto& row : temp_sheet) {
      result.SetRow(row);
    }
    return result;
  }

  bool ArgumentsLimiter(int vec_size) { return vec_size < 2; }

  bool stoi_no_exception(std::string str, int& num) {
    try {
      int n = std::stoi(str);
      num = n;
      return true;
    } catch (const std::invalid_argument) {
      return false;
    }
  }

  Sheet Filter_LT(const Sheet& sheet, const std::string& main,
                  const std::string& header1, const std::string& header2) {
    int m_index = sheet.index(main);
    int h1_index = sheet.index(header1);
    int h2_index = sheet.index(header2);
    int h1;
    int h2;
    Sheet result;
    result.SetHeader(std::vector<std::string>{main, header1, header2});
    for (const auto& row : sheet.rows()) {
      if (m_index == -1 || h1_index == -1 || h2_index == -1 ||
          !stoi_no_exception(row[h1_index], h1) ||
          !stoi_no_exception(row[h2_index], h2)) {
        Sheet bad_sheet;
        bad_sheet.SetHeader(std::vector<std::string>{"Error:Bad_Sheet!"});
        bad_sheet.SetRow(std::vector<std::string>{"Error:Bad_Sheet!"});
        return bad_sheet;
      }
      if (h1 < h2)
        result.SetRow(std::vector<std::string>{row[m_index], row[h1_index],
                                               row[h2_index]});
    }
    return result;
  }
};

}  // namespace order_analyse
