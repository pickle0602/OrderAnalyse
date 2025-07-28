#pragma once
#include <vector>
#include <string>

namespace order_analyse {

class Sheet {
 private:
   std::vector<std::string> headers_;
   std::vector<std::vector<std::string>> rows_;

 public:
   const std::vector<std::string>& header() { return headers_; }
   const std::vector<std::vector<std::string>>& rows() { return rows_; }
};

}  // namespace order_analyse

