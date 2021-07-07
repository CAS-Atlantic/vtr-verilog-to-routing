#!/bin/bash
VTR_ROOT=`pwd`;VTR_ROOT=${VTR_ROOT%vtr-verilog-to-routing*};VTR_ROOT=$VTR_ROOT"vtr-verilog-to-routing/"
TASK_PATH=`pwd`
TASK_NAME=${TASK_PATH%"config"}
TASK_NAME=${TASK_NAME##*"tasks/"}

if [ "$TASK_NAME" != "mixing_optimization/like_carry_chain_adders_correctness" ] ; then
    echo "The script should be executed from within the vtr_flow/tasks/mixing_optimization/like_carry_chain_adders_correctness/  folder"
    exit -1
fi
sed -i 's/<mix_soft_hard_blocks .*/<mix_soft_hard_blocks adders="1" adders_ratio="0.0"><\/mix_soft_hard_blocks>/g' config/multconfig.xml 
rm -rf run0*
rm -rf 0.*
rm -rf 1.0
prevvar=0.00
for (( i=0; i<=20; i++ ))
do
var=`python -c "print($i*1.0/20)"`
sed -i 's/adders_ratio="'$prevvar'"/adders_ratio="'$var'"/g' config/multconfig.xml 
prevvar=$var
python3 $VTR_ROOT/vtr_flow/scripts/run_vtr_task.py -j8 $TASK_NAME
mv run001 $var
here=`pwd`
cd $var
for elem in $(find . -mindepth 3  -maxdepth 3 -type d)
do
    cd $elem
    cp -r simulation_init simulation_use_this
    cp odin_config.xml no_opt_config.xml
    sed -i 's/odin.blif/odin.second_run.blif/g' ./no_opt_config.xml
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
    cd ../../..
done
cd $here
done
