/**
 * \copyright        Copyright 2022 RSP Systems A/S. All rights reserved.
 * \license          Mozilla Public License 2.0
 * \author:          Jesper Madsen
 * Created:           Tuesday, May 17th 2022, 8:49:44 am
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 * HISTORY:
 * Date      	By	Comments
 * ----------	---	----------------------------------------------------------
 */

#ifndef HTTPRESPONSE_H
#define HTTPRESPONSE_H

#include <network/IHttpRequest.h>
#include <network/IHttpResponse.h>

namespace rsp::network::http
{
     /**
     * \class HttpResponse
     *
     * \brief This is the output when making a HttpRequest and implements the IHttpResponse interface.
     *
     */
    class HttpResponse : public IHttpResponse
    {
    public:
        HttpResponse(IHttpRequest &arRequest)
            : mrRequest(arRequest)
        {
        }

        int GetStatusCode() const override
        {
            return mStatusCode;
        }
        const IHttpResponse &SetStatusCode(int const &arCode)
        {
            mStatusCode = arCode;
            return *this;
        }
        const IHttpResponse &SetHeaders(std::map<std::string, std::string> &arHeaders) override
        {
            mHeaders = arHeaders;
            return *this;
        }
        const IHttpResponse &SetBody(std::string const &arBody)
        {
            mBody = arBody;
            return *this;
        }
        const std::map<std::string, std::string> &AddHeader(std::string aKey, std::string aValue) override
        {
            mHeaders[aKey] = aValue;
            return mHeaders;
        }
        const std::map<std::string, std::string> &GetHeaders() const override { return mHeaders; }
        const IHttpRequest &GetRequest() const override { return mrRequest; }
        const std::string &GetBody() const override { return mBody; }

    protected:
        IHttpRequest &mrRequest;
        int mStatusCode = 0;
        std::map<std::string, std::string> mHeaders{};
        std::string mBody{};
    }; // Class HttpResponse
} // namespace rsp::network::http
#endif // HTTPRESPONSE_H
