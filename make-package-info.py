import json, os, subprocess

print "************************************************"

def filesinfolder(n):
    return [f for f in os.listdir(n) if not f.startswith('.')]

with open('default-package-info.json', 'r') as f:
    pinfo = json.load(f)

    # update the package listings
    # note: this runs after the archive script copies the files into the appropriate folders
    pinfo['filelist']['externals'] = filesinfolder("CNMAT-Externals/externals")
    pinfo['filelist']['java-classes'] = filesinfolder("CNMAT-Externals/java-classes")
    pinfo['filelist']['help'] = filesinfolder("CNMAT-Externals/help")
    pinfo['filelist']['media'] = filesinfolder("CNMAT-Externals/media")
    pinfo['filelist']['misc'] = filesinfolder("CNMAT-Externals/misc")
    pinfo['filelist']['docs'] = filesinfolder("CNMAT-Externals/docs/refpages/cnmat_ref")
    pinfo['filelist']['extras'] = filesinfolder("CNMAT-Externals/extras/CNMAT-Externs")

    version = subprocess.check_output("git describe --tags --long", shell=True).strip()
    branch = subprocess.check_output("git branch | egrep '^\*' | awk '{print $2}'", shell=True).strip()
    pinfo['version'] = version+"-"+branch

    with open('package-info.json', 'w') as outf:
        json.dump(pinfo, outf, indent=4)
