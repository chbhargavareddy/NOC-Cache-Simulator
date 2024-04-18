#!/bin/tcsh
cd /i3c/hpcl/rdas/workspace-linux-simics/modules/pepsi/NoX/nox/results  

foreach file (log-$1-*w-*n)
 echo $file > tn1
 grep "Done" $file | tail -n 1 > tn2
 paste tn1 tn2
 rm tn1 tn2
end
