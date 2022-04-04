/*!
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * \copyright   Copyright 2022 RSP Systems A/S. All rights reserved.
 * \license     Mozilla Public License 2.0
 * \author      Steffen Brummer
 */

#include <chrono>
#include <doctest.h>
#include <thread>
#include <utils/InRange.h>
#include <utils/StopWatch.h>

using namespace rsp::utils;

TEST_CASE("Stop Watch")
{

    StopWatch sw;
    std::this_thread::sleep_for(std::chrono::milliseconds(20));

    CHECK(IsInRange(sw.Elapsed<std::chrono::milliseconds>(), static_cast<int64_t>(20l), static_cast<int64_t>(25l)));

    sw.Reset();

    CHECK(IsInRange(sw.Elapsed<std::chrono::milliseconds>(), static_cast<int64_t>(0l), static_cast<int64_t>(5l)));

    std::this_thread::sleep_for(std::chrono::milliseconds(20));

    CHECK(IsInRange(sw.Elapsed<std::chrono::milliseconds>(), static_cast<int64_t>(20l), static_cast<int64_t>(25l)));
}
