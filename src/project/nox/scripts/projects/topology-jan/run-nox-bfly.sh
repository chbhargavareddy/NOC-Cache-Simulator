#!/bin/tcsh
# runs bfly's of sizes 16,27,64,125
set mindex = $1
cd /i3c/hpcl/rdas/workspace-linux-simics/modules/pepsi/NoX/nox/scripts  
foreach ary (3 4 5)  
  foreach width (42 64 85 128 256 512) 
   
   if($mindex == 15 || $mindex == 44 || $mindex == 47 || $mindex ==  57 || $mindex == 63 || $mindex == 87) then
      set mindex   = `expr $mindex + 1`
    endif
    set $fly = 3 
    set nodes = `echo "scale = 2 ; $ary^$fly" | bc` 
    ./remote.sh inti$mindex "./run-nox.sh bfly $width $ary $fly > ../results/log-bfly-${width}w-${nodes}n" 
    set mindex   = `expr $mindex + 1`
    
    if($ary == 4) then
      if($mindex == 15 || $mindex == 44 || $mindex == 47 || $mindex ==  57 || $mindex == 63 || $mindex == 87) then
        set mindex   = `expr $mindex + 1`
      endif
      set $fly = 2 
      set nodes = `echo "scale = 2 ; $ary^$fly" | bc` 
      ./remote.sh inti$mindex "./run-nox.sh bfly $width $ary $fly > ../results/log-bfly-${width}w-${nodes}n" 
      set mindex   = `expr $mindex + 1`
    endif

  end
end
