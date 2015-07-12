archive_name=CNMAT_Externals-Max-Win32-`git describe --tags --long`-`git branch | egrep '^\*' | awk '{print $2}'`.zip
dirs=(help media misc)

mkdir CNMAT-Externals && mkdir CNMAT-Externals/externals
cp build/Release/*.mxe CNMAT-Externals/externals

for f in ${dirs[*]}
do
	[ -e $f ] && cp -r $f CNMAT-Externals
done

# force-copy an old version of resonators~
cp -f kludge/resonators~.mxe CNMAT-Externals/externals

zip -r $archive_name CNMAT-Externals && rm -r CNMAT-Externals
