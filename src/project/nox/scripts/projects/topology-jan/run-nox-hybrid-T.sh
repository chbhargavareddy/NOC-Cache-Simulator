#!/bin/tcsh
set mindex = $1
cd /i3c/hpcl/rdas/workspace-linux-simics/modules/pepsi/NoX/nox/scripts  
foreach width (128 256 512 1024) 
   if($mindex == 15 || $mindex == 44 || $mindex == 47 || $mindex ==  57 || $mindex == 63 || $mindex == 87 || $mindex == 59) then
      set mindex   = `expr $mindex + 1`
   endif
   set n = 16 
    ./remote.sh inti$mindex "./run-nox.sh hybrid-T $width 1 2 8 > ../results/log-hybrid-T8-${width}w-${n}n"
   set mindex   = `expr $mindex + 1`
   if($mindex == 15 || $mindex == 44 || $mindex == 47 || $mindex ==  57 || $mindex == 63 || $mindex == 87 || $mindex == 59) then
      set mindex   = `expr $mindex + 1`
   endif
   set n = 32 
   ./remote.sh inti$mindex "./run-nox.sh hybrid-T $width 2 2 8 > ../results/log-hybrid-T8-${width}w-${n}n"
   set mindex   = `expr $mindex + 1`

   if($mindex == 15 || $mindex == 44 || $mindex == 47 || $mindex ==  57 || $mindex == 63 || $mindex == 87 || $mindex == 59) then
      set mindex   = `expr $mindex + 1`
   endif
   set n = 64 
    ./remote.sh inti$mindex "./run-nox.sh hybrid-T $width 2 4 8 > ../results/log-hybrid-T8-${width}w-${n}n"
   set mindex   = `expr $mindex + 1`
   if($mindex == 15 || $mindex == 44 || $mindex == 47 || $mindex ==  57 || $mindex == 63 || $mindex == 87 || $mindex == 59) then
      set mindex   = `expr $mindex + 1`
   endif
   set n = 128 
    ./remote.sh inti$mindex "./run-nox.sh hybrid-T $width 4 4 8 > ../results/log-hybrid-T8-${width}w-${n}n"
   set mindex   = `expr $mindex + 1`

   
end
