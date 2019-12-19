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

#include "accesscontrol.h"
#include "sailfishaccesscontrol.h"

using namespace Sailfish;

AccessControl::AccessControl(QObject *parent)
    : QObject(parent)
{
}

AccessControl::~AccessControl()
{
}

bool AccessControl::hasGroup(int uid, const QString groupName)
{
    switch (uid) {
        case RealUid:
            uid = getuid();
            break;
        case EffectiveUid:
            uid = geteuid();
            break;
        case UndefinedUid:
            return false;
            break;
        default:
            Q_ASSERT_X(uid >= 0, Q_FUNC_INFO, "Uid must be either of enum type Uid or non-negative.");
            break;
    }
    return sailfish_access_control_hasgroup(uid, groupName.toLatin1().data());
}

int AccessControl::systemUserUid()
{
    uid_t uid = sailfish_access_control_systemuser_uid();
    if (uid == SAILFISH_UNDEFINED_UID)
        return Uid::UndefinedUid;
    return uid;
}
