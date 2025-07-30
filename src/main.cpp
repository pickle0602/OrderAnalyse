#include <chrono>
#include <fstream>
#include <iostream>
#include <string>

#include "csv_reader.h"
#include "distinct_counter.h"
#include "matching_counter.h"
#include "order_list.h"

int main(int argc, char** argv) {
  std::string input_file;

  std::ifstream fin(argv[1]);
  if (not fin.is_open()) {
    std::cout << "File not found" << std::endl;
    return -1;
  }
  auto start1 = std::chrono::high_resolution_clock::now();

  order_analyse::CsvReader reader(fin);
  order_analyse::Sheet sheet = reader.Read();

  order_analyse::OrderList order_list(sheet.rows(), sheet.headers());

  auto end1 = std::chrono::high_resolution_clock::now();
  auto duration1 =
      std::chrono::duration_cast<std::chrono::microseconds>(end1 - start1);
  std::cout << duration1.count() << std::endl;

  std::string main;
  std::string sub1;
  std::string sub2;

  std::getline(std::cin, main);
  std::getline(std::cin, sub1);
  std::getline(std::cin, sub2);

  auto start = std::chrono::high_resolution_clock::now();
  order_analyse::MatchingCounter counter(order_list, main, sub1,
                                         sub2);  // 优化
  auto end = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(end - start);
  std::cout << duration.count() << std::endl;
  counter.out_put_test();
}