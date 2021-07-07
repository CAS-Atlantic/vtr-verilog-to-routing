# To use this script, you need the following:
# create a directory of the task under a name of your preference
# Within the directory, please provide the config.txt, as per run_vtr_flow guidances (see config.txt for example)
# note the -use_odin_xml (in this example, use_odin_xml is set to multconfig.xml)
# in the same folder, provide your custom odin configuration (for example, look at multconfig.xml)
#  NOTE THE CIRCUITS FILES in the task point at the circuit located under vtr_flow, can't access the ones in any other subfolder.


# UPDATE your vtr_root if it's not detected automatically
VTR_ROOT=`pwd`;VTR_ROOT=${VTR_ROOT%vtr-verilog-to-routing*};VTR_ROOT=$VTR_ROOT"vtr-verilog-to-routing/"
TASK_PATH=`pwd`
TASK_NAME=${TASK_PATH%"config"}
TASK_NAME=${TASK_NAME##*"tasks/"}
python3 $VTR_ROOT/vtr_flow/scripts/run_vtr_task.py $TASK_NAME
latest=`ls .. | sort | tail -n 1`
cd ../$latest/*/*/common/ 
cp -r simulation_init simulation_use_this
cp odin_config.xml no_opt_config.xml
sed -i 's/odin.blif/odin.second_run.blif/g' ./no_opt_config.xml
# Double check if the no_opt_config actually does what it is supposed to do and eliminates your optimization
sed '/mix_soft_hard_blocks/d' -i ./no_opt_config.xml
$VTR_ROOT/ODIN_II/odin_II -c ./no_opt_config.xml -t ./simulation_use_this/input_vectors -T ./simulation_use_this/output_vectors &> custom_sim_res.out
echo "==================================================="
echo "Disregard previous message about failed simulation"
echo "The result is"
echo "==================================================="
grep "matches output"  custom_sim_res.out
echo "==================================================="
echo "Details are available in"
NEW_PATH=`pwd`
echo "$NEW_PATH""/custom_sim_res.out" 