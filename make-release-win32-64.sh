archive_name=CNMAT_Externals-Max-Win32_64-`git describe --tags --long`-`git branch | egrep '^\*' | awk '{print $2}'`.zip
dirs=(docs help media misc java-classes)

mkdir CNMAT-Externals && mkdir CNMAT-Externals/externals && mkdir CNMAT-Externals/java-classes

cp build/Release/*.mxe* CNMAT-Externals/externals
cp build/Release/*.class CNMAT-Externals/java-classes

for f in ${dirs[*]}
do
	[ -e $f ] && cp -r $f CNMAT-Externals
done

# force-copy an old version of resonators~
# cp -f kludge/resonators~.mxe CNMAT-Externals/externals
# this is no longer needed AFAICT, rama (sept 2017)
python make-package-info.py

cp package-info.json CNMAT-Externals
cp license.txt CNMAT-Externals

zip -r $archive_name CNMAT-Externals && rm -r CNMAT-Externals
