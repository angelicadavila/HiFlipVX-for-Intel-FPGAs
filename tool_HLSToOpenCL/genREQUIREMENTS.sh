# angelicadg@unizar.es
################################################################################
# Help                                                                         #
################################################################################
Help()
{
   echo "Generates -xml file to be in an OpenCL library"
   echo
   echo "Insert all the parameters in the rtl from ip directory"
   echo "options:"
   echo "g     Print the GPL license notification."
   echo "h     Print this Help."
   echo "v     Verbose mode."
   echo "V     Print software version and exit."
   echo
}

################################################################################
# main                                                                         #
################################################################################
set -e
fun_name=$1

ls \ip >> file_ip.txt
sed 's/^/ <FILE name="ip\/&/g' file_ip.txt > file_iptmp.txt
sed 's/$/" \/>/' file_iptmp.txt > file_ip.txt
sed '72r file_ip.txt' rtl_template.xml > rtl_graph.xml
rm file_ip.txt
