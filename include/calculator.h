#pragma once
#include "sheet.h"

namespace order_analyse {

class Calculator {
 private:
 public:
<<<<<<< HEAD
  virtual Sheet Calculate(const Sheet& sheet,
                          const std::vector<std::string>& headers) = 0;
  virtual bool ArgumentsLimiter(int vec_size) = 0;
=======
  virtual std::map<std::string, int> Calculate(
      const OrderList& order_list, const std::vector<std::string> headers) = 0;
  virtual bool arguments_limiter(int vec_size) = 0;
>>>>>>> origin/new-master
};

}  // namespace order_analyse
