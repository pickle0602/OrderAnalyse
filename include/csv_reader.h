#pragma once
#include <string>
#include <fstream>
#include "sheet.h"

namespace order_analyse {

class CsvReader {
 private:
 std::ifstream& file_;
 public:
  CsvReader(std::ifstream& fin) : file_(fin) {
  }
  Sheet Read() {
    file_.seekg(0, std::ios::beg);
    Sheet sheet;
    std::string str;
    std::getline(file_,str);
    sheet.SetHeader(SplitRow(str));
    while(std::getline(file_,str) != NULL)
      sheet.SetRow(SplitRow(str));
    return sheet;
  }
  std::vector<std::string> SplitRow(const std::string& str_row) {
    int pos1=0;
    int pos2=0;
    std::string s;
    std::vector<std::string> row;
    while(std::string::npos != pos2) {
        pos1=pos2+1;
        pos2=str_row.find(",",pos2+1);
        s=str_row.substr(pos1,pos2-pos1);
        row.push_back(s);
    }
    return row;
}
};

}  // namespace order_analyse

