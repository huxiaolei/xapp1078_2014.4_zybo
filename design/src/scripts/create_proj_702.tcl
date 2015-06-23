

# Create project
set proj_name project_1
create_project -force $proj_name ./$proj_name

# Set project properties
set obj [get_projects $proj_name]
set_property "part" "xc7z020clg484-1" $obj
set_property "target_language" "Verilog" $obj
set_property board_part xilinx.com:zc702:part0:1.0 $obj

# Set the directory path for the new project
set proj_dir [get_property directory $obj]

set_property ip_repo_paths  $proj_dir/../../src/my_ip [current_fileset]
update_ip_catalog

#create BD
source $proj_dir/../../src/scripts/create_bd_702.tcl
validate_bd_design
save_bd_design

#Create top wrapper file
make_wrapper -files [get_files $proj_dir/$proj_name.srcs/sources_1/bd/design_1/design_1.bd] -top
import_files -force -norecurse $proj_dir/$proj_name.srcs/sources_1/bd/design_1/hdl/design_1_wrapper.v

#implement the design and create bit file
launch_runs impl_1 -to_step write_bitstream
wait_on_run -timeout 60 impl_1

#Export design to SDK
file mkdir $proj_dir/$proj_name.sdk
file copy -force $proj_dir/$proj_name.runs/impl_1/design_1_wrapper.sysdef $proj_dir/$proj_name.sdk/design_1_wrapper.hdf
launch_sdk -workspace $proj_dir/$proj_name.sdk -hwspec $proj_dir/$proj_name.sdk/design_1_wrapper.hdf
