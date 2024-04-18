#!/bin/tcsh
set mindex = $1
cd /i3c/hpcl/rdas/workspace-linux-simics/modules/pepsi/NoX/nox/scripts  
foreach nodes ( 4 6 8 12 16 32)  
  foreach width (42 64 85 128 256 512) 
   if($mindex == 15 || $mindex == 44 || $mindex == 47 || $mindex ==  57 || $mindex == 63 || $mindex == 87) then
      set mindex   = `expr $mindex + 1`
    endif
    ./remote.sh inti$mindex "./run-nox.sh ring $width $nodes > ../results/log-ring-${width}w-${nodes}n"
   set mindex   = `expr $mindex + 1`
  end
end
