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

#include "sailfishaccesscontrol.h"

#include <grp.h>
#include <pwd.h>
#include <stdlib.h>
#include <glib.h>

#define NO_GROUPS GINT_TO_POINTER(1)

GHashTable *s_groups = NULL;

static GSList *init_group_list(uid_t uid)
{
    GSList *group_list = NULL;
    struct passwd *passwd_entry = getpwuid(uid);

    if (passwd_entry) {
        int ngroups = 32;
        const char *user_name = passwd_entry->pw_name;
        gid_t group_id = passwd_entry->pw_gid;
        gid_t *groups = g_malloc0(ngroups * sizeof(gid_t));

        if (getgrouplist(user_name, group_id, groups, &ngroups) == -1) {
            g_free(groups);
            // When failing ngroups will contain number of groups.
            groups = g_malloc0(ngroups * sizeof(gid_t));
            if (getgrouplist(user_name, group_id, groups, &ngroups) == -1)
                ngroups = 0;
        }

        for (int i = 0; i < ngroups; i++) {
            struct group *gr = getgrgid(groups[i]);
            if (gr)
                group_list = g_slist_prepend(group_list, g_strdup(gr->gr_name));
        }

        g_free(groups);
    }
    g_hash_table_insert(s_groups, GINT_TO_POINTER(uid), (group_list ? group_list : NO_GROUPS));
    return group_list;
}

bool sailfish_access_control_hasgroup(uid_t uid, const char *group_name)
{
    GSList *groups = NULL;
    if (!s_groups)
        s_groups = g_hash_table_new(NULL, NULL);

    groups = g_hash_table_lookup(s_groups, GINT_TO_POINTER(uid));
    if (!groups)
        groups = init_group_list(uid);

    if (groups == NO_GROUPS)
        return false;

    GSList *item = g_slist_find_custom(groups, group_name, (GCompareFunc)g_strcmp0);
    if (item)
        return true;

    return false;
}
