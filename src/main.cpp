#include <string>
#include <fstream>
#include <iostream>
#include "../include/csv_reader.h"
#include "../include/distinct_counter.h"

int main(int argc, char** argv) {

  std::string input_file;
  // TODO: read input file from console
  std::ifstream fin(input_file);
  if (not fin.is_open()) return -1;

  order_analyse::CsvReader reader(fin);
  order_analyse::Sheet sheet = reader.Read();

  order_analyse::DistinctCounter counter;

  std::vector<order_analyse::Order> orders;


  for (const auto& row : sheet.rows())
    orders.emplace_back(order_analyse::Order::FromRow(sheet.header(), row));

  std::cout << counter.Calculate(orders) << std::endl;


}
