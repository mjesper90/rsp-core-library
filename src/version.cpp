/*!
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * \copyright   Copyright 2021 RSP Systems A/S. All rights reserved.
 * \license     Mozilla Public License 2.0
 * \author      Steffen Brummer
 */

#include <sstream>
#include <version.h>

using namespace std;

string get_library_version() {
    stringstream ss;

    ss << LIB_VERSION_MAJOR << "." << LIB_VERSION_MINOR << ".0";

    return ss.str();
}
