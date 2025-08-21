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
  order_analyse::CsvDealer csv;
  order_analyse::Sheet sheet(csv.InPut(file));
  std::list<order_analyse::Batch> result;
  order_analyse::Batcher batcher;
  std::ofstream out_put;
  double SPB;
  if (argc > 2) {
    std::string argv2 = argv[2];
    if (argv2 == "r" || argv2 == "random" || argv2 == "Random") {
      out_put.open("random.csv");

      SPB = batcher.RandomBatch(
          sheet, std::vector<std::string>{"Order ID(M)", "Item Code(M)"}, 10,
          result);
    }
  } else {
    out_put.open("output.csv");
    SPB = batcher.SortBatch(
        sheet, std::vector<std::string>{"Order ID(M)", "Item Code(M)"}, 10,
        result);
  }
  order_analyse::Sheet out_sheet;
  out_sheet.SetHeader(
      std::vector<std::string>{"Batch ID(M)", "Order ID(M)", "Item Code(M)"});
  int batch_num = 0;
  for (auto const& batch : result) {
    std::vector<std::string> row;
    for (auto const& order : batch.orders()) {
      for (auto const& item : order.second) {
        row.emplace_back(std::to_string(batch_num));
        row.emplace_back(order.first);
        row.emplace_back(item);
        out_sheet.SetRow(row);
      }
    }
  }
  csv.OutPut(out_put, out_sheet);
  std::cout << SPB << "SKUs Per Batch" << std::endl;
  return 0;
}
