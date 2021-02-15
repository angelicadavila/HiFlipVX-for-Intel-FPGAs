#**************************************************************************
# This scrips reads the RTL generated from HiFlipX graph for FPGA
# Read the files on tools_HLSToOpenCL wich contains:
#  opencl_template.cl  function template of OpenCL
#  c_model.cl template of the graph function implemented in HiFlipVX
#  rtl_template.xml template to read RTL information to encapsulate in OpenCL
#  
#  usage: /first execute Makefile
#	     ./create_graph "Name of OpenCL function"	   
#  
#  Author: Ang√lica D√vila  - Universidad de Zaragoza
#  
#**************************************************************************

#Color for warnings

RED='\033[0;31m'
NC='\033[0m' # No Color
#end ccolor for warnings
echo "FPGA with HBM? Y/N"
read FPGA_HBM
if [ "$FPGA_HBM" == "Y" ]; then
	echo "FPGA with HBM: e.g Stratix10 MX"
else
	echo "FPGA with external DRAM"
fi

func_name=$1
#moving_name=$2
#verify input name
if [ -z "$func_name" ]
then
	echo "graph funtion name is required"
	exit 1
fi

#print Library name
echo $1
#Build RTL for Intel FPGA
$make
#verify if the graph is already generated
if [ -d "./../"$func_name"" ] 
then
	 echo "Function already exist"
else

	mkdir ./../"$func_name"
  
#ToDo: Move the en if to avoid overwriting
fi
echo "creating directory:" 
echo "                    ./../"$func_name""

#get graph function name from HiFlipVx
HFfunc_name="$(basename *.prj/components/*)"
echo "$intfunc_name"
echo "$HFfunc_name"

if [ "$func_name" != "$HFfunc_name" ]
then
	echo -e " ${RED} warning ${NC}: Library name is different to HiFlipVx graph name"
fi

#report of RTL in the main dir
cp -r HiFlipVX_Intel.prj/reports ./../$func_name 
mv ./../$func_name/reports ./../$func_name/report_HFVX_grahp 

cp -r *.prj/components/$HFfunc_name ./../"$func_name"
cp ./tool_HLSToOpenCL/* ./../"$func_name"/"$HFfunc_name"

cd ./../$func_name/"$HFfunc_name" 
#GX

if [ "$FPGA_HBM" == "N" ]; then
	sed -i s/FunctionTemplateName/"$HFfunc_name"/g rtl_template.xml 
	sed -i s/FunctionTemplateName/"$HFfunc_name"/g c_model.cl
	sed -i s/FunctionTemplateName/"$HFfunc_name"/g opencl_template.cl
fi

#MX
if [ "$FPGA_HBM" == "Y" ]; then
	sed -i s/FunctionTemplateName/"$HFfunc_name"/g rtl_templateMX.xml 
	rm rtl_template.xml
	mv rtl_templateMX.xml rtl_template.xml
	sed -i s/FunctionTemplateName/"$HFfunc_name"/g openclMX_template.cl
	sed -i s/FunctionTemplateName/"$HFfunc_name"/g c_modelMX.cl
	mv openclMX_template.cl "$func_name".cl
fi

pwd
./genREQUIREMENTS.sh $HFfunc_name 
./genlib.sh "$HFfunc_name" 

telegram "%F0%9F%9A%80 start Compilation ..."
aoc -profile -v -l "$HFfunc_name"_lib.aoclib "$func_name".cl -o ./../device/"$func_name"_openCL.aocx
telegram "end Macizo %F0%9F%8C%BD"

cd ../..
#cp -r $func_name $moving_name
