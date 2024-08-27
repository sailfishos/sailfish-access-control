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
#include <sys/stat.h>

#define GROUPS_DATABASE "/etc/group"
#define NO_GROUPS GINT_TO_POINTER(1)
#define SAILFISH_SYSTEM_GROUP "sailfish-system"
#define SAILFISH_SYSTEM_PREFIX "sailfish-"

GHashTable *s_groups = NULL;

static void ensure_groups_validity(void)
{
    struct stat buf;
    time_t new_mtime = 0;
    static time_t old_mtime = 0;

    if (stat(GROUPS_DATABASE, &buf) == 0)
        new_mtime = buf.st_mtime;

    if (old_mtime != new_mtime) {
        old_mtime = new_mtime;
        if (s_groups)
            g_hash_table_remove_all(s_groups);
    }
}

static void destroy_group_list(GSList *group_list)
{
    if (group_list == NO_GROUPS)
        return;
    g_slist_free_full(group_list, g_free);
}

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

    ensure_groups_validity();

    if (!s_groups)
        s_groups = g_hash_table_new_full(NULL, NULL, NULL, (GDestroyNotify)destroy_group_list);

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

uid_t sailfish_access_control_systemuser_uid(void)
{
    struct group *grp = getgrnam(SAILFISH_SYSTEM_GROUP);
    struct passwd *pw;
    int i;
    uid_t uid = SAILFISH_UNDEFINED_UID;

    // Get system user UID using sailfish-system group
    if (grp) {
        for (i = 0; grp->gr_mem[i]; i++) {
            if (strncmp(grp->gr_mem[i], SAILFISH_SYSTEM_PREFIX, strlen(SAILFISH_SYSTEM_PREFIX))) {
                pw = getpwnam(grp->gr_mem[i]);
                if (pw) {
                    uid = pw->pw_uid;
                    break;
                }
            }
        }
    }

    return uid;
}
