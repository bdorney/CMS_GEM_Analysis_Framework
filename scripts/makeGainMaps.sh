#!bin/zsh
# Usage:
#	source makeGainMaps.sh <Summary File>
#
# Here the "Summary File" is a text file with tab delimited data in the format:
#	
#	file	gp0	gp0Err	gp1	gp1Err	name	hvPoint	hvlist
#
# Where:
#	file	physical filename of the Framework output file
#	gp0	Value of p0 in G(x) = exp(p0*x+p1); here x is either divider current or V_Drift
#	gp0Err	Error on p0 in G(x) = exp(p0*x+p1); here x is either divider current or V_Drift
#	gp1	Value of p0 in G(x) = exp(p0*x+p1); here x is either divider current or V_Drift
#	gp1Err	Error on p0 in G(x) = exp(p0*x+p1); here x is either divider current or V_Drift
#	name	String assigned to "Detector_Name" field in the run config file used to make the Framework output file
#	hvPoint	HV value at which the original QC5_Resp_Uni measurement was performed at (input either divider current or V_Drift)
#	hvlist	comma separated list of hv points for which the gain map should be calculated at

while IFS=$'\t' read -r -A argList
do
	if [[ ${argList[1]} == *"#"* ]]; then
		echo "commented line"
	else
		echo "================="
		echo ""
		echo ""
		echo "processing file: " ${argList[1]}
		echo "with command:"
		echo ""
		echo python2.7 $GEM_BASE/python/computeGainMap.py --file=${argList[1]} --gp0=${argList[2]} --gp0Err=${argList[3]} --gp1=${argList[4]} --gp1Err=${argList[5]} --name=${argList[6]} --hvPoint=${argList[7]} --hvlist=${argList[8]} --fileMap=${argList[9]}
		echo ""
		python2.7 $GEM_BASE/python/computeGainMap.py --file=${argList[1]} --gp0=${argList[2]} --gp0Err=${argList[3]} --gp1=${argList[4]} --gp1Err=${argList[5]} --name=${argList[6]} --hvPoint=${argList[7]} --hvlist=${argList[8]} --fileMap=${argList[9]}
		echo ""
		echo ""
		echo "================="
		
		sleep 1
	fi
done < $1
