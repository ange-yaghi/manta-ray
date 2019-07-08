"""
Simple python script to run a batch file (or other executable)

The need for this file is not immediately obvious. I'm sure you're scratching
your head right now trying to figure out why I even added it in the first place.
The short answer is that Visual Studio has a very peculiar feature of turning
off ALL logging from pre/post build events after a batch file is run. I'm guessing
they received so many complaints from people trying to run malformed batch
files (that leave echoing to the terminal on) and having their build output cluttered. 
This script effectively bypasses Visual Studio's own workaround and allows output
to the console after a batch file is run.
"""

import subprocess
import sys
import os
import os.path

SCRIPT_PATH = os.path.dirname(os.path.realpath(__file__))

def run_batch_file(filename):
    full_path = "{}\{}".format(SCRIPT_PATH, filename)

    print("INFO: Running file '{}'".format(filename))

    try:
        subprocess.call(full_path)
    except FileNotFoundError:
        print("ERROR: Could not find batch file '{}'".format(full_path))

if __name__ == "__main__":
    run_batch_file(sys.argv[1])
