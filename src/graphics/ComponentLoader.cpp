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
#include <fstream>
#include <iostream>
#include <string>

namespace rsp::graphics
{
ComponentLoader::ComponentLoader(std::string aBitmapSource, std::string aSectioningInfo)
{
    // Read bitmap source
    Bitmap source(aBitmapSource);

    // Read csv file for sections
    std::cout << aSectioningInfo << std::endl;
    std::ifstream sectionFile;
    sectionFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    sectionFile.open(aSectioningInfo, std::ifstream::in);

    // Get header
    std::getline(sectionFile, mLine);
    std::stringstream headerStream(mLine);
    unsigned int columnCounter = 0;
    while (std::getline(headerStream, mCell, ',')) {
        mColumnHeaders.insert(std::pair<std::string, unsigned int>(mCell, columnCounter));
        columnCounter++;
    }

    std::vector<std::string> lineCells;
    while (sectionFile.peek() != EOF) {
        lineCells.clear();
        std::getline(sectionFile, mLine);
        std::cout << mLine << std::endl; // Debug
        std::stringstream lineStream(mLine);
        while (std::getline(lineStream, mCell, ',')) {
            lineCells.push_back(mCell);
        }
        /*
        std::cout << "Inserting Component: " << std::endl;
        std::cout << "UnitName: " << lineCells[mColumnHeaders["UnitName"]] << std::endl;
        std::cout << "SpriteX: " << lineCells[mColumnHeaders["SpriteX"]] << std::endl;
        std::cout << "SpriteY: " << lineCells[mColumnHeaders["SpriteY"]] << std::endl;
        std::cout << "SpriteHeight: " << lineCells[mColumnHeaders["SpriteHeight"]] << std::endl;
        std::cout << "SpriteWidth: " << lineCells[mColumnHeaders["SpriteWidth"]] << std::endl;
        */
        Bitmap section(source.GetSection(Point(std::stoi(lineCells[mColumnHeaders["SpriteX"]]),
                                               std::stoi(lineCells[mColumnHeaders["SpriteY"]])),
                                         std::stoi(lineCells[mColumnHeaders["SpriteHeight"]]),
                                         std::stoi(lineCells[mColumnHeaders["SpriteWidth"]])));

        Component comp(Point(std::stoi(lineCells[mColumnHeaders["GuiX"]]),
                             std::stoi(lineCells[mColumnHeaders["GuiY"]])),
                       section);

        mComponents.insert(std::pair<std::string, Component>(lineCells[mColumnHeaders["UnitName"]], comp));
    }
    std::cout << "Components: " << mComponents.size() << std::endl;
}

ComponentLoader::~ComponentLoader()
{
}

Component &ComponentLoader::GetComponent(std::string aName)
{
    try {
        return mComponents.at(aName);
    } catch (const std::out_of_range &e) {
        throw std::out_of_range(std::string("Component not found") + ": " + e.what());
    }
}

} // namespace rsp::graphics
