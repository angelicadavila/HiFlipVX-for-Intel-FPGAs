#exm1-> with a burst coalesced unit with size 512
#aocl library hdl-comp-pkg rtl_graph.xml -o opencl_lib.aoco
#aocl library create -name opencl_lib opencl_lib.aoco
#cp opencl_lib.aoclib .. 
#aoc -rtl -v -l opencl_lib.aoclib exm1.cl
#no-- means a return in without class strcuture 
name_lib=$1
set -e 
aocl library hdl-comp-pkg rtl_graph.xml -o "$name_lib"_lib.aoco
aocl library create -name "$name_lib"_lib "$name_lib"_lib.aoco
#cp opencl_lib2048_buff.aoclib .. 
#aoc -v -l "$name_lib"_lib.aoclib "$name_lib".cl -o "$name_lib".aocx
