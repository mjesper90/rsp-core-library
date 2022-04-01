/*!
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * \copyright   Copyright 2022 RSP Systems A/S. All rights reserved.
 * \license     Mozilla Public License 2.0
 * \author      Simon Glashoff
 */
#ifndef COMPONENTLOADER_H
#define COMPONENTLOADER_H

#include "graphics/primitives/Bitmap.h"
#include <map>
#include <sstream>

namespace rsp::graphics
{
class ComponentLoader
{
  public:
    ComponentLoader(std::string aBitmapSource, std::string aSectioningInfo);
    ~ComponentLoader();

    Bitmap &GetComponent(std::string aName);

  protected:
    std::string mLine = "";
    std::string mCell = "";

    std::unordered_map<std::string, Bitmap> mComponents{};
    std::map<std::string, unsigned int> mColumnHeaders{};
};

} // namespace rsp::graphics

#endif // COMPONENTLOADER_H