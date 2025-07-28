#pragma once
#include <vector>
#include <string>

namespace order_analyse {

class Sheet {
 private:
   std::vector<std::string> headers_;//表头
   std::vector<std::vector<std::string>> rows_;//二维容器

 public:
   void SetHeader(const std::vector<std::string>& header) {
    headers_ = header;
   }
   void SetRow(const std::vector<std::string>& row) {
    rows_.emplace_back(row);
   }
   const std::vector<std::string>& headers() { return headers_; }
   const std::vector<std::vector<std::string>>& rows() { return rows_; }
};

}  // namespace order_analyse

