#!/bin/tcsh
set mindex = $1
cd /i3c/hpcl/rdas/workspace-linux-simics/modules/pepsi/NoX/nox/scripts  
foreach nodes (2 4 6 8 16)  
  foreach width (128 256 512 1024) 
   if($mindex == 15 || $mindex == 44 || $mindex == 47 || $mindex ==  57 || $mindex == 63 || $mindex == 87) then
      set mindex   = `expr $mindex + 1`
   endif
   set n = `echo "scale = 2 ; $nodes*8" | bc` 
    ./remote.sh inti$mindex "./run-nox.sh hybrid-R $width $nodes 8 > ../results/log-hybrid-R8-${width}w-${n}n"
   set mindex   = `expr $mindex + 1`
   if($mindex == 15 || $mindex == 44 || $mindex == 47 || $mindex ==  57 || $mindex == 63 || $mindex == 87) then
      set mindex   = `expr $mindex + 1`
   endif
   set n = `echo "scale = 2 ; $nodes*6" | bc` 
    ./remote.sh inti$mindex "./run-nox.sh hybrid-R $width $nodes 6 > ../results/log-hybrid-R6-${width}w-${n}n"
   set mindex   = `expr $mindex + 1`
   end
end
