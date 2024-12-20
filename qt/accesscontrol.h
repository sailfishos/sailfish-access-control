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

#ifndef SAILFISH_ACCESSCONTROL_H
#define SAILFISH_ACCESSCONTROL_H

#include <climits>
#include <QObject>

namespace Sailfish {

class AccessControl : public QObject
{
    Q_OBJECT

public:
    explicit AccessControl(QObject *parent = 0);
    ~AccessControl();

    enum Uid : int {
        RealUid = -1,
        EffectiveUid = -2,
        UndefinedUid = -INT_MAX
    };
    Q_ENUM(Uid)

    Q_INVOKABLE bool hasGroup(int uid, const QString groupName);
    Q_INVOKABLE int systemUserUid();
};

}

#endif // SAILFISH_ACCESSCONTROL_H
