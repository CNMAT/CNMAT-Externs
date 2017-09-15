import json, os, subprocess

print "************************************************"

with open('default-package-info.json', 'r') as f:
    pinfo = json.load(f)

    # update the package listings
    pinfo['filelist']['externals'] = os.listdir("build/Release")
    pinfo['filelist']['help'] = os.listdir("help")
    pinfo['filelist']['media'] = os.listdir("media")
    pinfo['filelist']['misc'] = os.listdir("misc")

    version = subprocess.check_output("git describe --tags --long", shell=True).strip()
    branch = subprocess.check_output("git branch | egrep '^\*' | awk '{print $2}'", shell=True).strip()
    pinfo['version'] = version+"-"+branch

    with open('package-info.json', 'w') as outf:
        json.dump(pinfo, outf, indent=4)
