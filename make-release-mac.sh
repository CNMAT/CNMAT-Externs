archive_name=CNMAT_Externals-Max-OSX-`git describe --tags --long`-`git branch | egrep '^\*' | awk '{print $2}'`.zip
dirs=(help media misc docs java-classes extras deprecated)
deprecated=(list-accum.mxo)
dephelp=(list-accum.maxhelp)

mkdir CNMAT-Externals && mkdir CNMAT-Externals/externals && mkdir CNMAT-Externals/java-classes && mkdir CNMAT-Externals/extras
mkdir CNMAT-Externals/deprecated

cp -r build/Release/*.mxo CNMAT-Externals/externals
cp -r build/Release/*.class CNMAT-Externals/java-classes

for f in ${dirs[*]}
do
    [ -e $f ] && cp -r $f CNMAT-Externals
done

for f in ${deprecated[*]}
do
    mv "CNMAT-Externals/externals/$f" CNMAT-Externals/deprecated/
done
for f in ${dephelp[*]}
do
    mv "CNMAT-Externals/help/$f" CNMAT-Externals/deprecated/
done

python make-package-info.py

cp package-info.json CNMAT-Externals
cp license.txt CNMAT-Externals
cp icon.png CNMAT-Externals

zip -r $archive_name CNMAT-Externals && rm -r CNMAT-Externals
