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
 *
 */

#ifndef SAILFISH_ACCESS_CONTROL_H_
#define SAILFISH_ACCESS_CONTROL_H_

#include <stdbool.h>
#include <unistd.h>

#define SAILFISH_UNDEFINED_UID UINT_MAX

#ifdef __cplusplus
extern "C" {
#endif
#pragma GCC visibility push(default)

bool sailfish_access_control_hasgroup(uid_t uid, const char *group_name);
uid_t sailfish_access_control_systemuser_uid();

#pragma GCC visibility pop

#ifdef __cplusplus
};
#endif

#endif
