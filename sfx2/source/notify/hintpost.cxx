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

#include <hintpost.hxx>

#include <sfx2/request.hxx>
#include <utility>
#include <vcl/svapp.hxx>

SfxHintPoster::SfxHintPoster(std::function<void(std::unique_ptr<SfxRequest>)> aLink)
    : m_Link(std::move(aLink))
{
}

SfxHintPoster::~SfxHintPoster() {}

void SfxHintPoster::Post(std::unique_ptr<SfxRequest> pHintToPost)
{
    Application::PostUserEvent((LINK(this, SfxHintPoster, DoEvent_Impl)), pHintToPost.release());
    AddFirstRef();
}

IMPL_LINK(SfxHintPoster, DoEvent_Impl, void*, pPostedHint, void)
{
    auto pRequest = static_cast<SfxRequest*>(pPostedHint);
    if (m_Link)
        m_Link(std::unique_ptr<SfxRequest>(pRequest));
    else
        delete pRequest;
    ReleaseRef();
}

void SfxHintPoster::SetEventHdl(const std::function<void(std::unique_ptr<SfxRequest>)>& rLink)
{
    m_Link = rLink;
}

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
