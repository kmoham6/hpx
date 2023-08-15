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

  std::size_t start = 8192;
  std::size_t till = 1 << 29;

  const auto NUM_ITERATIONS = 10;

  std::vector<std::array<double, 4>> data;
  for (size_t s = start; s <= till; s *= 2) {
    // std::vector<std::string> arr = generateStrings (s, "string");
    // std::iota(std::begin(arr), std::end(arr), 1, [](int value) { 
    //   return std::to_string (value);
    std::vector<int> arr(s);
    std::iota(std::begin(arr), std::end(arr), 1);

    //  chunk_size *= 2;
    //  hpx::execution::experimental::static_chunk_size scs(s/8);
    //  hpx::execution::experimental::num_cores nc;
     hpx::execution::experimental::adaptive_core_chunk_size acc;

    double seqTime = 0;
    double parTime = 0;
    double speedUp = 0;

    for (int i = 0; i <= NUM_ITERATIONS + 5; i++) {
      std::vector<int> res(s);
      auto t1 = std::chrono::high_resolution_clock::now();
      hpx::adjacent_difference(hpx::execution::seq, arr.begin(), arr.end(),
                               res.begin());

      auto end1 = std::chrono::high_resolution_clock::now();

      if (i < 5) {
        continue;
      }
      std::chrono::duration<double> time_span1 =
          std::chrono::duration_cast<std::chrono::duration<double>>(end1 - t1);
      seqTime += time_span1.count();
    }
    for (int i = 0; i <= NUM_ITERATIONS + 5; i++) {
      std::vector<int> res1(s);       
      auto t2 = std::chrono::high_resolution_clock::now();
      // hpx::adjacent_difference(hpx::execution::par.with(nc, scs), arr.begin(),
      //                          arr.end(), res1.begin());
      hpx::adjacent_difference(hpx::execution::par.with(acc), arr.begin(),
                               arr.end(), res1.begin());

      auto end2 = std::chrono::high_resolution_clock::now();

      if (i < 5) {
        continue;
      }

      std::chrono::duration<double> time_span2 =
          std::chrono::duration_cast<std::chrono::duration<double>>(end2 - t2);

      parTime += time_span2.count();
    }

    seqTime /= NUM_ITERATIONS;
    parTime /= NUM_ITERATIONS;
    speedUp = seqTime / parTime;

    data.push_back(std::array<double, 4>{(double)s, seqTime, parTime, speedUp});
    std::cout << "n : " << s << '\n';
    std::cout << "seq: " << seqTime << '\n';
    std::cout << "par: " << parTime << '\n';
    std::cout << "spddd: " << speedUp << "\n\n";
  }

  for (auto &d : data) {
    std::cout << d[0] << "," << d[1] << "," << d[2] << "," << d[3] << ","
              << ",\n";
  }
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
