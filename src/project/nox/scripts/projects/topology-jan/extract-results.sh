#!/bin/tcsh
cd /i3c/hpcl/rdas/workspace-linux-simics/modules/pepsi/NoX/nox/results  
ls -1 out-$1-*-* > nt
echo $1 > dt
set topo = `cut dt -d"-" -f1`
rm dt

set f1 = 3
set f2 = 4

if($topo == "hybrid") then
 set f1 = 4
 set f2 = 5
endif

rm ../final-results/fin-*$1-out.txt 

foreach field (6 4 2 3)
foreach node (`cut nt -d"-" -f$f2 | sort -n -u`)
set index = 1
  foreach width (`cut nt -d"-" -f$f1 | sort -n -u`)
    set file = out-$1-$width-$node
    set param = `cut $file -f $field | head -n 1`
    #echo $file $param
    echo $width > t$index
    cut $file -f $field | grep -v $param >> t$index
    set index = `expr $index + 1`
  end
  #echo "Nodes $node Topology $1" > ../final-results/fin-$param-$1-${node}n-out.txt
  #paste ../scripts/rates t? >> ../final-results/fin-$param-$1-${node}n-out.txt
  echo "Nodes:$node" >> ../final-results/fin-$param-$1-out.txt
  echo $param > index-tmp
  cat ../scripts/rates >> index-tmp
  paste index-tmp t? >> ../final-results/fin-$param-$1-out.txt
  rm index-tmp
  #echo $param
  rm t?
end
end
rm nt
paste ../final-results/fin-*-$1-out.txt > tmp
rm ../final-results/fin-*-$1-out.txt 
mv tmp ../final-results/fin-$1-out.txt 
