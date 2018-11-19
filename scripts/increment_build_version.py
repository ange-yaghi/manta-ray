# This is a short script which increments the build version

import re
import datetime

import os
from os import listdir
from os.path import isfile, join

from datetime import datetime, date, time

def GetFileLineCount(filename):
    f = open(filename)
    lineCount = 0
    
    for line in f:
        lineCount += 1
        
    return lineCount

if __name__ == "__main__":

    buildVersion = 0
    lineCount = 0
    buildLog = []

    mypath = os.path.dirname(os.path.realpath(__file__)) + "/../"

    directories = ['src', 'test', 'include', 'scripts', 'opencl_programs']
    
    f = open(mypath + "/tracking/BuildVersion.txt", "r")
    
    if (f):
    
        headerLine1 = f.readline()
        headerLine2 = f.readline()
        
        fileVersionLine = f.readline()
        match = re.search("BUILD VERSION:\s*(\d+)", fileVersionLine)

        if match is not None:
            buildVersion = int(match.group(1))
            
        # Save the build log
        for line in f:
            # Check if the line is empty
            if line.strip():
                buildLog.append(line)
        
    # Increment the build version
    buildVersion += 1
    
    # Calculate line count

    #sourceFileList = [f for f in listdir(mypath) if isfile(join(mypath, f))]
    
    ignoreFiles = ['sqlite3ext.h', 'sqlite3.h', 'shell.c', 'sqlite3.c'];

    for directory in directories:
        for root, subFolders, files in os.walk(mypath + directory):
            for fileEntry in files:
                if (fileEntry.endswith(".h") or fileEntry.endswith(".cpp") or fileEntry.endswith(".py") or fileEntry.endswith('.cl')):
                    if (fileEntry not in ignoreFiles):
                        lineCount += GetFileLineCount(os.path.join(root, fileEntry.strip()))
        
    # Rewrite the file
       
    f = open(mypath + "/tracking/BuildVersion.txt", "w")
    
    f.write("MantaRay 2018 Build Information\n")
    f.write("Ange Yaghi | 2018 | Every now and then, some rain must fall\n")
    
    f.write("BUILD VERSION: %d\n\n" % (buildVersion))
    
    dt = datetime.now()   
    dateString = dt.strftime("%Y-%m-%d %H:%M")
    f.write("Build\t%s\t%d\t%d\n" % (dateString, lineCount, buildVersion))
    
    for logEntry in buildLog:
        f.write(logEntry);
    
    f.close()
