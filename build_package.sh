#! /bin/bash

if [ $# -eq 1 ]
then

rep=$(pwd)/../a2ri-$1

mkdir $rep
cp -r * $rep/
rm $rep/build_package.sh
rm -rf $rep/CVS
rm -rf $rep/*/CVS
rm -rf $rep/*/*/CVS
rm -rf $rep/*/*/*/CVS
rm -rf $rep/*/*/*/*/CVS
rm -rf $rep/*/*/*/*/*/CVS
rm -rf $rep/*/*/*/*/*/*/CVS
rm -rf $rep/*/*/*/*/*/*/*/CVS
rm -rf $rep/*/*/*/*/*/*/*/*/CVS

cd $rep
make clean

exemples=$(ls examples)

for i in $exemples
do
    if [ "$i$" != "CVS" ]
    then
	cd $rep/examples/$i
	make clean
    fi
done

cd $rep/../

tar czvf a2ri-$1.tar.gz a2ri-$1
zip -r a2ri-$1.zip a2ri-$1

rm -rf $rep
fi