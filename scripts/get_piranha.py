#!python3

"""
This is a short script which fetches the Piranha binaries in case
the developer is working on both MantaRay and Piranha at the same time
"""

import sys
import os
import os.path
import shutil
import distutils
import distutils.dir_util
from standard_logging import *

SCRIPT_PATH = os.path.dirname(os.path.realpath(__file__))
ROOT_DIR = SCRIPT_PATH + "/../"
DEPENDENCY_PATH = ROOT_DIR + "/dependencies/libraries/piranha"
PIRANHA_PATH = ROOT_DIR + "/dependencies/submodules/piranha/"
PIRANHA_BUILD_ROOT = PIRANHA_PATH + "/build/"
PIRANHA_BUILD = PIRANHA_BUILD_ROOT + "/piranha/"


def log(level, data):
    print("{0}: {1}".format(level, data))


def make_directory(name):
    os.makedirs(name)


def copy_piranha():
    log("INFO", "Copying local Piranha build")
    try:
        distutils.dir_util.copy_tree(PIRANHA_BUILD, DEPENDENCY_PATH)
        log("INFO", "Found local Piranha build; merging it with dependencies")

        # Get build info from the info file
        info_elements = []
        with open(PIRANHA_BUILD_ROOT + "build_info.txt") as f:
            for row in f:
                s = row.split('=')
                param_name = s[0].strip()
                param_value = s[1].strip()

                info_elements.append("{}={}".format(param_name, param_value))
        
        log("INFO", ', '.join(info_elements))

    except (FileNotFoundError, distutils.errors.DistutilsFileError):
        log("INFO", "Local Piranha build not found; using what's already present in dependencies")


if __name__ == "__main__":
    print_full_header('Get Local Piranha Build')
    copy_piranha()
    print_footer()
