#include <chrono>
#include <fstream>
#include <iostream>
#include <string>

#include "csv_reader.h"
#include "distinct_counter.h"
#include "matching_counter.h"
#include "order_list.h"
#include "result_data.h"

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

  std::string main = argv[2];
  std::string sub1 = argv[3];
  std::string sub2 = argv[4];
  std::string file_name = "";
  if (argc == 6) file_name = argv[5];

  auto start = std::chrono::high_resolution_clock::now();
  order_analyse::MatchingCounter counter;
  order_analyse::ResultData result_data(main);
  result_data.insert(
      sub1,
      counter.Calculate(order_list, std::vector<std::string>{main, sub1}));
  result_data.insert(
      sub2,
      counter.Calculate(order_list, std::vector<std::string>{main, sub2}));
  auto end = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(end - start);
  std::cout << duration.count() << std::endl;

  auto cout_buf = std::cout.rdbuf();
  std::ofstream OutFile(file_name);
  if (file_name != "") std::cout.rdbuf(OutFile.rdbuf());
  result_data.OutPut(std::cout);
  std::cout.rdbuf(cout_buf);

  result_data.compare(sub1, sub2);
}
