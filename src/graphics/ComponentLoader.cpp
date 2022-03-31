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

    // std::cout << "Number of columns detected" << mColumnHeaders.size() << std::endl;
    // for (const auto column : mColumnHeaders) {
    //     std::cout << column << std::endl;
    // }
    // std::getchar();

    std::vector<std::string> lineCells;
    while (std::getline(sectionFile, mLine)) {
        std::cout << mLine << std::endl; // Debug
        std::stringstream lineStream(mLine);
        while (std::getline(lineStream, mCell, ',')) {
            lineCells.push_back(mCell);
        }
        // Debug
        /*for (const auto aCell : lineCells) {
            std::cout << aCell << std::endl;
        }*/
        // End Debug
        mComponents.insert(std::pair<std::string, Bitmap>(lineCells[mColumnHeaders["UnitName"]],
                                                          source.GetSection(Point(std::stoi(lineCells[mColumnHeaders["SpriteX"]]),
                                                                                  std::stoi(lineCells[mColumnHeaders["SpriteY"]])),
                                                                            std::stoi(lineCells[mColumnHeaders["SpriteHeight"]]),
                                                                            std::stoi(lineCells[mColumnHeaders["SpriteWidth"]]))));
    }
    std::getchar(); // Debug

    // Section bitmap into all its components
    // Add all the sectioned bitmap components to member map
}

ComponentLoader::~ComponentLoader()
{
}

/*Bitmap &ComponentLoader::GetComponent(std::string aName)
{
    // Simple for now
    return mComponents[aName];
}*/

} // namespace rsp::graphics
