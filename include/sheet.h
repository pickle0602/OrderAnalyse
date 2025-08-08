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
  bool SetRow(const std::vector<std::string>& row) {
    if (row.size() != headers_.size()) return false;
    rows_.emplace_back(row);
    return true;
  }
<<<<<<< HEAD
  const std::vector<std::string>& headers() const { return headers_; }
  const std::vector<std::vector<std::string>>& rows() const { return rows_; }
  size_t index(const std::string& header) const {
    size_t index = 0;
    for (const auto& temp_header : headers_) {
      if (temp_header == header) return index;
      index++;
    }
    return -1;
  }
=======
  const std::vector<std::string>& headers() { return headers_; }
  const std::vector<std::vector<std::string>>& rows() { return rows_; }
>>>>>>> origin/new-master
};

}  // namespace order_analyse
