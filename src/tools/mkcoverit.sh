#!/bin/bash

# unpack firejail archive
ARCFIREJAIL=`ls *.tar.bz2| grep firejail`
if [ "$?" -eq 0 ];
then
	echo "preparing $ARCFIREJAIL"
	DIRFIREJAIL=`basename $ARCFIREJAIL  .tar.bz2`
	rm -fr $DIRFIREJAIL
	tar -xjvf $ARCFIREJAIL
	cd $DIRFIREJAIL
	./configure --prefix=/usr
	cd ..
else
	echo "Error: firejail source archive missing"
	exit 1
fi


# unpack firetools archive
ARCFIRETOOLS=`ls *.tar.bz2 | grep firetools`
if [ "$?" -eq 0 ];
then
	echo "preparing $ARCFIRETOOLS"
	DIRFIRETOOLS=`basename $ARCFIRETOOLS .tar.bz2`
	rm -fr $DIRFIRETOOLS
	tar -xjvf $ARCFIRETOOLS
	cd $DIRFIRETOOLS
	pwd
	./configure --prefix=/usr
	cd ..
	
else
	echo "Error: firetools source archive missing"
	exit 1
fi

# move firetools in firejail source tree
mkdir -p $DIRFIREJAIL/extras
mv $DIRFIRETOOLS $DIRFIREJAIL/extras/firetools

# build
cd $DIRFIREJAIL
cov-build --dir cov-int make -j 4 extras
tar czvf myproject.tgz cov-int