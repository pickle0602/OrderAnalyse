#include <gtest/gtest.h>

#include <fstream>
#include <iostream>
#include <string>

#include "csv_reader.h"
#include "distinct_counter.h"
#include "matching_counter.h"
#include "order_list.h"
#include "result_data.h"

TEST(BadSheet, CSVReader) {
  std::ifstream fin("../test/test1.csv");
  order_analyse::CsvReader reader(fin);
  order_analyse::Sheet sheet = reader.Read();
  EXPECT_EQ(sheet.headers()[0], "Error:Bad Sheet!");
}

TEST(WrongArguments, DistinctCounter) {
  std::ifstream fin("../test/test2.csv");
  order_analyse::CsvReader reader(fin);
  order_analyse::Sheet sheet = reader.Read();
  order_analyse::DistinctCounter counter;
  order_analyse::OrderList order_list(sheet.rows(), sheet.headers());
  EXPECT_EQ(counter.Calculate(order_list,
                              std::vector<std::string>{"1", "2", "3"})["Error"],
            -1);
  EXPECT_EQ(counter.Calculate(
                order_list,
                std::vector<std::string>{"Bin_Location(M)"})["Bin_Location(M)"],
            4);
}

TEST(WrongArguments, MatchingCounter) {
  std::ifstream fin("../test/test2.csv");
  order_analyse::CsvReader reader(fin);
  order_analyse::Sheet sheet = reader.Read();
  order_analyse::MatchingCounter counter;
  order_analyse::OrderList order_list(sheet.rows(), sheet.headers());
  EXPECT_EQ(counter.Calculate(order_list,
                              std::vector<std::string>{"1", "2", "3"})["Error"],
            -1);
  order_analyse::ResultData result_data("Bin_Location(M)");
  result_data.insert(
      "Order_ID(M)",
      counter.Calculate(order_list, std::vector<std::string>{"Bin_Location(M)",
                                                             "Order_ID(M)"}));
  result_data.insert(
      "Batch_ID(O)",
      counter.Calculate(order_list, std::vector<std::string>{"Bin_Location(M)",
                                                             "Batch_ID(O)"}));

  EXPECT_FALSE(result_data.compare("Order_ID(M)", "Batch_ID(O)"));
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
