#pragma once
#include "sheet.h"

namespace order_analyse {

class Calculator {
 private:
 public:
  virtual Sheet Calculate(const Sheet& sheet,
                          const std::vector<std::string>& headers) = 0;
  virtual bool ArgumentsLimiter(int vec_size) = 0;
};

}  // namespace order_analyse
