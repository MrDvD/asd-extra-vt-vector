#include <benchmark/benchmark.h>

#include "../vt/vt.hpp"

static void PushBack_NoReserve(benchmark::State& state) {
  for (auto _ : state) {
    vt::Vector<int> array = {};
    for (std::size_t i = 0; i < state.range(0); i++) {
      array.PushBack(5);
    }
  }
  // size of the input
  state.SetComplexityN(state.range(0));
}
BENCHMARK(PushBack_NoReserve)->RangeMultiplier(8)->Range(8, 8 << 10)->Complexity();

BENCHMARK_MAIN();