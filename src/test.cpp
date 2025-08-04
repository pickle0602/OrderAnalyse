#include <fstream>
#include <iostream>
#include <string>

#include "csv_reader.h"
#include "distinct_counter.h"
#include "gtest/gtest.h"
#include "matching_counter.h"
#include "order_list.h"
#include "result_data.h"

int main(int argc, char** argv) {
  std::ifstream file1("wasd");
  std::ifstream file2("data.csv");
  order_analyse::CsvReader reader1(file1);
  order_analyse::Sheet sheet1 = reader1.Read();
  order_analyse::CsvReader reader2(file2);
  order_analyse::Sheet sheet2 = reader2.Read();
  std::vector<std::string> temp;
  order_analyse::OrderList order_list1(sheet2.rows(), temp);
  order_analyse::OrderList order_list2(sheet2.rows(), sheet2.headers());
  order_analyse::MatchingCounter counter;
  order_analyse::DistinctCounter counterD;
  order_analyse::ResultData result_data1("wasd");
  order_analyse::ResultData result_data2("Bin_Location(M)");
  std::cout << counterD.Calculate(order_list1,
                                  std::vector<std::string>{"123"})["123"]
            << std::endl;
  std::cout << counterD.Calculate(order_list1,
                                  std::vector<std::string>{"123"})["345"]
            << std::endl;
  std::cout << counterD.Calculate(
                   order_list2,
                   std::vector<std::string>{"Order_ID(M)"})["Order_ID(M)"]
            << std::endl;
  result_data1.insert(
      "1234",
      counter.Calculate(order_list1, std::vector<std::string>{"543", "qwer"}));
  result_data1.insert(
      "1234",
      counter.Calculate(order_list2, std::vector<std::string>{"543", "qwer"}));
  result_data2.insert(
      "Batch_ID(O)",
      counter.Calculate(order_list2, std::vector<std::string>{"Bin_Location(M)",
                                                              "Batch_ID(O)"}));
  result_data2.insert(
      "Order_ID(M)",
      counter.Calculate(order_list2, std::vector<std::string>{"Bin_Location(M)",
                                                              "Order_ID(M)"}));
  result_data1.OutPut(std::cout);
  result_data2.OutPut(std::cout);
  result_data1.compare("Batch_ID(O)", "Order_ID(M)");
  result_data2.compare("Batch_ID(O)", "Order_ID(M)");
  result_data2.compare("Order_ID(M)", "Batch_ID(O)");
}
