#!/usr/bin/env python3

import sys
import os

cgroupRoot = ""

command = sys.stdin.readline().strip().split(maxsplit=1)
while True:
    if len(command) > 0:
        if command[0] == "setroot":
            cgroupRoot = command[1]

        elif command[0] == "mount":
            os.system("mount -t cgroup2 none {0}".format(cgroupRoot))

        elif command[0] == "mkdir":
            try:
                os.mkdir("{0}/btd6".format(cgroupRoot))
            except FileExistsError:
                pass

        elif command[0] == "addpid":
            os.system("echo {0} > {1}/btd6/cgroup.procs".format(command[1], cgroupRoot))

        elif command[0] == "umount":
            os.system("umount -t cgroup2 {0}".format(cgroupRoot))

    command = sys.stdin.readline().strip().split(maxsplit=1)
