#pragma once
#include <algorithm>
#include <iostream>
#include <list>
#include <map>
#include <random>
#include <set>
#include <string>
#include <vector>

#include "batch.h"
#include "batch_staging.h"
#include "sheet.h"

namespace order_analyse {

typedef std::pair<std::string, std::set<std::string>> order;

class Batcher {
 private:
 public:
  bool cmp(order const& a, order const& b) {
    if (a.second.size() != b.second.size())
      return a.second.size() > b.second.size();
    return a.second < b.second;
  }

  double SortBatch(Sheet const& origin, std::vector<std::string> const& headers,
                   int num, std::list<Batch>& batches) {
    std::vector<size_t> indexs;
    for (const auto& header : headers) {
      int temp = origin.index(header);
      indexs.emplace_back(temp);
    }
    std::map<std::string, std::set<std::string>> map;
    for (const auto& row : origin.rows())
      map[row[indexs[0]]].insert(row[indexs[1]]).second;
    std::vector<order> temp;
    for (auto const& m2s : map) temp.emplace_back(m2s);
    std::sort(temp.begin(), temp.end(),
              [this](auto const& a, auto const& b) { return cmp(a, b); });
    std::list<order> main2set(temp.begin(), temp.end());
    for (auto head = main2set.begin(); head != main2set.end();) {
      BatchStaging staging(num);
      staging.insert(head);
      head++;
      for (auto it = head; staging.orders().size() != num &&
                           it->second == staging.skus() && it != main2set.end();
           it++) {
        staging.insert(it);
        head = it;
        head++;
      }
      if (staging.orders().size() == num ||
          staging.orders().size() >= main2set.size()) {
        batches.emplace_back(storage(main2set, staging));
        head = main2set.begin();
      }
    }

    for (int merge_num = 0; main2set.size() != 0 && merge_num < num;
         merge_num++) {
      for (auto batch_head = main2set.begin();
           batch_head != main2set.end() && main2set.size() != 0;) {
        BatchStaging staging(num);
        auto head = batch_head;
        staging.insert(head);
        head++;
        for (int merge_count = 0;
             merge_count < merge_num && head != main2set.end();) {
          int size = head->second.size();
          int min = staging.skus().size() * 2 + 1;
          auto min_it = main2set.begin();
          for (auto it = head;
               it != main2set.end() && it->second.size() == size &&
               staging.orders().size() != num;
               it++) {
            if (staging.diff(it).size() < min && it != main2set.end() &&
                !staging.includes(it) &&
                staging.orders().end() == std::find(staging.orders().begin(),
                                                    staging.orders().end(),
                                                    it)) {
              min = staging.diff(it).size();
              min_it = it;
            }
          }
          if (min_it != main2set.begin()) {
            staging.insert(min_it);
            merge_count++;
          } else {
            auto it = head;
            while (it != main2set.end() && it->second.size() == size) it++;
            head = it;
          }
        }
        auto m2s = batch_head;
        m2s++;

        for (std::map<std::set<std::string>, int> sku_count;
             staging.orders().size() != num && m2s != main2set.end() &&
             main2set.size() != 0;
             m2s++) {
          if (staging.includes(m2s) &&
              staging.orders().end() == std::find(staging.orders().begin(),
                                                  staging.orders().end(),
                                                  m2s) &&
              m2s != main2set.end()) {
            staging.insert(m2s);
          }
        }
        if (staging.orders().size() == num ||
            staging.orders().size() >= main2set.size()) {
          batches.emplace_back(storage(main2set, staging));
          batch_head = main2set.begin();
        } else {
          batch_head++;
        }
      }
    }
    double SKU_sum = 0;
    int batch_num = 0;
    for (auto const& batch : batches) SKU_sum += batch.skus().size();
    return SKU_sum / batches.size();
  }
  double RandomBatch(Sheet const& origin,
                     std::vector<std::string> const& headers, int num,
                     std::list<Batch>& batches) {
    std::vector<size_t> indexs;
    for (const auto& header : headers) {
      int temp = origin.index(header);
      indexs.emplace_back(temp);
    }
    std::map<std::string, std::set<std::string>> map;
    for (const auto& row : origin.rows())
      map[row[indexs[0]]].insert(row[indexs[1]]).second;
    std::vector<order> temp;
    for (auto const& m : map) temp.emplace_back(m);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(temp.begin(), temp.end(), gen);
    std::list<order> main2set(temp.begin(), temp.end());
    for (auto head = main2set.begin(); head != main2set.end();) {
      BatchStaging staging(num);
      staging.insert(head);
      head++;
      for (auto it = head;
           staging.orders().size() != num && it != main2set.end(); it++) {
        staging.insert(it);
        head = it;
        head++;
      }
      if (staging.orders().size() == num ||
          staging.orders().size() >= main2set.size()) {
        batches.emplace_back(storage(main2set, staging));
        head = main2set.begin();
      }
    }

    double SKU_sum = 0;
    int batch_num = 0;
    for (auto const& batch : batches) SKU_sum += batch.skus().size();
    return SKU_sum / batches.size();
  }
  Batch storage(std::list<order>& main2set, BatchStaging const& staging) {
    Batch result(staging.num());
    for (auto const& order : staging.orders()) {
      result.insert(order);
      main2set.erase(order);
    }
    return result;
  }
};
}  // namespace order_analyse