#!/usr/bin/env bash

## Setting var screen

echo  "--------------------------------------------------------------------"
echo  "|     SETTING UP ALL THE ENVIRONEMENT VARIABLES FOR THE SCRIPT     |"
echo  "--------------------------------------------------------------------"

## Setting var

## set
# immediate exiting at failure
# also sets up option-name pipefail that avoid exiting if the pipe could still be executed
# i.e failure_cmd || true
# allow the finition of the command before aborting and result an error, useful for exit cases
set -e -o pipefail

# shortcut to /usr/bin/
bin="/usr/bin/"

# shorcut to /opt/fairrepack/
fairrepack="/opt/fairrepack/"

# shortcut output to /dev/null 2>&1
output="/dev/null 2>&1"
## Functions initialisation
# outerr exits to stderr
outerr (){
  echo "$@" >&2
}

echo ""
echo ""

## Welcome Screen
echo  "--------------------------------------------------------------------"
echo  "|      WELCOME ON THE SAGLISS INDUSTRIES' SHELL INSTALL SCRIPT     |"
echo  "| PLEASE MAKE SURE THAT YOU ARE RUNNING THE INSTALL SCRIPT AS ROOT |"
echo  "--------------------------------------------------------------------"

sleep 2

## check if the userID is (ne) not equal to root, if yes then exit failure
if [ "$EUID" -ne 0 ]
then outerr "You are not root !"
outerr "aborting..."
exit 0
fi

## Compilation
cmake CMakeLists.txt
make extractor
make merger
#copy to /usr/bin/
cp extractor merger $bin

#owned by root user in case it don't
chown root:root ${bin}extractor
chown root:root ${bin}merger

#chmod the app to permit all users
chmod 755 ${bin}extractor
chmod 755 ${bin}merger

#make dir fairrepack in /opt
mkdir -p ${fairrepack}

#symlink the apps in /opt/fairrepack/
ln -s ${bin}extractor ${fairrepack}extractor
ln -s ${bin}merger ${fairrepack}merger

#Generating the cron jobs
# m h DoM MoN DoW command output
echo "55 23 * * * extractor output" >> /var/spool/cron/contabs/root
echo "0 0 1 * * merger output" >> /var/spool/cron/contabs/root