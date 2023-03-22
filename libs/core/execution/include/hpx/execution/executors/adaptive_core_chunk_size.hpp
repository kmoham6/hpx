//  Copyright (c) 2007-2015 Hartmut Kaiser
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

/// \file parallel/executors/adaptive_core_chunk_size.hpp

#pragma once

#include <hpx/config.hpp>
#include <hpx/execution/executors/execution_parameters.hpp>
#include <hpx/execution_base/execution.hpp>
#include <hpx/execution_base/traits/is_executor_parameters.hpp>
#include <hpx/modules/timing.hpp>
#include <hpx/serialization/serialize.hpp>

#include <hpx/execution/executors/execution_parameters_fwd.hpp>
#include <hpx/execution_base/execution.hpp>
#include <hpx/timing/high_resolution_clock.hpp>
#include <hpx/timing/steady_clock.hpp>

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <utility>

namespace hpx::execution::experimental {
    ///////////////////////////////////////////////////////////////////////////
    /// Loop iterations are divided into pieces of size \a chunk_size and then
    /// assigned to threads. If \a chunk_size is not specified, the iterations
    /// are evenly (if possible) divided contiguously among the threads.
    ///
    /// \note This executor parameters type is equivalent to OpenMP's STATIC
    ///       scheduling directive.
    ///
    struct adaptive_core_chunk_size
    {
    public:
        /// Construct a \a static_chunk_size executor parameters object
        ///
        /// \note By default the number of loop iterations is determined from
        ///       the number of available cores and the overall number of loop
        ///       iterations to schedule.
        ///

        constexpr adaptive_core_chunk_size() noexcept
          : chunk_size_(0)
          , min_time_(32768)
          , t_(1)
        //   , t1_(400000)
        //   , t2_(600000)
        //   , t3_(1368150)
        //   , t4_(154036)
        //   , num_iters_for_timing_(0)
        {
        }

        /// Construct a \a static_chunk_size executor parameters object
        ///
        /// \param chunk_size   [in] The optional chunk size to use as the
        ///                     number of loop iterations to run on a single
        ///                     thread.
        ///
        constexpr explicit adaptive_core_chunk_size(std::size_t chunk_size)
          : chunk_size_(chunk_size)
          , min_time_(32768)
          , t_(1)
        //   , t1_(400000)
        //   , t2_(600000)
        //   , t3_(1368150)
        //   , t4_(154036)
        //   , num_iters_for_timing_(0)
        {
        }

        /// Construct an \a static_chunk_size executor parameters object
        ///
        /// \note Default constructed \a static_chunk_size executor parameter
        ///       types will use 80 microseconds as the minimal time for which
        ///       any of the scheduled chunks should run.
        ///

        // constexpr explicit static_chunk_size(
        //     std::uint64_t num_iters_for_timing = 0)
        //   : t1_(228)
        //   , t2_(660)
        //   , num_iters_for_timing_(num_iters_for_timing)
        // {
        // }

        explicit adaptive_core_chunk_size(
          hpx::chrono::steady_duration const& rel_time,
            // hpx::chrono::steady_duration const& rel_time2,
            // hpx::chrono::steady_duration const& rel_time3,
            std::uint64_t t = 10) noexcept
          : min_time_(rel_time.value().count())
        //   , t1_(rel_time1.value().count())
        //   , t2_(rel_time2.value().count())
        //   , num_iters_for_timing_(num_iters_for_timing)
             , t_(t)
        {
        }

        /// \cond NOINTERNAL
        // This executor parameters type synchronously invokes the provided
        // testing function in order to approximate the chunk-size.
        using invokes_testing_function = std::true_type;

        // Estimate execution time for one iteration
        template <typename Executor, typename F>
        auto measure_iteration(
            Executor&& exec, F&& f, std::size_t count) noexcept
        {
            std:: uint64_t t = 1;
            // by default use 1% of the iterations
            // if (num_iters_for_timing_ == 0)
            // {
            //     num_iters_for_timing_ = count / 100;
            // }

            // // perform measurements only if necessary
            // if (num_iters_for_timing_ > 0)
            // {
            //     using hpx::chrono::high_resolution_clock;
            //     std::uint64_t t = high_resolution_clock::now();

            //     // use executor to launch given function for measurements
            //     std::size_t test_chunk_size =
            //         hpx::parallel::execution::sync_execute(
            //             HPX_FORWARD(Executor, exec), f, num_iters_for_timing_);

            //     if (test_chunk_size != 0)
            //     {
            //         t = (high_resolution_clock::now() - t) / test_chunk_size;
            //         if (t == 0 )
            //         {
            //             return std::chrono::nanoseconds(2);
            //         }
            //         else if (min_time_ >= t)
            //         {
            //             return std::chrono::nanoseconds(t);
            //         }
            //         // else if (t1_ >= t && t2_ >= t && t3_ >= t)
            //         // {
            //         //     // return execution time for one iteration
            //         //     return std::chrono::nanoseconds(t);
            //         // }
            //     }
            // }

            return std::chrono::nanoseconds(t);
        }
        //calculate number of cores
        template <typename Executor>
        constexpr std::size_t processing_units_count(Executor&& exec,
            hpx::chrono::steady_duration const& iteration_duration, std::size_t count) const noexcept
        {
        //     std::size_t s_core =
        // hpx::parallel::execution::processing_units_count(HPX_FORWARD(Executor, exec), iteration_duration, count);
        // std::size_t s_core =
        // hpx::parallel::execution::processing_units_count(exec, iteration_duration, count);

            // std::size_t chunk_size = 1;
            // double coeff = 1;
            // std::uint64_t num_cores = 2;
            // std::uint64_t t1 = 400000;
            // std::uint64_t t2 = 600000;
            // std::uint64_t t3 = 1368150;
            // std::uint64_t t4 = 154036;
            std::uint64_t min_time = 32768;
            auto us = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    iteration_duration.value());
            // std::uint64_t min_cores = us.count() / min_time_;
            std::size_t s_core = 40;
            std::cout << "\n\n" << "this part is the start of adaptive_core_chunk_size : " << "********************************************" << "\n" << std::endl;
            std::cout << "this is count in proccessing_unit: " << count << std::endl;
            std::cout << "this is time per iteration:" << us.count() << std::endl;
            std::cout << "this is number of cores:" << (count * us.count()) / min_time <<std::endl;
            std::size_t t_time = count * us.count ();
            std::size_t num_cores = (std::min) (s_core, (std::size_t)(t_time / min_time_));
            // std::uint64_t num_iteration2 = t2 / us.count();
            // std::uint64_t num_iteration3 = t3 / us.count();
            // std::uint64_t num_iteration4 = t4 / us.count();

            // if (count <= num_iteration1)
            // {
            //     num_cores = 2;
            // }
            // if (count <= num_iteration2)
            // {
            //     num_cores = 8;
            // }
            // else if (count <= num_iteration3)
            // {
            //     num_cores = 16;
            // }
            // else if (count > num_iteration3)
            // {
            //     num_cores = 32;
            // }
            // else 
            // {
            //     num_cores = 2;
            // }
            // num_cores = 2;
            if (num_cores <= 2 )
            {

                num_cores = 2;
            }
             std::cout << "this is the final number of cores:" << num_cores <<std::endl;

            return num_cores;
        }

        // Estimate a chunk size based on number of cores used.
        template <typename Executor>
        std::size_t get_chunk_size(Executor&&,
            hpx::chrono::steady_duration const& iteration_duration,
            std::size_t cores, std::size_t count) noexcept
        {
            std::cout <<"\n" << std::endl;
            std::cout <<"this is number of cores in get_chunk_chunk: " << cores << std::endl;
            // return chunk size which will create the required amount of work
            if (iteration_duration.value().count() != 0)
            {
                auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    iteration_duration.value());
                // return (std::min)(count, (std::size_t)(min_time_ / ns.count()));
            }


            // use the given chunk size if given
            if (chunk_size_ != 0)
            {
                return chunk_size_;
            }

            // Return a chunk size that is a power of two; and that leads to at
            // least 2 chunks per core, and at most 4 chunks per core.
            std::size_t chunk_size = 1;
            double coeff = 1;
            // std::uint64_t t1 = 128000;
            // std::uint64_t t2 = 820000;
            // std::uint64_t num_iteration1 = t1 / iteration_duration.value();
            // std::uint64_t num_iteration2 = t2 / iteration_duration.value();
            // std::uint64_t num_iters_for_timing = 0;

            // if (cores == 1)
            // {
            //     return count;
            // }

            if (cores < 32)
            {
                coeff = 1;
            }
            // if (cores ==8 )
            // {
            //     coeff = 4;
            // }
            // else if (cores == 16)
            // {
            //     coeff = 1;
            // }
            // if (cores == 32)
            // {
            //     coeff = 8;
            // }
            else 
            {
                coeff = 4;
            }
            // std::cout << "\n" << "chunk_size*cores*coeff : " << chunk_size * cores * coeff << std::endl;
            while (chunk_size * cores * coeff < count)
            {
                chunk_size *= 2;
            }
            std::cout << "this is chunk_size: " << chunk_size << std::endl;
            std::cout << "this is cores:: " << cores << std::endl;
            std::cout << "this is count: " << count << std::endl;
            std::cout <<  "\n" << "this is the end of adaptive_core_chunk_size" << "*****************************************" << "\n\n" << std::endl;
            // double num_chunk = count / chunk_size;
            // std::cout << "this is number of chunk: " <<num_chunk << "\n" << std::endl;
            return chunk_size;
        }
        /// \endcond

    private:
        /// \cond NOINTERNAL
        friend class hpx::serialization::access;

        template <typename Archive>
        void serialize(Archive& ar, const unsigned int /* version */)
        {
            ar& chunk_size_& min_time_& t_;
        }
        /// \endcond

    private:
        /// \cond NOINTERNAL
        std::size_t chunk_size_;

        // target time for on thread (nanoseconds)
        std::uint64_t min_time_;

        // target time for on thread (nanoseconds)
        // std::uint64_t t1_;

        // // target time for on thread (nanoseconds)
        // std::uint64_t t2_;

        // // target time for on thread (nanoseconds)
        // std::uint64_t t3_;

        // target time for on thread (nanoseconds)
        // std::uint64_t t4_;

        // number of iteration to use for timing
        // std::uint64_t num_iters_for_timing_;
           std::uint64_t t_;
        /// \endcond
    };
}    // namespace hpx::execution

// namespace hpx { namespace parallel { namespace execution {
//     using static_chunk_size HPX_DEPRECATED_V(1, 6,
//         "hpx::parallel::execution::static_chunk_size is deprecated. Use "
//         "hpx::execution::static_chunk_size instead.") =
//         hpx::execution::static_chunk_size;
// }}}    // namespace hpx::parallel::execution

namespace hpx {
    namespace parallel {
        namespace execution {
            /// \cond NOINTERNAL
            template <>
            struct is_executor_parameters<
                hpx::execution::experimental::adaptive_core_chunk_size>
              : std::true_type
            {
            };
            /// \endcond
        }    // namespace hpx::parallel::executions

    }}

        namespace hpx::execution {

            using adaptive_core_chunk_size HPX_DEPRECATED_V(1, 9,
                "hpx::execution::auto_chunk_size is deprecated, use "
                "hpx::execution::experimental::auto_chunk_size instead") =
                hpx::execution::experimental::adaptive_core_chunk_size;
        }