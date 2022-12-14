/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * This file is part of the LibreOffice project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * This file incorporates work covered by the following license notice:
 *
 *   Licensed to the Apache Software Foundation (ASF) under one or more
 *   contributor license agreements. See the NOTICE file distributed
 *   with this work for additional information regarding copyright
 *   ownership. The ASF licenses this file to you under the Apache
 *   License, Version 2.0 (the "License"); you may not use this file
 *   except in compliance with the License. You may obtain a copy of
 *   the License at http://www.apache.org/licenses/LICENSE-2.0 .
 */

#pragma once

#include <vcl/dllapi.h>
#include <tools/urlobj.hxx>
#include <tools/stream.hxx>
#include <unotools/ucbhelper.hxx>
#include <unotools/ucbstreamhelper.hxx>
#include <utility>

namespace vcl
{
class VCL_DLLPUBLIC SwapFile
{
private:
    INetURLObject maSwapURL;

public:
    SwapFile(INetURLObject aSwapURL)
        : maSwapURL(std::move(aSwapURL))
    {
    }

    ~SwapFile() COVERITY_NOEXCEPT_FALSE
    {
        utl::UCBContentHelper::Kill(maSwapURL.GetMainURL(INetURLObject::DecodeMechanism::NONE));
    }

    const INetURLObject& getSwapURL() const { return maSwapURL; }

    OUString getSwapURLString() const
    {
        return maSwapURL.GetMainURL(INetURLObject::DecodeMechanism::NONE);
    }

    std::unique_ptr<SvStream> openOutputStream()
    {
        OUString sSwapURL = getSwapURLString();
        if (!sSwapURL.isEmpty())
        {
            try
            {
                return utl::UcbStreamHelper::CreateStream(
                    sSwapURL, StreamMode::READWRITE | StreamMode::SHARE_DENYWRITE);
            }
            catch (const css::uno::Exception&)
            {
            }
        }
        return std::unique_ptr<SvStream>();
    }
};
}

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
