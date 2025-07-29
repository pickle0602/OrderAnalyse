#include <fstream>
#include <iostream>
#include <string>

#include "csv_reader.h"
#include "distinct_counter.h"

int main(int argc, char** argv) {
  std::cout << "Type file's name" << std::endl;
  std::string input_file;
  // std::cin >> input_file;
  input_file = "datacharonly.csv";  // testing，need remove.

  std::ifstream fin(input_file);
  if (not fin.is_open()) return -1;

  order_analyse::CsvReader reader(fin);
  order_analyse::Sheet sheet = reader.Read();

  order_analyse::DistinctCounter counter;

  std::vector<order_analyse::Order> orders;

  for (const auto& row : sheet.rows())
    orders.emplace_back(order_analyse::Order(sheet.headers(), row));

  std::cout << "Type header's name" << std::endl;
  std::string header_name;
  std::cin >> header_name;
  std::cout << counter.Calculate(orders, header_name) << std::endl;
}