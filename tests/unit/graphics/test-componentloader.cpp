/*!
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * \copyright   Copyright 2021 RSP Systems A/S. All rights reserved.
 * \license     Mozilla Public License 2.0
 * \author      Simon Glashoff
 */

#include "graphics/ComponentLoader.h"
#include <doctest.h>

using namespace ::rsp::graphics;

TEST_CASE("Component Loader Test")
{
    ComponentLoader loader("testFiles/Sprite sheet (GUI libary) V1.0.bmp",
                           "testFiles/GuiItemsPlacement.csv");
}