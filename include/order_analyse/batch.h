#pragma once
#include <algorithm>
#include <iostream>
#include <list>
#include <map>
#include <random>
#include <set>
#include <string>
#include <vector>

namespace order_analyse {
class Batch {
 private:
  std::list<std::pair<std::string, std::set<std::string>>> orders_;
  std::set<std::string> skus_;
  int num_;

 public:
  Batch(int const& n) : num_(n) {}
  bool insert(
      std::list<std::pair<std::string, std::set<std::string>>>::iterator const&
          it) {
    if (orders_.size() >= num_) return false;
    if (!std::includes(skus_.begin(), skus_.end(), it->second.begin(),
                       it->second.end()))
      for (auto const& item : it->second) skus_.insert(item);
    orders_.emplace_back(*it);
    return true;
  }
  std::list<std::pair<std::string, std::set<std::string>>> const& orders()
      const {
    return orders_;
  }
  std::set<std::string> const& skus() const { return skus_; }
};
}  // namespace order_analyse
