#pragma once
#include <algorithm>
#include <iostream>
#include <list>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "batch.h"
#include "batcher.h"

namespace order_analyse {
class BatchStaging {
 private:
  std::list<std::list<std::pair<std::string, std::set<std::string>>>::iterator>
      orders_;
  std::set<std::string> skus_;
  int num_;

 public:
  BatchStaging(int const& n) : num_(n) {}
  int num() const { return num_; }
  bool insert(
      std::list<std::pair<std::string, std::set<std::string>>>::iterator const&
          it) {
    if (orders_.size() >= num_) return false;
    if (!std::includes(skus_.begin(), skus_.end(), it->second.begin(),
                       it->second.end()))
      for (auto const& item : it->second) skus_.insert(item);
    orders_.emplace_back(it);
    return true;
  }
  bool includes(
      std::list<std::pair<std::string, std::set<std::string>>>::iterator const&
          it) const {
    return std::includes(skus_.begin(), skus_.end(), it->second.begin(),
                         it->second.end());
  }
  std::set<std::string> const& skus() const { return skus_; }

  std::list<
      std::list<std::pair<std::string, std::set<std::string>>>::iterator> const&
  orders() const {
    return orders_;
  }
  std::set<std::string> diff(

      std::list<std::pair<std::string, std::set<std::string>>>::iterator it)
      const {
    std::set<std::string> diff;
    std::set_symmetric_difference(skus_.begin(), skus_.end(),
                                  it->second.begin(), it->second.end(),
                                  std::inserter(diff, diff.begin()));
    return diff;
  }
};

}  // namespace order_analyse