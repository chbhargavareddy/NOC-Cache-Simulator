#!/bin/tcsh
set mindex = $1
while ( $mindex < $2 )
   if($mindex == 15 || $mindex == 44 || $mindex == 47 || $mindex ==  57 || $mindex == 63 || $mindex == 87) then
      set mindex   = `expr $mindex + 1`
    endif
  ./simkill.sh "inti$mindex"
  set mindex   = `expr $mindex + 1`
end
