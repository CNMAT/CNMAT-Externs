archive_name=CNMAT_Externals-Max-OSX-`git describe --tags --long`-`git branch | egrep '^\*' | awk '{print $2}'`.zip
dirs=(help media misc docs java-classes extras)

mkdir CNMAT-Externals 
&& mkdir CNMAT-Externals/externals 
&& mkdir CNMAT-Externals/java-classes
&& mkdir CNMAT-Externals/extras

cp -r build/Release/*.mxo CNMAT-Externals/externals
cp -r build/Release/*.class CNMAT-Externals/java-classes

for f in ${dirs[*]}
do
    [ -e $f ] && cp -r $f CNMAT-Externals
done

python make-package-info.py

cp package-info.json CNMAT-Externals
cp license.txt CNMAT-Externals
cp icon.png CNMAT-Externals

zip -r $archive_name CNMAT-Externals && rm -r CNMAT-Externals
