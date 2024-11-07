#pragma once
#include <functional>


  template<typename Index, typename FunctionType >
  inline bool parallel_for(
    const Index loop_size,
    const FunctionType & func,
    const size_t min_parallel=0);
 
  template<
    typename Index,
    typename PrepFunctionType,
    typename FunctionType,
    typename AccumFunctionType
    >
  inline bool parallel_for(
    const Index loop_size,
    const PrepFunctionType & prep_func,
    const FunctionType & func,
    const AccumFunctionType & accum_func,
    const size_t min_parallel=0);


// Implementation

#include "default_num_threads.h"

#include <cmath>
#include <cassert>
#include <thread>
#include <vector>
#include <algorithm>

template<typename Index, typename FunctionType >
inline bool parallel_for(
  const Index loop_size,
  const FunctionType & func,
  const size_t min_parallel)
{
  using namespace std;
  // no op preparation/accumulation
  const auto & no_op = [](const size_t /*n/t*/){};
  // two-parameter wrapper ignoring thread id
  const auto & wrapper = [&func](Index i,size_t /*t*/){ func(i); };
  return parallel_for(loop_size,no_op,wrapper,no_op,min_parallel);
}

template<
  typename Index,
  typename PreFunctionType,
  typename FunctionType,
  typename AccumFunctionType>
inline bool parallel_for(
    const Index loop_size,
    const PreFunctionType& prep_func,
    const FunctionType& func,
    const AccumFunctionType& accum_func,
    const size_t min_parallel)
{
    assert(loop_size >= 0);
    if (loop_size == 0) return false;
    // Estimate number of threads in the pool
    // http://ideone.com/Z7zldb
#ifdef IGL_PARALLEL_FOR_FORCE_SERIAL
    const size_t nthreads = 1;
#else
    const size_t nthreads = default_num_threads();
#endif
    if (loop_size < min_parallel || nthreads <= 1)
    {
        // serial
        prep_func(1);
        for (Index i = 0; i < loop_size; i++) func(i, 0);
        accum_func(0);
        return false;
    }
    else
    {
        // Size of a slice for the range functions
        Index slice =
            std::max(
                (Index)std::round((loop_size + 1) / static_cast<double>(nthreads)), (Index)1);

        // [Helper] Inner loop
        const auto& range = [&func](const Index k1, const Index k2, const size_t t)
            {
                for (Index k = k1; k < k2; k++) func(k, t);
            };
        prep_func(nthreads);
        // Create pool and launch jobs
        std::vector<std::thread> pool;
        pool.reserve(nthreads);
        // Inner range extents
        Index i1 = 0;
        Index i2 = std::min(0 + slice, loop_size);
        {
            size_t t = 0;
            for (; t + 1 < nthreads && i1 < loop_size; ++t)
            {
                pool.emplace_back(range, i1, i2, t);
                i1 = i2;
                i2 = std::min(i2 + slice, loop_size);
            }
            if (i1 < loop_size)
            {
                pool.emplace_back(range, i1, loop_size, t);
            }
        }
        // Wait for jobs to finish
        for (std::thread& t : pool) if (t.joinable()) t.join();
        // Accumulate across threads
        for (size_t t = 0; t < nthreads; t++)
        {
            accum_func(t);
        }
        return true;
    }
}
