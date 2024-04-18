#!/bin/tcsh
set mindex = $1
cd /i3c/hpcl/rdas/workspace-linux-simics/modules/pepsi/NoX/nox/scripts  
foreach width (42 64 85 128 256 512) 
   if($mindex == 15 || $mindex == 44 || $mindex == 47 || $mindex ==  57 || $mindex == 63 || $mindex == 87) then
      set mindex   = `expr $mindex + 1`
    endif
   set nodes = 8 
   ./remote.sh inti$mindex "./run-nox.sh mesh $width 2 4 > ../results/log-mesh-${width}w-${nodes}n" 
   set mindex   = `expr $mindex + 1`

   if($mindex == 15 || $mindex == 44 || $mindex == 47 || $mindex ==  57 || $mindex == 63 || $mindex == 87) then
      set mindex   = `expr $mindex + 1`
    endif
   set nodes = 16 
   ./remote.sh inti$mindex "./run-nox.sh mesh $width 4 4 > ../results/log-mesh-${width}w-${nodes}n" 
   set mindex   = `expr $mindex + 1`


   if($mindex == 15 || $mindex == 44 || $mindex == 47 || $mindex ==  57 || $mindex == 63 || $mindex == 87) then
      set mindex   = `expr $mindex + 1`
    endif
   set nodes = 32 
   ./remote.sh inti$mindex "./run-nox.sh mesh $width 4 8 > ../results/log-mesh-${width}w-${nodes}n" 
   set mindex   = `expr $mindex + 1`


   if($mindex == 15 || $mindex == 44 || $mindex == 47 || $mindex ==  57 || $mindex == 63 || $mindex == 87) then
      set mindex   = `expr $mindex + 1`
    endif
   set nodes = 64 
   ./remote.sh inti$mindex "./run-nox.sh mesh $width 8 8 > ../results/log-mesh-${width}w-${nodes}n" 
   set mindex   = `expr $mindex + 1`

   if($mindex == 15 || $mindex == 44 || $mindex == 47 || $mindex ==  57 || $mindex == 63 || $mindex == 87) then
      set mindex   = `expr $mindex + 1`
    endif
   set nodes = 128 
   ./remote.sh inti$mindex "./run-nox.sh mesh $width 8 16 > ../results/log-mesh-${width}w-${nodes}n" 
   set mindex   = `expr $mindex + 1`
end
