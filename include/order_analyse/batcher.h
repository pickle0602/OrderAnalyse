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
#include "ortools/sat/cp_model.h"
#include "ortools/sat/cp_model.pb.h"
#include "ortools/sat/cp_model_solver.h"
#include "ortools/util/logging.h"
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

  std::list<Batch> SortBatch(Sheet const& origin,  // 排序Batch
                             std::vector<std::string> const& headers, int num,
                             std::vector<std::vector<bool>>& mapping_tab) {
    std::list<Batch> batches;
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
    std::vector<std::string> order_ids;
    for (auto const& m2s : main2set) order_ids.emplace_back(m2s.first);
    std::map<std::string, int> order_map;
    {
      int i = 0;
      for (auto const& order_id : order_ids) {
        order_map[order_id] = i;
        i++;
      }
    }

    for (int i = 0; i < main2set.size(); i++) {
      std::vector<bool> vec;
      for (int j = 0; j < (main2set.size() - 1) / num + 1; j++) {
        vec.emplace_back(false);
      }
      mapping_tab.emplace_back(vec);
    }
    auto merge_begin = batches.end();
    merge_begin--;
    for (int merge_num = 0; main2set.size() != 0 && merge_num < num;
         merge_num++) {
      std::cout << merge_num << std::endl;
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

        for (; staging.orders().size() != num && m2s != main2set.end() &&
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
          Batch batch = storage(main2set, staging);
          batches.emplace_back(batch);
          batch_head = main2set.begin();
        } else {
          batch_head++;
        }
      }
    }
    std::vector<Batch> merged_batches;
    merge_begin++;
    for (auto batch = merge_begin; batch != batches.end(); batch++)
      merged_batches.emplace_back(*batch);
    std::sort(merged_batches.begin(), merged_batches.end(),
              [this](auto const& a, auto const& b) { return batch_cmp(a, b); });
    int j = 0;
    for (auto const& batch : merged_batches) {
      for (auto const& order : batch.orders()) {
        mapping_tab[order_map[order.first]][j] = true;
      }
      j++;
    }
    return batches;
  }
  std::list<Batch> RandomBatch(Sheet const& origin,  // 随机Batch
                               std::vector<std::string> const& headers,
                               int num) {
    std::list<Batch> batches;
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

    return batches;
  }
  Batch storage(std::list<order>& main2set, BatchStaging const& staging) {
    Batch result(staging.num());
    for (auto const& order : staging.orders()) {
      result.insert(order);
      main2set.erase(order);
    }
    return result;
  }
  std::list<Batch> CPBatch(Sheet const& origin,  // 约束规划Batch
                           std::vector<std::string> const& headers, int num,
                           std::vector<std::vector<bool>>& mapping_tab) {
    std::list<Batch> batches;
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

    const int n = main2set.size();
    const int k = (n - 1) / num + 1;

    // 获取所有元素的并集（全集U）
    std::set<std::string> universe;
    for (const auto& s : main2set) {
      universe.insert(s.second.begin(), s.second.end());
    }
    const int M = universe.size();

    // 创建元素到索引的映射
    std::vector<std::string> elements(universe.begin(), universe.end());

    // 创建模型
    operations_research::sat::CpModelBuilder cp_model;

    // 决策变量 x_ij: 集合i是否分配到组j
    std::vector<std::vector<operations_research::sat::BoolVar>> x(n);
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < k; ++j) {
        x[i].emplace_back(cp_model.NewBoolVar());
      }
    }

    // 决策变量 y_ej: 元素e是否出现在组j的并集中
    std::vector<std::vector<operations_research::sat::BoolVar>> y(M);
    for (int e = 0; e < M; ++e) {
      for (int j = 0; j < k; ++j) {
        y[e].emplace_back(cp_model.NewBoolVar());
      }
    }
    std::vector<operations_research::sat::BoolVar> all_var;
    for (auto const& x_row : x)
      for (auto const& x_var : x_row) all_var.emplace_back(x_var);
    for (auto const& y_row : y)
      for (auto const& y_var : y_row) all_var.emplace_back(y_var);

    // 策略:先x后y，先0后1
    cp_model.AddDecisionStrategy(
        all_var, operations_research::sat::DecisionStrategyProto::CHOOSE_FIRST,
        operations_research::sat::DecisionStrategyProto::SELECT_MIN_VALUE);

    // 约束1: 每个集合必须被分配到恰好一组
    for (int i = 0; i < n; ++i) cp_model.AddExactlyOne(x[i]);

    // 约束2: 每组恰好包含num个集合
    for (int j = 0; j < k; ++j) {
      operations_research::sat::LinearExpr sum;
      for (int i = 0; i < n; ++i) {
        sum += x[i][j];
      }
      cp_model.AddGreaterOrEqual(num, sum);
    }
    std::vector<order> main2set_vec(main2set.begin(), main2set.end());
    // 约束3: 如果集合i被分配到组j且包含元素e
    for (int e = 0; e < M; ++e) {
      const std::string element = elements[e];
      for (int j = 0; j < k; ++j) {
        for (int i = 0; i < n; ++i) {
          // 检查元素e是否在集合i中
          if (main2set_vec[i].second.find(element) !=
              main2set_vec[i].second.end()) {
            // y_ej >= x_ij
            cp_model.AddGreaterOrEqual(y[e][j], x[i][j]);
          }
        }
      }
    }
    // 约束4:每组元素数从大到小排列
    std::vector<operations_research::sat::LinearExpr> batch_SKUs(k);
    for (int j = 0; j < k; j++) {
      for (int e = 0; e < M; e++) {
        batch_SKUs[j] += y[e][j];
      }
      if (j != 0) cp_model.AddGreaterOrEqual(batch_SKUs[j - 1], batch_SKUs[j]);
    }
    // 约束5:每种元素数总数不大于存在该元素的集合数
    std::map<std::string, int> elements_count;
    for (auto const& m2s : main2set)
      for (auto const& sku : m2s.second) elements_count[sku]++;

    for (int e = 0; e < k; e++) {
      operations_research::sat::LinearExpr SKU_count;
      for (int j = 0; j < k; j++) {
        SKU_count += y[e][j];
      }
      cp_model.AddGreaterOrEqual(elements_count[elements[e]], SKU_count);
    }
    // 约束8:结果不大于启发式解
    operations_research::sat::LinearExpr SKUs;
    int old_SKUs = 0;
    for (int j = 0; j < k; j++) {
      for (int e = 0; e < M; e++) {
        SKUs += y[e][j];
        old_SKUs += mapping_tab[e][j];
      }
      cp_model.AddGreaterOrEqual(old_SKUs, SKUs);
    }
    // 约束6:同一组中相同集合数要么为全部相同集合总量，要么该组不存在小于该集合大小的集合
    // std::map<std::set<std::string>, int> set_count;
    // for (auto const& m2s : main2set) set_count[m2s.second]++;
    // for (int j = 0; j < k; j++) {
    //   std::map<std::set<std::string>, operations_research::sat::LinearExpr>
    //       batch_set_count;
    //   std::set<std::string> min;
    //   operations_research::sat::IntVar min = cp_model.NewIntVar({ 0, }
    //   "min"); for (int i = 0; i < n; i++)
    //     batch_set_count[main2set_vec[i].second] += x[i][j];
    //   for (auto const& count : batch_set_count) {
    //     cp_model.AddEquality(count.second, set_count[count.first])
    //         .OnlyEnforceIf(min.size() != count.first.size());
    //   }
    // }

    // 约束7:启发式生成映射表作为提示
    if (mapping_tab.size() != 0) {
      for (int i = 0; i < n; i++) {
        for (int j = 0; j < k; j++) {
          cp_model.AddHint(x[i][j], mapping_tab[i][j]);
        }
      }
    }
    // 约束8:首元素递索引增排序
    std::vector<std::vector<operations_research::sat::LinearExpr>> befor(n)(k);
    for (int i = 0; i < n; i++) {
      for (int j = 1; j < k; j++) { 
        for (int i_i = 0; i_i < i; i_i++) befor[i][j] += x[i_i][j-1];
        cp_model.AddGreaterOrEqual(befor, 1).OnlyEnforceIf(x[i][j].Not());
        cp_model.AddLessThan(befor, 1).OnlyEnforceIf(x[i][j]);

      }
    }
    
    // 目标函数: 最小化所有组并集大小的总和
    operations_research::sat::LinearExpr objective;
    for (int e = 0; e < M; ++e) {
      for (int j = 0; j < k; ++j) {
        objective += y[e][j];
      }
    }
    cp_model.Minimize(objective);

    // 求解
    const operations_research::sat::CpSolverResponse response =
        Solve(cp_model.Build());

    // 输出结果
    if (response.status() ==
        operations_research::sat::CpSolverStatus::OPTIMAL) {
      for (int i = 0; i < n; i++)
        for (int j = 0; j < k; j++) mapping_tab[i][j] = false;
      for (int j = 0; j < k; ++j) {
        Batch batch(num);
        for (int i = 0; i < n; ++i) {
          mapping_tab[i][j] = SolutionIntegerValue(response, x[i][j]);
          if (SolutionIntegerValue(response, x[i][j]) == 1) {
            batch.insert(main2set_vec[i]);
          }
        }
        batches.emplace_back(batch);
      }
    } else {
      std::cout << "Not Found.Status:" << CpSolverStatus_Name(response.status())
                << std::endl;
      return std::list<Batch>{};
    }

    return batches;
  }
  bool batch_cmp(Batch const& a, Batch const& b) {
    return a.orders().size() > b.orders().size();
  }
};
}  // namespace order_analyse
