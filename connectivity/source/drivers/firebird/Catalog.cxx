/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4; fill-column: 100 -*- */
/*
 * This file is part of the LibreOffice project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "Catalog.hxx"
#include "Tables.hxx"
#include "Users.hxx"
#include "Views.hxx"

#include <com/sun/star/sdbc/XRow.hpp>

using namespace ::connectivity::firebird;
using namespace ::com::sun::star;
using namespace ::com::sun::star::sdbc;
using namespace ::com::sun::star::uno;

Catalog::Catalog(const uno::Reference< XConnection >& rConnection):
    OCatalog(rConnection),
    m_xConnection(rConnection)
{
}

//----- OCatalog -------------------------------------------------------------
void Catalog::refreshTables()
{
    Sequence< OUString > aTypes {"TABLE", "VIEW"};

    uno::Reference< XResultSet > xTables = m_xMetaData->getTables(Any(),
                                                            "%",
                                                            "%",
                                                            aTypes);

    if (!xTables.is())
        return;

    ::std::vector< OUString> aTableNames;

    fillNames(xTables, aTableNames);

    if (!m_pTables)
        m_pTables.reset( new Tables(m_xConnection->getMetaData(),
                               *this,
                               m_aMutex,
                               aTableNames) );
    else
        m_pTables->reFill(aTableNames);

}

void Catalog::refreshViews()
{
    css::uno::Reference<css::sdbc::XResultSet> xViews
        = m_xMetaData->getTables(css::uno::Any(), "%", "%", { "VIEW" });

    if (!xViews.is())
        return;

    ::std::vector<OUString> aViewNames;

    fillNames(xViews, aViewNames);

    if (!m_pViews)
        m_pViews.reset(new Views(m_xConnection, *this, m_aMutex, aViewNames));
    else
        m_pViews->reFill(aViewNames);
}

//----- IRefreshableGroups ---------------------------------------------------
void Catalog::refreshGroups()
{
    // TODO: implement me
}

//----- IRefreshableUsers ----------------------------------------------------
void Catalog::refreshUsers()
{
    Reference<XStatement> xStmt= m_xMetaData->getConnection()->createStatement();
    uno::Reference< XResultSet > xUsers = xStmt->executeQuery("SELECT DISTINCT RDB$USER FROM RDB$USER_PRIVILEGES");

    if (!xUsers.is())
        return;

    ::std::vector< OUString> aUserNames;

    uno::Reference< XRow > xRow(xUsers,UNO_QUERY);
    while (xUsers->next())
    {
        aUserNames.push_back(xRow->getString(1));
    }

    if (!m_pUsers)
        m_pUsers.reset( new Users(m_xConnection->getMetaData(),
                             *this,
                             m_aMutex,
                             aUserNames) );
    else
        m_pUsers->reFill(aUserNames);
}

/* vim:set shiftwidth=4 softtabstop=4 expandtab cinoptions=b1,g0,N-s cinkeys+=0=break: */
