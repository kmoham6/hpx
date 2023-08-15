#include <hpx/local/init.hpp>
#include <hpx/modules/algorithms.hpp>
#include <hpx/modules/program_options.hpp>
#include <hpx/modules/testing.hpp>
#include <hpx/parallel/algorithms/adjacent_difference.hpp>
#include <algorithm>
#include <array>
#include <chrono>
#include <cstddef>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <numeric>
#include <random>
#include <string>
#include <vector>

// std::vector<std::string> generateStrings(std::size_t size, const std::string& pattern) {
//   std::vector<std::string> arr(size);
//   for (std::size_t i = 0; i < size; ++i) {
//     arr[i] = pattern + std::to_string(i + 1);
//   }
//   return arr;
// }


void measureAdjacent_differenceAlgorithms() {

  // auto  chunk_size = 4;

  std::size_t start = 1000000;
  // std::size_t till = 1 << 23;
  
std::vector<int> res(start);
   std::vector<int> arr(start);
   std::iota(std::begin(arr), std::end(arr), 1);
  
   auto t1 = std::chrono::high_resolution_clock::now();
      hpx::adjacent_difference(hpx::execution::seq, arr.begin(), arr.end(),
                               res.begin());

      auto end1 = std::chrono::high_resolution_clock::now();
      
auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end1 - t1).count();
    double time_per_iteration = static_cast<double>(duration) / start;
    
    std::cout << "Total time taken: " << duration << " nanoseconds" << std::endl;
    std::cout << "Time taken per iteration: " << time_per_iteration << " nanoseconds" << std::endl;

  
  //   }
}
int hpx_main(hpx::program_options::variables_map &) {
  measureAdjacent_differenceAlgorithms();

  return hpx::local::finalize();
}
int main(int argc, char *argv[]) {
  std::vector<std::string> cfg;
  cfg.push_back("hpx.os_threads=all");
  hpx::local::init_params init_args;
  init_args.cfg = cfg;

  // Initialize and run HPX.
  HPX_TEST_EQ_MSG(hpx::local::init(hpx_main, argc, argv, init_args), 0,
                  "HPX main exited with non-zero status");

  return hpx::util::report_errors();
}
