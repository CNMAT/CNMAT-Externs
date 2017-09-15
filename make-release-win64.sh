archive_name=CNMAT_Externals-Max-Win64-`git describe --tags --long`-`git branch | egrep '^\*' | awk '{print $2}'`.zip
dirs=(help media misc)

mkdir CNMAT-Externals && mkdir CNMAT-Externals/externals
cp build/Release/*.mxe64 CNMAT-Externals/externals

for f in ${dirs[*]}
do
	[ -e $f ] && cp -r $f CNMAT-Externals
done

python make-package-info.py

cp package-info.json CNMAT-Externals

zip -r $archive_name CNMAT-Externals && rm -r CNMAT-Externals
