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

#include "pdfioutdev_gpl.hxx"
#ifdef _WIN32
# include <io.h>
# include <fcntl.h>  /*_O_BINARY*/
#endif
#ifndef SYSTEM_POPPLER
#include <string>         // std::string
#include <cstddef>        // std::size_t
#include <config_folders.h> //LIBO_SHARE_FOLDER
#endif

FILE* g_binary_out=stderr;

static const char *ownerPassword = "\001";
static const char *userPassword  = "\001";
static const char *outputFile    = "\001";
static const char *options       = "\001";

int main(int argc, char **argv)
{
    int k = 0;
    while (k < argc)
    {
        if (!strcmp(argv[k], "-f"))
        {
            outputFile = argv[k+1];
            argc -= 2;
            for (int j = k; j < argc; ++j)
                argv[j] = argv[j+2];
        }
        else if (!strcmp(argv[k], "-o"))
        {
            options = argv[k+1];
            argc -= 2;
            for (int j = k; j < argc; ++j)
                argv[j] = argv[j+2];
        }

        else if (!strcmp(argv[k], "-opw"))
        {
            ownerPassword = argv[k+1];
            argc -= 2;
            for (int j = k; j < argc; ++j)
                argv[j] = argv[j+2];
        }
        else if (!strcmp(argv[k], "-upw"))
        {
            userPassword = argv[k+1];
            argc -= 2;
            for (int j = k; j < argc; ++j)
                argv[j] = argv[j+2];
        }
        ++k;
    }

    /* Get data directory location */
#ifdef SYSTEM_POPPLER
    const char* datadir = nullptr;
#else
    /* Creates an absolute path to the poppler_data directory, by taking the path
     * to the xpdfimport executable (provided in argv[0], and concatenating a
     * relative path to the poppler_data directory from the program directory. */
    const std::string execPath = argv[0];
    const std::size_t filenameStartPos = execPath.find_last_of("/\\")+1;
    const std::string programPath = execPath.substr(0,filenameStartPos);
    const std::string popplerDataPath = programPath + "../" LIBO_SHARE_FOLDER "/xpdfimport/poppler_data";
    const char* datadir = popplerDataPath.c_str();
#endif

    // read config file
#if POPPLER_CHECK_VERSION(0, 83, 0)
    globalParams = std::make_unique<GlobalParams>(datadir);
#else
    globalParams = new GlobalParams(datadir);
#endif
    globalParams->setErrQuiet(true);
#if defined(_MSC_VER)
    globalParams->setupBaseFonts(nullptr);
#endif

    // try to read a possible open password from stdin
    char aPwBuf[129];
    aPwBuf[128] = 0;
    if( ! fgets( aPwBuf, sizeof(aPwBuf)-1, stdin ) )
        aPwBuf[0] = 0; // mark as empty
    else
    {
        for( size_t i = 0; i < sizeof(aPwBuf); i++ )
        {
            if( aPwBuf[i] == '\n' )
            {
                aPwBuf[i] = 0;
                break;
            }
        }
    }

    // PDFDoc takes over ownership for all strings below
    GooString* pFileName    = new GooString(argv[1]);
    GooString* pErrFileName = new GooString(argv[2]);

    // check for password string(s)
    GooString* pOwnerPasswordStr( aPwBuf[0] != 0
                                 ? new GooString( aPwBuf )
                                 : (ownerPassword[0] != '\001'
                                    ? new GooString(ownerPassword)
                                    : nullptr ) );
    GooString* pUserPasswordStr( aPwBuf[0] != 0
                                ? new GooString( aPwBuf )
                                : (userPassword[0] != '\001'
                                  ? new GooString(userPassword)
                                  : nullptr ) );
    if( outputFile[0] != '\001' )
        g_binary_out = fopen(outputFile,"wb");

#ifdef _WIN32
    // Win actually modifies output for O_TEXT file mode, so need to
    // revert to binary here
    _setmode( _fileno( g_binary_out ), _O_BINARY );
#endif

#if POPPLER_CHECK_VERSION(22, 6, 0)
    PDFDoc aDoc( std::make_unique<GooString>(pFileName),
                 std::optional<GooString>(pOwnerPasswordStr),
                 std::optional<GooString>(pUserPasswordStr) );

    PDFDoc aErrDoc( std::make_unique<GooString>(pErrFileName),
                 std::optional<GooString>(pOwnerPasswordStr),
                 std::optional<GooString>(pUserPasswordStr) );
#else
    PDFDoc aDoc( pFileName,
                 pOwnerPasswordStr,
                 pUserPasswordStr );

    PDFDoc aErrDoc( pErrFileName,
                 pOwnerPasswordStr,
                 pUserPasswordStr );
#endif

    // Check various permissions for aDoc.
    PDFDoc &rDoc = aDoc.isOk()? aDoc: aErrDoc;

    pdfi::PDFOutDev aOutDev(&rDoc);
    if (!strcmp(options, "SkipImages")) {
            aOutDev.setSkipImages(true);
    }

    // tell the receiver early - needed for proper progress calculation
    const int nPages = rDoc.isOk()? rDoc.getNumPages(): 0;
    pdfi::PDFOutDev::setPageNum(nPages);

    // virtual resolution of the PDF OutputDev in dpi
    static const int PDFI_OUTDEV_RESOLUTION = 7200;

    // do the conversion
    for (int i = 1; i <= nPages; ++i)
    {
        rDoc.displayPage(&aOutDev,
                i,
                PDFI_OUTDEV_RESOLUTION,
                PDFI_OUTDEV_RESOLUTION,
                0, true, true, true);
        rDoc.processLinks(&aOutDev, i);
    }

    return 0;
}

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
