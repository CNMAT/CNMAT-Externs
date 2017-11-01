import json, os, subprocess

print "************************************************"

def filesinfolder(n):
    return [f for f in os.listdir(n) if not f.startswith('.')]

with open('default-package-info.json', 'r') as f:
    pinfo = json.load(f)

    # update the package listings
    pinfo['filelist']['externals'] = filesinfolder("build/Release")
    pinfo['filelist']['help'] = filesinfolder("help")
    pinfo['filelist']['media'] = filesinfolder("media")
    pinfo['filelist']['misc'] = filesinfolder("misc")

    version = subprocess.check_output("git describe --tags --long", shell=True).strip()
    branch = subprocess.check_output("git branch | egrep '^\*' | awk '{print $2}'", shell=True).strip()
    pinfo['version'] = version+"-"+branch

    with open('package-info.json', 'w') as outf:
        json.dump(pinfo, outf, indent=4)
