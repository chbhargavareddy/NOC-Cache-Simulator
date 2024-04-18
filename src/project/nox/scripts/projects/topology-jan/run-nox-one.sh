#!/bin/tcsh
cd /i3c/hpcl/rdas/workspace-linux-simics/modules/pepsi/NoX/nox/scripts  
foreach ary ( 3 )  
  foreach width (42 64 85 128 256 512) 
    set nodes = `echo "scale = 2 ; $ary^3" | bc` 
    ./run-nox.sh bus $width $ary 3 > ../results/log-bus-${width}w-${nodes}n 
  end
end
