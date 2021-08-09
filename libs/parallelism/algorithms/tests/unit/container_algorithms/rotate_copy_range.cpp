//  Copyright (c) 2007-2015 Hartmut Kaiser
//  Copyright (c) 2021 Chuanqiu He
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpx/iterator_support/iterator_range.hpp>
#include <hpx/iterator_support/tests/iter_sent.hpp>
#include <hpx/local/init.hpp>
#include <hpx/modules/testing.hpp>
#include <hpx/parallel/container_algorithms/rotate.hpp>

#include <cstddef>
#include <iostream>
#include <iterator>
#include <numeric>
#include <string>
#include <vector>

#include "test_utils.hpp"

///////////////////////////////////////////////////////////////////////////////
template <typename IteratorTag>
void test_rotate_copy_sent(IteratorTag)
{
    std::vector<std::size_t> c(10007);
    std::vector<std::size_t> d1(c.size());
    std::vector<std::size_t> d2(c.size());    //-V656

    std::iota(std::begin(c.base()), std::end(c.base()), std::rand());

    std::size_t mid_pos = std::rand() % c.size();

    auto mid = std::begin(c);
    std::advance(mid, mid_pos);

    hpx::ranges::rotate_copy(std::begin(c), mid,
        sentinel<std::size_t>{*(std::end(c) - 1)}), std::begin(d1));

    auto mid_base = std::begin(c.base());
    std::advance(mid_base, mid_pos);

    std::rotate_copy(std::begin(c), mid_base, std::end(c) - 1, std::begin(d2));

    std::size_t count = 0;
    HPX_TEST(std::equal(std::begin(d1), std::end(d1) - 1, std::begin(d2),
        [&count](std::size_t v1, std::size_t v2) -> bool {
            HPX_TEST_EQ(v1, v2);
            ++count;
            return v1 == v2;
        }));
    HPX_TEST_EQ(count, d1.size() - 1);
}

template <typename ExPolicy, typename IteratorTag>
void test_rotate_copy_sent(ExPolicy policy, IteratorTag)
{
    std::vector<std::size_t> c(10007);
    std::vector<std::size_t> d1(c.size());
    std::vector<std::size_t> d2(c.size());    //-V656

    std::iota(std::begin(c.base()), std::end(c.base()), std::rand());

    std::size_t mid_pos = std::rand() % c.size();

    auto mid = std::begin(c);
    std::advance(mid, mid_pos);

    hpx::ranges::rotate_copy(policy, std::begin(c), mid,
        sentinel<std::size_t>{*(std::end(c) - 1)}), std::begin(d1));

    auto mid_base = std::begin(c.base());
    std::advance(mid_base, mid_pos);

    std::rotate_copy(std::begin(c), mid_base, std::end(c) - 1, std::begin(d2));

    std::size_t count = 0;
    HPX_TEST(std::equal(std::begin(d1), std::end(d1) - 1, std::begin(d2),
        [&count](std::size_t v1, std::size_t v2) -> bool {
            HPX_TEST_EQ(v1, v2);
            ++count;
            return v1 == v2;
        }));
    HPX_TEST_EQ(count, d1.size() - 1);
}

//add test w/o ExPolicy
template <typename IteratorTag>
void test_rotate_copy(IteratorTag)
{
    typedef std::vector<std::size_t>::iterator base_iterator;
    typedef test::test_iterator<base_iterator, IteratorTag> iterator;

    typedef test::test_container<std::vector<std::size_t>, IteratorTag>
        test_vector;

    test_vector c(10007);
    std::vector<std::size_t> d1(c.size());
    std::vector<std::size_t> d2(c.size());    //-V656

    std::iota(std::begin(c.base()), std::end(c.base()), std::rand());

    std::size_t mid_pos = std::rand() % c.size();

    auto mid = std::begin(c);
    std::advance(mid, mid_pos);

    hpx::ranges::rotate_copy(c, iterator(mid), std::begin(d1));

    auto mid_base = std::begin(c.base());
    std::advance(mid_base, mid_pos);

    std::rotate_copy(
        std::begin(c.base()), mid_base, std::end(c.base()), std::begin(d2));

    std::size_t count = 0;
    HPX_TEST(std::equal(std::begin(d1), std::end(d1), std::begin(d2),
        [&count](std::size_t v1, std::size_t v2) -> bool {
            HPX_TEST_EQ(v1, v2);
            ++count;
            return v1 == v2;
        }));
    HPX_TEST_EQ(count, d1.size());
}

template <typename ExPolicy, typename IteratorTag>
void test_rotate_copy(ExPolicy policy, IteratorTag)
{
    static_assert(hpx::is_execution_policy<ExPolicy>::value,
        "hpx::is_execution_policy<ExPolicy>::value");

    std::vector<std::size_t> c(10007);
    std::vector<std::size_t> d1(c.size());
    std::vector<std::size_t> d2(c.size());    //-V656

    std::iota(std::begin(c.base()), std::end(c.base()), std::rand());

    std::size_t mid_pos = std::rand() % c.size();

    auto mid = std::begin(c);
    std::advance(mid, mid_pos);

    hpx::ranges::rotate_copy(policy, c, mid, std::begin(d1));

    auto mid_base = std::begin(c);
    std::advance(mid_base, mid_pos);

    std::rotate_copy(std::begin(c), mid_base, std::end(c), std::begin(d2));

    std::size_t count = 0;
    HPX_TEST(std::equal(std::begin(d1), std::end(d1), std::begin(d2),
        [&count](std::size_t v1, std::size_t v2) -> bool {
            HPX_TEST_EQ(v1, v2);
            ++count;
            return v1 == v2;
        }));
    HPX_TEST_EQ(count, d1.size());
}

template <typename ExPolicy, typename IteratorTag>
void test_rotate_copy_async(ExPolicy p, IteratorTag)
{
    std::vector<std::size_t> c(10007);
    std::vector<std::size_t> d1(c.size());
    std::vector<std::size_t> d2(c.size());    //-V656

    std::iota(std::begin(c), std::end(c), std::rand());

    std::size_t mid_pos = std::rand() % c.size();

    auto mid = std::begin(c);
    std::advance(mid, mid_pos);

    auto f = hpx::ranges::rotate_copy(p, c, mid, std::begin(d1));
    f.wait();

    auto mid_base = std::begin(c.base());
    std::advance(mid_base, mid_pos);

    std::rotate_copy(std::begin(c), mid_base, std::end(c), std::begin(d2));

    std::size_t count = 0;
    HPX_TEST(std::equal(std::begin(d1), std::end(d1), std::begin(d2),
        [&count](std::size_t v1, std::size_t v2) -> bool {
            HPX_TEST_EQ(v1, v2);
            ++count;
            return v1 == v2;
        }));
    HPX_TEST_EQ(count, d1.size());
}

template <typename IteratorTag>
void test_rotate_copy()
{
    using namespace hpx::execution;
    test_rotate_copy_sent(IteratorTag());
    test_rotate_copy_sent(seq, IteratorTag());
    test_rotate_copy_sent(par, IteratorTag());
    test_rotate_copy_sent(par_unseq, IteratorTag());

    test_rotate_copy(seq, IteratorTag());
    test_rotate_copy(par, IteratorTag());
    test_rotate_copy(par_unseq, IteratorTag());

    test_rotate_copy_async(seq(task), IteratorTag());
    test_rotate_copy_async(par(task), IteratorTag());
}

void rotate_copy_test()
{
    test_rotate_copy<std::random_access_iterator_tag>();
    test_rotate_copy<std::forward_iterator_tag>();
}

int hpx_main(hpx::program_options::variables_map& vm)
{
    unsigned int seed = (unsigned int) std::time(nullptr);
    if (vm.count("seed"))
        seed = vm["seed"].as<unsigned int>();

    std::cout << "using seed: " << seed << std::endl;
    std::srand(seed);

    rotate_copy_test();

    return hpx::local::finalize();
}

int main(int argc, char* argv[])
{
    // add command line option which controls the random number generator seed
    using namespace hpx::program_options;
    options_description desc_commandline(
        "Usage: " HPX_APPLICATION_STRING " [options]");

    desc_commandline.add_options()("seed,s", value<unsigned int>(),
        "the random number generator seed to use for this run");

    // By default this test should run on all available cores
    std::vector<std::string> const cfg = {"hpx.os_threads=all"};

    // Initialize and run HPX
    hpx::local::init_params init_args;
    init_args.desc_cmdline = desc_commandline;
    init_args.cfg = cfg;

    HPX_TEST_EQ_MSG(hpx::local::init(hpx_main, argc, argv, init_args), 0,
        "HPX main exited with non-zero status");

    return hpx::util::report_errors();
}
