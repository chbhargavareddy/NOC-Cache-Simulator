#! /bin/bash

ARGS=2
E_BADARGS=65
ONE=1                     # For the grammar.

if [ $# -ne "$ARGS" ]
then
  echo "Usage: `basename $0` old-extension new-extension (extensions without '.')"
  #  For example, "rn gif jpg" will change all gif files to all jpg files.
  exit $E_BADARGS
fi

number=0                  # Number of files changed.


for filename in *$1      # Find all files 
do
   if [ -f "$filename" ]  # If found
   then
     fname=`basename $filename`            # Separate Path
     n=`echo $fname | sed -e "s/\.$1/\.$2/"` # Change name
     mv $fname $n                          # Rename
     let "number += 1"
   fi
done   

if [ "$number" -eq "$ONE" ]              
then
 echo "Changed $number file."
else 
 echo "Changed $number files."
fi 

exit 0

