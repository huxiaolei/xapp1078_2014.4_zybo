# Readme

This project is to create a GUI and Simulink model by using MPC (Model Predictive Control) for a hybrid energy storage system consisting Battery and Ultra-capacitor. THe work is published with the title of "Optimization of battery energy storage system with super-capacitor for renewable energy applications" on 2011 IEEE 8th International Conference on Power Electronics and ECCE Asia (ICPE & ECCE). The paper is accessible through IEEE Xplore Digital Library, please click [here](http://ieeexplore.ieee.org/xpl/articleDetails.jsp?tp=&arnumber=5944515&queryText%3DOptimization+of+battery+energy+storage+system+with+super-capacitor+for+renewable+energy+applications).

This project is to describe how to run [xapp1078_2014.4](http://www.wiki.xilinx.com/XAPP1078+Latest+Information) on a zybo board. The major difference between zybo and zc702 is:

* DDR memory on zybo is 512MB while zc702 has 1GB
* There are certain ii2 devices on zc702 that zybo doesn't have.
* Ethernet PHY controller on zybo is different from that is used by zc702.

Based on that, changes are required on:

* Set up a new vivado project and import the design for zc702 and generate new hdf and bit file for zybo board.
* change boot args to limit linux on CPU0 only to use 384MB of DDR memory.
* update system-top.dts for device on zybo.
* Board support package (bsp) for CPU1 has to be changed to limit the usage of DDR memory to lower 128MB (0x18000000 to 0x1fffffff).

## How to use this package

Basically, this package has the same layout as that given by xilinx. The difference is that:

* work directory has been created for zybo. At this moment I don't know how to use tcl to create and set up a project successfully. I tried, but it doesn't work. If anyone knows how to do it, please let me know.
* design/generated_files/ contains BOOT.BIN and image.ub for zybo board.
* docs/Vivado instructions.txt has been modified to show the changes for zybo board.
 