#Definitional proc to organize widgets for parameters.
proc create_gui { ipview } {
	set Page0 [ ipgui::add_page $ipview  -name "Page 0" -layout vertical]
	set Component_Name [ ipgui::add_param  $ipview  -parent  $Page0  -name Component_Name ]
	ipgui::add_static_text $ipview -parent $Page0 -name JM_ADDR_SIZE -text {9}
	set C_DPHASE_TIMEOUT [ipgui::add_param $ipview -parent $Page0 -name C_DPHASE_TIMEOUT]
}

proc C_DPHASE_TIMEOUT_updated {ipview} {
	# Procedure called when C_DPHASE_TIMEOUT is updated
	return true
}

proc validate_C_DPHASE_TIMEOUT {ipview} {
	# Procedure called to validate C_DPHASE_TIMEOUT
	return true
}


proc updateModel_C_DPHASE_TIMEOUT {ipview} {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value

	set_property modelparam_value [get_property value [ipgui::get_paramspec C_DPHASE_TIMEOUT -of $ipview ]] [ipgui::get_modelparamspec C_DPHASE_TIMEOUT -of $ipview ]

	return true
}

