#!/bin/bash
VTR_ROOT=`pwd`;VTR_ROOT=${VTR_ROOT%vtr-verilog-to-routing*};
VTR_ROOT=$VTR_ROOT"vtr-verilog-to-routing/"
TASK_PATH=`pwd`
TASK_NAME=${TASK_PATH%"config"}
TASK_NAME=${TASK_NAME##*"tasks/"}

if [ "$TASK_NAME" != "mixing_optimization/like_carry_chain_adders" ] ; then
    echo $TASK_NAME
    echo "The script should be executed from within the vtr_flow/tasks/mixing_optimization/like_carry_chain_adders/  folder"
    exit -1
fi
sed -i 's/<mix_soft_hard_blocks .*/<mix_soft_hard_blocks adders="1" adders_ratio="0.0"><\/mix_soft_hard_blocks>/g' config/multconfig.xml 
rm -rf run0*
rm -rf 0.*
rm -rf 1.0
prevvar=0.00
for (( i=0; i<=20; i++ ))
do
var=`python -c "print($i/20)"`
sed -i 's/adders_ratio="'$prevvar'"/adders_ratio="'$var'"/g' config/multconfig.xml 
prevvar=$var
python3 $VTR_ROOT/vtr_flow/scripts/run_vtr_task.py -j8 $TASK_NAME
mv run001 $var
done
