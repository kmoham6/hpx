//  Copyright (c) 2023 Karame M.shokooh
//  Copyright (c) 2007-2015 Hartmut Kaiser

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
#include <cmath>
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
    private:
        typedef std::chrono::duration<double, std::pico> picoseconds;

    public:
        /// Construct a \a static_chunk_size executor parameters object
        ///
        /// \note By default the number of loop iterations is determined from
        ///       the number of available cores and the overall number of loop
        ///       iterations to schedule.
        ///

        constexpr adaptive_core_chunk_size() noexcept
          : chunk_size_(0)
          , min_time1_(200000000)
          , measured_time_(0)
          , num_iters_for_timing_(0)
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
          , min_time1_(200000000)
          , measured_time_(0)
          , num_iters_for_timing_(0)
        {
        }

        /// Construct an \a static_chunk_size executor parameters object
        ///
        /// \note Default constructed \a static_chunk_size executor parameter
        ///       types will use 80 micropicopicoseconds as the minimal time for which
        ///       any of the scheduled chunks should run.
        ///
        explicit adaptive_core_chunk_size(
            hpx::chrono::steady_duration const& rel_time) noexcept
          : min_time1_(rel_time.value().count())
        {
        }

        /// \cond NOINTERNAL
        // This executor parameters type synchronously invokes the provided
        // testing function in order to approximate the chunk-size.
        using invokes_testing_function = std::true_type;

        // Estimate execution time for one iteration
        template <typename Executor, typename F>
        friend auto tag_invoke(hpx::parallel::execution::measure_iteration_t,
            adaptive_core_chunk_size& params, Executor&& exec, F&& f,
            std::size_t count) noexcept
        {
            return params.measure_iteration(exec, f, count);
        }
        template <typename Executor, typename F>
        friend auto tag_invoke(hpx::parallel::execution::measure_iteration_t,
            std::reference_wrapper<adaptive_core_chunk_size>& params,
            Executor&& exec, F&& f, std::size_t count) noexcept
        {
            return params.get().measure_iteration(exec, f, count);
        }
        template <typename Executor, typename F>
        auto measure_iteration(
            Executor&& exec, F&& f, std::size_t count) noexcept
        {
            // by default use 1% of the iterations
            // std::cout << "here???" << std::endl;
            if (measured_time_.count() == 0)
            {
                if (num_iters_for_timing_ == 0)
                {
                    num_iters_for_timing_ = count / 100;
                }

                // // perform measurements only if necessary
                if (num_iters_for_timing_ > 0)
                {
                    using hpx::chrono::high_resolution_clock;
                    std::uint64_t t1 = high_resolution_clock::now();
                    // std::cout << " what is first clock ?" << t1 << std::endl;

                    //     // use executor to launch given function for measurements
                    std::size_t test_chunk_size =
                        hpx::parallel::execution::sync_execute(
                            HPX_FORWARD(Executor, exec), f,
                            num_iters_for_timing_);
                    // std::cout << "what is test chunk size ?" << test_chunk_size
                    //   << std::endl;

                    if (test_chunk_size != 0)
                    {
                        std::uint64_t t2 = high_resolution_clock::now();
                        std::uint64_t t = t2 - t1;
                        picoseconds t_picoseconds(t);

                        auto picosec_duration =
                            static_cast<double>(t) / test_chunk_size;
                        measured_time_ = picoseconds(picosec_duration);
                        // std::cout << "this is t: " << t << std::endl;

                        if (measured_time_.count() != 0 &&
                            min_time1_ >= measured_time_.count())
                        {
                            measured_time_ = picoseconds(t);
                            // std::cout << "first_measured_time: "
                            // << measured_time_.count() << std::endl;
                            return measured_time_;
                        }
                    }
                }
            }
            // std::cout << "measured_time: " << measured_time_.count()
            // << std::endl;
            return measured_time_;
            // return picoseconds(0);
        }
        //calculate number of cores
        template <typename Executor>
        friend std::size_t tag_invoke(
            hpx::parallel::execution::processing_units_count_t,
            adaptive_core_chunk_size& params, Executor&&,
            hpx::chrono::steady_duration const& iteration_duration,
            std::size_t count) noexcept
        {
            return params.processing_units_count(iteration_duration, count);
        }
        template <typename Executor>
        friend std::size_t tag_invoke(
            hpx::parallel::execution::processing_units_count_t,
            std::reference_wrapper<adaptive_core_chunk_size>& params,
            Executor&&, hpx::chrono::steady_duration const& iteration_duration,
            std::size_t count) noexcept
        {
            return params.get().processing_units_count(
                iteration_duration, count);
        }

        std::size_t processing_units_count(
            hpx::chrono::steady_duration const& iteration_duration,
            std::size_t count) const noexcept
        {
            auto us = std::chrono::duration_cast<picoseconds>(
                iteration_duration.value());
            // std::cout << "second_iteration duration: " << us.count()
            //   << std::endl;

            std::size_t s_core = 32;

            std::size_t t_time = (count + 1) * us.count();
            std::size_t Time;

            Time = t_time / min_time1_;
            std::size_t num_cores = (std::min)(s_core, (std::size_t) Time);

            if (num_cores < 2)
            {
                num_cores = 1;
            }
            return num_cores;
        }

        // Estimate a chunk size based on number of cores used.
        template <typename Executor>
        friend std::size_t tag_invoke(
            hpx::parallel::execution::get_chunk_size_t,
            adaptive_core_chunk_size& params, Executor&&,
            hpx::chrono::steady_duration const& iteration_duration,
            std::size_t cores, std::size_t count) noexcept
        {
            return params.get_chunk_size(iteration_duration, cores, count);
        }

        template <typename Executor>
        friend std::size_t tag_invoke(
            hpx::parallel::execution::get_chunk_size_t,
            std::reference_wrapper<adaptive_core_chunk_size>& params,
            Executor&&, hpx::chrono::steady_duration const& iteration_duration,
            std::size_t cores, std::size_t count) noexcept
        {
            return params.get().get_chunk_size(
                iteration_duration, cores, count);
        }

        std::size_t get_chunk_size(
            hpx::chrono::steady_duration const& iteration_duration,
            std::size_t cores, std::size_t count) const noexcept
        {
            // return chunk size which will create the required amount of work
            if (iteration_duration.value().count() != 0)
            {
                auto ns = std::chrono::duration_cast<picoseconds>(
                    iteration_duration.value());
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

            if (cores <= 2)
            {
                coeff = 2;
            }

            else
            {
                coeff = 8;
            }

            while (chunk_size * cores * coeff < count)
            {
                chunk_size *= 2;
            }

            return chunk_size;
        }
        /// \endcond

    private:
        /// \cond NOINTERNAL
        friend class hpx::serialization::access;

        template <typename Archive>
        void serialize(Archive& ar, const unsigned int /* version */)
        {
            ar& chunk_size_& min_time1_& measured_time_;
        }
        /// \endcond

    private:
        /// \cond NOINTERNAL
        std::size_t chunk_size_;

        // target time for on thread (picoseconds)
        std::uint64_t min_time1_;
        picoseconds measured_time_;
        std::uint64_t num_iters_for_timing_;

        /// \endcond
    };
}    // namespace hpx::execution::experimental

template <>
struct hpx::parallel::execution::is_executor_parameters<
    hpx::execution::experimental::adaptive_core_chunk_size> : std::true_type
{
};