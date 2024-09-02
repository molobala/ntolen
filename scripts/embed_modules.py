#!/usr/bin/env python
# coding: utf-8

import argparse
import glob
import os.path
import re


PREAMBLE = """// Generated automatically from {0}. Do not edit.
R"------(
{2}
)------";
"""

def mosc_to_c_string(file, mosc_source_lines, module):
    mosc_source = ""
    # cut off blank lines at the bottom
    while (mosc_source_lines[-1].strip()==""):
        mosc_source_lines.pop()
    for line in mosc_source_lines:
        mosc_source += line + '\n'

    mosc_source = mosc_source.strip()
    return PREAMBLE.format(file, module, mosc_source)

def process_file(path):
    infile = os.path.basename(path)
    outfile = path + ".inc"
    print("{} => {}".format(path.replace("src/",""), outfile))

    with open(path, "r") as f:
        mosc_source_lines = f.readlines() + ["\n\n"]
        with open(outfile, "w") as out:
            print("Writing to {0}".format(outfile))
            out.write(mosc_to_c_string(path, mosc_source_lines, ""))




def main():
    files = glob.glob("src/core/*.msc") + glob.glob("src/core/**/*.msc")
    for file in files:
        process_file(file)


main()