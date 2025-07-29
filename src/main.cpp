#include <chrono>
#include <fstream>
#include <iostream>
#include <string>

#include "csv_reader.h"
#include "distinct_counter.h"

int main(int argc, char** argv) {
  std::cout << "Type file's name" << std::endl;
  std::string input_file;
  std::getline(std::cin, input_file);

  std::ifstream fin(input_file);
  if (not fin.is_open()) {
    std::cout << "File not found" << std::endl;
    return -1;
  }
  auto start1 = std::chrono::high_resolution_clock::now();

  order_analyse::CsvReader reader(fin);
  order_analyse::Sheet sheet = reader.Read();

  order_analyse::DistinctCounter counter;

  std::vector<order_analyse::Order> orders;  // 本质结构

  for (const auto& row : sheet.rows())
    orders.emplace_back(order_analyse::Order(sheet.headers(), row));
  auto end1 = std::chrono::high_resolution_clock::now();
  auto duration1 =
      std::chrono::duration_cast<std::chrono::microseconds>(end1 - start1);
  std::cout << duration1.count() << std::endl;

  std::cout << "Type header's name" << std::endl;
  std::string header_name;
  std::getline(std::cin, header_name);
  auto start = std::chrono::high_resolution_clock::now();
  std::cout << counter.Calculate(orders, header_name) << std::endl;  // 优化
  auto end = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(end - start);
  std::cout << duration.count() << std::endl;
}