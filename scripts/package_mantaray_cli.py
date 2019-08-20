#!python3

"""
This script packages the MantaRay CLI into a redistributable build.
"""

import sys
import os
import os.path
import shutil
from distutils.dir_util import copy_tree
from standard_logging import *
import copy_dlls

SCRIPT_PATH = os.path.dirname(os.path.realpath(__file__))
ROOT_DIR = SCRIPT_PATH + "/../"
TARGET_DIRECTORY = ROOT_DIR + "/project/"

def log(level, data):
    print("{0}: {1}".format(level, data))

def generate_target_path():
    return ROOT_DIR + "/workspace/build/mantaray_cli/"

def generate_compiler_output_dir(architecture, configuration):
    return TARGET_DIRECTORY + "/{}/{}/".format(architecture, configuration)

def generate_sdl_lib_dir():
    return ROOT_DIR + "/sdl/manta-lib"

def clean():
    log("INFO", "Deleting last build...")

    try:
        shutil.rmtree(generate_target_path())
    except FileNotFoundError:
        log("INFO", "No previous build found, skipping clean")
    except OSError:
        log("ERROR", "Could not clean build, files are likely in use")
        sys.exit(1) # Return with an error

def create_dir():
    try:
        os.makedirs(generate_target_path())
    except FileExistsError:
        log("WARNING", "Build folder was not cleaned")
        pass

def copy_exe(architecture, configuration):
    target_directory = generate_target_path()
    compiler_dir = generate_compiler_output_dir(architecture, configuration)

    shutil.copy(compiler_dir + "mantaray_cli.exe", target_directory)

def copy_runtime_dlls(architecture, configuration):
    source_directory = copy_dlls.generate_source_directory(architecture, configuration)
    target_directory = generate_target_path()

    copy_dlls.copy_dlls(source_directory, target_directory)

def copy_vc_runtime(vc_crt_path):
    source_directory = vc_crt_path
    target_directory = generate_target_path()

    copy_dlls.copy_dlls(source_directory, target_directory)

def copy_sdl_std_lib():
    source_directory = generate_sdl_lib_dir()
    target_directory = generate_target_path()

    conf_file = open(target_directory + "mantaray_cli.conf", "w")
    conf_file.write("manta-lib/")
    conf_file.close()

    try:
        shutil.copytree(source_directory, target_directory + "/manta-lib/")
    except FileNotFoundError:
        log("INFO", "Could not find SDL standard library")
    except OSError:
        log("ERROR", "Could not copy SDL standard library, files likely in use")
        sys.exit(1) # Return with an error

if __name__ == "__main__":
    architecture = sys.argv[1]
    configuration = sys.argv[2]
    vc_crt_path = sys.argv[3]

    print_full_header('Packaging MantaRay CLI')
    clean()
    create_dir()
    copy_exe(architecture, configuration)
    copy_runtime_dlls(architecture, configuration)
    copy_vc_runtime(vc_crt_path)
    copy_sdl_std_lib()
    print_footer()
