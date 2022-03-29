/*!
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * \copyright   Copyright 2022 RSP Systems A/S. All rights reserved.
 * \license     Mozilla Public License 2.0
 * \author      Simon Glashoff
 */

#include "graphics/ComponentLoader.h"

namespace rsp::graphics
{
ComponentLoader::ComponentLoader(std::string aPathToSource)
{
    // Read bitmap source
    Bitmap source(aPathToSource);
    // Section bitmap into all its components
    // Add all the sectioned bitmap components to member map
}

ComponentLoader::~ComponentLoader()
{
}

Bitmap &ComponentLoader::GetComponent(std::string aName)
{
    // Simple for now
    return mComponents[aName];
}

} // namespace rsp::graphics
