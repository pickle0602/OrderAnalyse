#pragma once
#include "sheet.h"

namespace order_analyse {

class CsvReader {
 private:
 public:
  CsvReader(std::ifstream& fin) {}
  Sheet Read() {
    Sheet sheet;
    return sheet;
  }
};

}  // namespace order_analyse

