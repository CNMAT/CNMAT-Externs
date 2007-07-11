
set NumTracks 5

set TrackColors {red blue green cyan magenta yellow darkgreen}


proc ReadTacTexData {fname} {

    set chn [open $fname r]
    set data {}
    while {![eof $chn]} {
	set line [gets $chn]
	if {[llength $line] == 130} {
	    lappend data [lrange $line 2 end]
	}
    }
    return $data
}

set TacTexData {}

proc SetColor {x} {
    return [format "#%2X%2X%2X" $x $x $x]
}

proc InitData {} {
    global TacTexData

    set black {}
    set white {}
    for {set i 0} {$i < 126} {incr i} {
	lappend black 0
 	lappend white 250
    }
    set TacTexData [list $black $white]
}

set pointsInUse 0

proc ShowFrame {index} {
    global TacTexData

    set goodpoints {}
    set maxpress 0
    set mins 0

    set frame [lindex $TacTexData $index]

    for {set i 0} {$i < 126} {incr i} {
	set taxel [lindex $frame $i]
	if {$taxel > $::threshold} {
	    lappend goodpoints [list $i $taxel]
	    if {[expr $i % 14] == 0} {
		set dx1 100
	    } else {
		set dx1 [expr $taxel - [lindex $frame [expr $i - 1]]]
	    }
	    if {[expr $i % 14] == 13} {
		set dx2 -100
	    } else {
		set dx2 [expr [lindex $frame [expr $i + 1]] - $taxel]
	    }
	    if {$i < 14} {
		set dy1 100
	    } else {
		set dy1 [expr $taxel - [lindex $frame [expr $i - 14]]]
	    }
	    if {$i > 111} {
		set dy2 -100
	    } else {
		set dy2 [expr [lindex $frame [expr $i + 14]] - $taxel]
	    }
	    if {
		($dx1 >= 0 && $dx2 < 0) &&
		($dy1 >= 0 && $dy2 < 0)
	    } {
		incr mins
	    }
	    
	}
	if {$taxel > $maxpress} {
	    set maxpress $taxel
	}
	.c itemconfigure taxel$i -fill [SetColor $taxel]
    }
    
    if {$::shouldTrack} {
	if {[llength $goodpoints] > 0} {
	    set ::pointsInUse 1
	    set centroids [FindCentroids $goodpoints $mins]
	    #puts [km debug]
	    for {set j 0} {$j < [llength $centroids]} {incr j} {
		set centroid [lindex $centroids $j]
		if {[lindex $centroid 2] <= 0.0} {
		    .c delete centroid$j
		    set ::press$j 0 
		} else {
		    set x [expr [lindex $centroid 0] * 20 + 5]
		    set y [expr [lindex $centroid 1] * 20 + 5]
		    if {[llength [.c find withtag centroid$j]] == 0} {
			.c create oval $x $y [expr $x + 10] [expr $y + 10] \
				-tag centroid$j \
				-fill [lindex $::TrackColors $j]
		    } else {
			.c coords centroid$j $x $y [expr $x + 10] \
				[expr $y + 10]
		    }
		    set ::press$j [lindex $centroid 2]
		}
	    }
	} else {
	    if {$::pointsInUse} {
		for {set i 0} {$i < $::NumTracks} {incr i} {
		    .c delete centroid$i
		}
	    }
	}
    }
    .info configure -text "[llength $goodpoints] Pressure Points      Peaks: $mins"
}

proc FindCentroids {points peaks} {
    set coords {}
    set weights {}
    foreach point $points {
	set x [expr [lindex $point 0] % 14]
	set y [expr [lindex $point 0] / 14]
	lappend coords [list $x $y]
	lappend weights [lindex $point 1]
    }
    if {$::peaklimit} {
	return [km $coords $weights $peaks $::heuristic]
    } else {
	return [km $coords $weights 5 $::heuristic]
    }
}

load kmeans.dll
kmeans km 5

proc OpenFile {} {

    global TacTexData

    set fname [tk_getOpenFile]
    if {$fname != ""} {
	set TacTexData [ReadTacTexData $fname]
    }
    .s configure -from 0 -to [expr [llength $TacTexData] - 1]
    #km init
}


InitData

canvas .c -width 280 -height 180
for {set i 0} {$i < 126} {incr i} {
    set x [expr $i % 14]
    set y [expr $i / 14]
    set x [expr $x * 20]
    set y [expr $y * 20]
    .c create rectangle $x $y [expr $x + 20] [expr $y + 20] -fill "#000" -tag taxel$i
}

scale .threshold -from 0 -to 100 -orient horizontal -variable threshold -label "Threshold"
set threshold 40
scale .s -from 0 -to 1 -orient horizontal -command ShowFrame
checkbutton .track -text "Tracking" -variable shouldTrack
set shouldTrack 0
frame .heuristic
radiobutton .heuristic.0 -text "Simple distance" -value 0 -variable heuristic
radiobutton .heuristic.1 -text "Cluster size" -value 1 -variable heuristic
radiobutton .heuristic.2 -text "Cluster age" -value 2 -variable heuristic
checkbutton .heuristic.3 -text "Limit by peaks" -variable peaklimit
pack .heuristic.0 .heuristic.1 .heuristic.2 .heuristic.3 -side left
label .info -text ""
button .load -text "Load" -command OpenFile

pack .load .s .threshold .heuristic .track .info -side bottom -fill x -expand yes
#pack .threshold -side left
pack .c -side left
for {set i 0} {$i < 5} {incr i} {
    scale .p$i -from 3000 -to 0 -orient vertical -variable press$i
    pack .p$i -side left
}
