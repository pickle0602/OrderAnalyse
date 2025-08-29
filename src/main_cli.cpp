#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <vector>

#include "batch.h"
#include "batcher.h"
#include "calculator.h"
#include "csv_dealer.h"
#include "distinct_counter.h"
#include "matching_counter.h"
#include "sheet.h"

int main(int argc, char** argv) {
  std::ifstream file;
  if (argc > 1)
    file.open(argv[1]);
  else
    file.open("data.csv");
  if (!file.is_open()) {
    std::cout << "No such file!\n";
    return 1;
  }
  int OPB = 10;
  if (argc > 2) {
    std::string argv2 = argv[2];
    OPB = std::stoi(argv2);
  }
  order_analyse::CsvDealer csv;
  order_analyse::Sheet sheet(csv.InPut(file));
  std::list<order_analyse::Batch> result;
  order_analyse::Batcher batcher;
  std::ofstream out_put;

  double SPB;
  if (argc > 3) {
    std::string argv3 = argv[3];
    if (argv3 == "r" || argv3 == "random" || argv3 == "Random") {
      out_put.open("random.csv");

      result = batcher.RandomBatch(
          sheet, std::vector<std::string>{"Order ID(M)", "Item Code(M)"}, OPB);
    } else if (argv3 == "cp" || argv3 == "c" || argv3 == "CP") {
      out_put.open("CP.csv");
      result = batcher.CPBatch(
          sheet, std::vector<std::string>{"Order ID(M)", "Item Code(M)"}, OPB);
    }
  } else {
    out_put.open("output.csv");
    result = batcher.SortBatch(
        sheet, std::vector<std::string>{"Order ID(M)", "Item Code(M)"}, OPB);
  }
  order_analyse::Sheet out_sheet;
  out_sheet.SetHeader(
      std::vector<std::string>{"Batch ID(M)", "Order ID(M)", "Item Code(M)"});
  int batch_num = 0;
  double SKUs_sum = 0;
  for (auto const& batch : result) {
    std::set<std::string> SKUs;
    for (auto const& order : batch.orders()) {
      for (auto const& item : order.second) {
        std::vector<std::string> row;
        SKUs.insert(item);
        row.emplace_back(std::to_string(batch_num));
        row.emplace_back(order.first);
        row.emplace_back(item);
        out_sheet.SetRow(row);
      }
    }
    SKUs_sum += SKUs.size();
    batch_num++;
  }
  SPB = SKUs_sum / batch_num;
  csv.OutPut(out_put, out_sheet);
  std::cout << SPB << "SKUs Per Batch" << std::endl;
  return 0;
}
