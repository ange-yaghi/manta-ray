# This is a short script which keeps track of how many local builds were made
# and the overall line count of the project

import re
import datetime

import os
from os import listdir
from os.path import isfile, join

from datetime import datetime, date, time

def get_file_line_count(filename):
    f = open(filename)
    line_count = 0
    
    for line in f:
        line_count += 1
    
    return line_count

def scan_directory(root_path, directory, ignore_files, extensions):
    line_count = 0
    for root, sub_dirs, files in os.walk(root_path + directory):
        for file_entry in files:
            correct_extension = False
            for ext in extensions:
                if file_entry.endswith(ext):
                    correct_extension = True
                    break

            if correct_extension:
                if file_entry not in ignore_files:
                    line_count += get_file_line_count(os.path.join(root, file_entry.strip()))

    return line_count


if __name__ == "__main__":
    buildVersion = 0
    line_count = 0
    buildLog = []

    mypath = os.path.dirname(os.path.realpath(__file__)) + "/../"

    extensions = ['.h', '.cpp', '.py', '.cl', '.y', '.l', '.mr']
    directories = ['src', 'test', 'include', 'scripts', 'opencl_programs', 'demos', 'utilities', 'flex-bison', 'sdl']
    
    # Find the right file (previous versions used different naming conventions)
    possible_file_names = ['build_version.txt', 'BuildVersion.txt']
    actual_file_name = possible_file_names[0]
    for fname in possible_file_names:
        try:
            with open(mypath + '/workspace/tracking/' + fname, 'r') as f:
                actual_file_name = fname
                break
        except FileNotFoundError:
            pass

    try:
        with open(mypath + '/workspace/tracking/' + actual_file_name, 'r') as f:    
            headerLine1 = f.readline()
            headerLine2 = f.readline()
        
            fileVersionLine = f.readline()
            match = re.search('BUILD VERSION:\s*(\d+)', fileVersionLine)

            if match is not None:
                buildVersion = int(match.group(1))
            
            # Save the build log
            for line in f:
                # Check if the line is empty
                if line.strip():
                    buildLog.append(line)

    except FileNotFoundError:
        # File does not currently exist which is okay
        try:
            os.makedirs(mypath + '/workspace/tracking/')
        except FileExistsError:
            pass
        
    # Increment the build version
    buildVersion += 1
    
    # Calculate line count    
    ignoreFiles = ['sqlite3ext.h', 'sqlite3.h', 'shell.c', 'sqlite3.c']

    for directory in directories:
        line_count += scan_directory(mypath, directory, ignoreFiles, extensions)
        
    # Rewrite the file
    f = open(mypath + '/workspace/tracking/build_version.txt', 'w')
    
    f.write('MantaRay 2019 Build Information\n')
    f.write('Ange Yaghi | 2019\n')
    
    f.write('BUILD VERSION: %d\n\n' % (buildVersion))
    
    dt = datetime.now()   
    dateString = dt.strftime('%Y-%m-%d %H:%M')
    f.write('Build\t%s\t%d\t%d\n' % (dateString, line_count, buildVersion))
    
    for logEntry in buildLog:
        f.write(logEntry)
    
    f.close()
