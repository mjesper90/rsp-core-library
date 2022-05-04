/*!
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * \copyright   Copyright 2022 RSP Systems A/S. All rights reserved.
 * \license     Mozilla Public License 2.0
 * \author      Steffen Brummer
 */

#include <security/Encrypt.h>

namespace rsp::security {

Encrypt::Encrypt(std::string_view aIvSeed, std::string_view aSecret)
    : CryptBase(CipherTypes::AES_128_CBC)
{
    Init(aIvSeed, aSecret);
}

void Encrypt::Init(std::string_view aIvSeed, std::string_view aSecret)
{
    if (!pImpl) {
        pImpl = MakePimpl(mCipherType);
    }
    pImpl->Init(aIvSeed, aSecret);
}

} /* namespace rsp::security */
