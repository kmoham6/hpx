//  Copyright (c)      2020 Mikael Simberg
//  Copyright (c) 2007-2022 Hartmut Kaiser
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

/// \file parallel/executors/restricted_thread_pool_executors.hpp

#pragma once

#include <hpx/config.hpp>
#include <hpx/assert.hpp>
#include <hpx/execution/execution.hpp>
#include <hpx/execution/executors/execution_parameters.hpp>
#include <hpx/executors/parallel_executor.hpp>

#include <atomic>
#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <utility>
#include <vector>

namespace hpx::parallel::execution {

    template <typename Policy>
    class restricted_policy_executor
    {
    private:
        static constexpr std::size_t hierarchical_threshold_default_ = 6;

        using embedded_executor =
            hpx::execution::parallel_policy_executor<Policy>;

    public:
        /// Associate the parallel_execution_tag executor tag type as a default
        /// with this executor.
        using execution_category =
            typename embedded_executor::execution_category;

        /// Associate the static_chunk_size executor parameters type as a default
        /// with this executor.
        using executor_parameters_type =
            typename embedded_executor::executor_parameters_type;

        /// Create a new parallel executor
        explicit restricted_policy_executor(std::size_t first_thread = 0,
            std::size_t num_threads = 1,
            threads::thread_priority priority =
                threads::thread_priority::default_,
            threads::thread_stacksize stacksize =
                threads::thread_stacksize::default_,
            threads::thread_schedule_hint schedulehint = {},
            std::size_t hierarchical_threshold =
                hierarchical_threshold_default_)
          : first_thread_(static_cast<std::uint16_t>(first_thread))
          , os_thread_(0)
          , exec_(priority, stacksize, schedulehint,
                parallel::execution::detail::get_default_policy<Policy>::call(),
                hierarchical_threshold)
        {
            // set initial number of cores
            exec_ = hpx::parallel::execution::with_processing_units_count(
                exec_, num_threads);
        }

        restricted_policy_executor(restricted_policy_executor const& other)
          : first_thread_(other.first_thread_)
          , os_thread_(other.first_thread_)
          , exec_(other.exec_)
        {
        }

        /// \cond NOINTERNAL
        bool operator==(restricted_policy_executor const& rhs) const noexcept
        {
            return exec_ == rhs.exec_ && first_thread_ == rhs.first_thread_;
        }

        bool operator!=(restricted_policy_executor const& rhs) const noexcept
        {
            return !(*this == rhs);
        }

        restricted_policy_executor const& context() const noexcept
        {
            return *this;
        }

    private:
        // this function is conceptually const (os_threads_ is mutable)
        std::int16_t get_next_thread_num() const
        {
            return static_cast<std::int16_t>(first_thread_ +
                (os_thread_++ %
                    hpx::parallel::execution::processing_units_count(exec_)));
        }

        embedded_executor generate_executor(std::uint16_t thread_num) const
        {
            return hpx::execution::experimental::with_hint(
                exec_, threads::thread_schedule_hint(thread_num));
        }

    private:
        // property implementations

        // support all properties exposed by the embedded executor
        template <typename Tag, typename Property,
            typename Enable = std::enable_if_t<hpx::functional::
                    is_tag_invocable_v<Tag, embedded_executor, Property>>>
        friend restricted_policy_executor tag_invoke(
            Tag, restricted_policy_executor const& exec, Property&& prop)
        {
            auto exec_with_prop = exec;
            exec_with_prop.exec_ =
                Tag{}(exec.exec_, HPX_FORWARD(Property, prop));
            return exec_with_prop;
        }

        template <typename Tag,
            typename Enable = std::enable_if_t<
                hpx::functional::is_tag_invocable_v<Tag, embedded_executor>>>
        friend decltype(auto) tag_invoke(
            Tag, restricted_policy_executor const& exec)
        {
            return Tag{}(exec.exec_);
        }

        // executor API
        template <typename F, typename... Ts>
        friend decltype(auto) tag_invoke(
            hpx::parallel::execution::sync_execute_t,
            restricted_policy_executor const& exec, F&& f, Ts&&... ts)
        {
            return hpx::parallel::execution::sync_execute(
                exec.generate_executor(exec.get_next_thread_num()),
                HPX_FORWARD(F, f), HPX_FORWARD(Ts, ts)...);
        }

        template <typename F, typename... Ts>
        friend decltype(auto) tag_invoke(
            hpx::parallel::execution::async_execute_t,
            restricted_policy_executor const& exec, F&& f, Ts&&... ts)
        {
            return hpx::parallel::execution::async_execute(
                exec.generate_executor(exec.get_next_thread_num()),
                HPX_FORWARD(F, f), HPX_FORWARD(Ts, ts)...);
        }

        template <typename F, typename Future, typename... Ts>
        friend decltype(auto) tag_invoke(
            hpx::parallel::execution::then_execute_t,
            restricted_policy_executor const& exec, F&& f, Future&& predecessor,
            Ts&&... ts)
        {
            return hpx::parallel::execution::then_execute(
                exec.generate_executor(exec.get_next_thread_num()),
                HPX_FORWARD(F, f), HPX_FORWARD(Future, predecessor),
                HPX_FORWARD(Ts, ts)...);
        }

        // NonBlockingOneWayExecutor (adapted) interface
        template <typename F, typename... Ts>
        friend decltype(auto) tag_invoke(hpx::parallel::execution::post_t,
            restricted_policy_executor const& exec, F&& f, Ts&&... ts)
        {
            return hpx::parallel::execution::post(
                exec.generate_executor(exec.get_next_thread_num()),
                HPX_FORWARD(F, f), HPX_FORWARD(Ts, ts)...);
        }

        // clang-format off
        template <typename F, typename S, typename... Ts,
            HPX_CONCEPT_REQUIRES_(
                !std::is_integral_v<S>
            )>
        // clang-format on
        friend decltype(auto) tag_invoke(
            hpx::parallel::execution::bulk_async_execute_t,
            restricted_policy_executor const& exec, F&& f, S const& shape,
            Ts&&... ts)
        {
            return hpx::parallel::execution::bulk_async_execute(
                exec.generate_executor(exec.first_thread_), HPX_FORWARD(F, f),
                shape, HPX_FORWARD(Ts, ts)...);
        }

        // clang-format off
        template <typename F, typename S, typename Future, typename... Ts,
            HPX_CONCEPT_REQUIRES_(
                !std::is_integral_v<S>
            )>
        // clang-format on
        friend decltype(auto) tag_invoke(
            hpx::parallel::execution::bulk_then_execute_t,
            restricted_policy_executor const& exec, F&& f, S const& shape,
            Future&& predecessor, Ts&&... ts)
        {
            return hpx::parallel::execution::bulk_then_execute(
                exec.generate_executor(exec.first_thread_), HPX_FORWARD(F, f),
                shape, HPX_FORWARD(Future, predecessor),
                HPX_FORWARD(Ts, ts)...);
        }
        /// \endcond

    private:
        std::uint16_t const first_thread_;
        mutable std::atomic<std::size_t> os_thread_;

        embedded_executor exec_;
    };

    using restricted_thread_pool_executor =
        restricted_policy_executor<hpx::launch>;

    ///////////////////////////////////////////////////////////////////////////
    /// \cond NOINTERNAL
    template <typename Policy>
    struct is_one_way_executor<restricted_policy_executor<Policy>>
      : is_one_way_executor<hpx::execution::parallel_policy_executor<Policy>>
    {
    };

    template <typename Policy>
    struct is_never_blocking_one_way_executor<
        restricted_policy_executor<Policy>>
      : is_never_blocking_one_way_executor<
            hpx::execution::parallel_policy_executor<Policy>>
    {
    };

    template <typename Policy>
    struct is_bulk_one_way_executor<restricted_policy_executor<Policy>>
      : is_bulk_one_way_executor<
            hpx::execution::parallel_policy_executor<Policy>>
    {
    };

    template <typename Policy>
    struct is_two_way_executor<restricted_policy_executor<Policy>>
      : is_two_way_executor<hpx::execution::parallel_policy_executor<Policy>>
    {
    };

    template <typename Policy>
    struct is_bulk_two_way_executor<restricted_policy_executor<Policy>>
      : is_bulk_two_way_executor<
            hpx::execution::parallel_policy_executor<Policy>>
    {
    };

    template <typename Policy>
    struct is_scheduler_executor<restricted_policy_executor<Policy>>
      : is_scheduler_executor<hpx::execution::parallel_policy_executor<Policy>>
    {
    };
    /// \endcond
}    // namespace hpx::parallel::execution
