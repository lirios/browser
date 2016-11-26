/*
 * This file is part of Liri Browser
 *
 * Copyright (C) 2016 Tim Süberkrüb <tim.sueberkrueb@web.de>
 *
 * $BEGIN_LICENSE:GPL3+$
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * $END_LICENSE$
*/


#include "tab.h"

Tab::Tab(QObject *parent, bool valid)
    : QObject(parent)
{
    m_uid = -1;
    m_canReload = true;
    validChanged(m_valid = valid);
}

unsigned int Tab::uid() const
{
    return m_uid;
}

void Tab::setUid(int uid)
{
    uidChanged(m_uid = uid);
}

QUrl Tab::url() const
{
    return m_url;
}

void Tab::setUrl(QUrl url)
{
    urlChanged(m_url = url);
}

QString Tab::title() const
{
    return m_title;
}

void Tab::setTitle(QString title)
{
    titleChanged(m_title = title);
}

QUrl Tab::iconUrl() const
{
    return m_iconUrl;
}

void Tab::setIconUrl(QUrl iconUrl)
{
    iconUrlChanged(m_iconUrl = iconUrl);
}

bool Tab::canGoBack() const
{
    return m_canGoBack;
}

void Tab::setCanGoBack(bool canGoBack)
{
    canGoBackChanged(m_canGoBack = canGoBack);
}

bool Tab::canGoForward() const
{
    return m_canGoForward;
}

void Tab::setCanGoForward(bool canGoForward)
{
    canGoForwardChanged(m_canGoForward = canGoForward);
}

bool Tab::canReload() const
{
    return m_canReload;
}

void Tab::setCanReload(bool canReload)
{
    canReloadChanged(m_canReload = canReload);
}

bool Tab::loading() const
{
    return m_loading;
}

void Tab::setLoading(bool loading)
{
    loadingChanged(m_loading = loading);
}

unsigned int Tab::loadProgress() const
{
    return m_loadProgress;
}

void Tab::setLoadProgress(unsigned int loadProgress)
{
    loadProgressChanged(m_loadProgress = loadProgress);
}

bool Tab::valid() const
{
    return m_valid;
}

bool Tab::hasThemeColor() const
{
    return m_hasThemeColor;
}

void Tab::setHasThemeColor(bool hasThemeColor)
{
    hasThemeColorChanged(m_hasThemeColor = hasThemeColor);
}

QColor Tab::themeColor() const
{
    return m_themeColor;
}

void Tab::setThemeColor(QColor themeColor)
{
    themeColorChanged(m_themeColor = themeColor);
}

