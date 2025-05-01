# Create a project
open_project -reset out_proj

# Add design files
add_files example.cpp
# Add test bench & files
add_files -tb example_test.cpp
# add_files -tb result_ref.dat

# Set the top-level function
set_top example

# ########################################################
# Create a solution
open_solution -reset solution1
# Define technology and clock rate
set_part  {xcvu9p-flga2104-1-i}
create_clock -period 25 -name default 

# Source x_hls.tcl to determine which steps to execute
source x_hls.tcl
csim_design

# Set any optimization directives
# End of directives

if {$hls_exec == 1} {
	# Run Synthesis and Exit
	csynth_design
	
} elseif {$hls_exec == 2} {
	# Run Synthesis, RTL Simulation and Exit
	csynth_design
	
	cosim_design
} elseif {$hls_exec == 3} { 
	# Run Synthesis, RTL Simulation, RTL implementation and Exit
	csynth_design
	
	cosim_design
	export_design -format ip_catalog
} else {
	# Default is to exit after setup
	csynth_design
}

exit


