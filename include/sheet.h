#pragma once
#include <string>
#include <vector>

namespace order_analyse {

class Sheet {
 private:
  std::vector<std::string> headers_;
  std::vector<std::vector<std::string>> rows_;

 public:
  void SetHeader(const std::vector<std::string>& header) { headers_ = header; }
  void SetRow(const std::vector<std::string>& row) { rows_.emplace_back(row); }
  const std::vector<std::string>& headers() { return headers_; }
  const std::vector<std::vector<std::string>>& rows() { return rows_; }
};

}  // namespace order_analyse
