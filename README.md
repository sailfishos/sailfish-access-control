Sailfish Access Control
=======================

This package provides a thin wrapper library on top of the getuid, getpwuid, getgrouplist, and friends.
Checking whether a user belongs to a group should be done via this Sailfish
Access Control library.

This will make it easier to fix for instance rerentrancy issues.

