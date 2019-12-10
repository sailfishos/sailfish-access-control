/*
 * Copyright (c) 2019 Open Mobile Platform LLC.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see <http://www.gnu.org/licenses/>.
 */

#include <QQmlExtensionPlugin>
#include <QQmlEngine>
#include <qqml.h>

#include "accesscontrol.h"

using namespace Sailfish;

template <typename T> static QObject *singletonApiCallback(QQmlEngine *engine, QJSEngine *)
{
    return new T(engine);
}

class SailfishAccessControlPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "Sailfish.AccessControl" )

public:
    virtual void registerTypes(const char *uri)
    {
        if (QLatin1String(uri) == QLatin1String("Sailfish.AccessControl")) {
            qmlRegisterSingletonType<AccessControl>(uri, 1, 0, "AccessControl", singletonApiCallback<AccessControl>);
        }
    }
};

#include "plugin.moc"
