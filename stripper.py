#from __future__ import with_statement
import os
import re

COUNT_CARRIAGE = True
SPACE_SENSITIVE = False

disallowed = [__file__[__file__.rfind('\\')+1:], r".*?.jpg", r".*?.png", r".*?.exe"]

for f in os.listdir('.'):
    if os.path.isfile(f):
        for bad in disallowed:
            if re.match(bad, f):
                break
        else:
            with open(f, "rb") as fi:
                data = fi.read()
            if COUNT_CARRIAGE:
                print "%s: %d carriage returns found."%(f, data.count("\r"))
            with open(f, "wb") as fo:
                if SPACE_SENSITIVE:
                    fo.write(data.replace("\r\n", "\n"))
                else:
                    for line in data.split('\n'):
                        if line:
                            fo.write(line.rstrip())
                            fo.write('\n')
raw_input("Press enter to continue...")
