#!/bin/bash
#
#         -== SKYNET ==-
# The Therminator 2 manager 0.1a
#       by Mateusz Galazyn
#

# Therminator2 location
TH2_DIR="/data/home/galazyn/Therminator2"

# Event files location
EVNT_DIR="/data/home/galazyn/Therminator2/events"

# Skynet workspace - has to be relative to the TH2_DIR
REL_WORKSPACE="skynet"
WORKSPACE=$TH2_DIR/$REL_WORKSPACE

# Logs path
LOGS="$REL_WORKSPACE/log"

# Number of events to generate by single drone
EVNT_NR=10000

# Maximum number of running jobs per node
JOBS_PER_NODE=3

# List of nodes to run therminator on
NODELIST="node1057
node1060
node1061
node1062
node1063
node1064
node1065
node1066
node1067
node1069
node1070
node1071
node1072"

# Executable file
EXECUTABLE="therm2_events"

# HBT script
HBT_EXEC="runhbt.sh"

# Node working domain
NODEDOMAIN="grid4cern.if.pw.edu.pl"

# Commands used to launch generation of figures
function extract_figures_cmds()
{
FIGURESCMDS=$(cat <<THEVERYENDOFME
root -b -l -q -x "./macro/figure_distpt.C(\"$EVNT_MODEL_DIR\", $EVNT_COUNT)"
root -b -l -q -x "./macro/figure_distpt_exotic.C(\"$EVNT_MODEL_DIR\", $EVNT_COUNT)"
root -b -l -q -x "./macro/figure_distpt_pion.C(\"$EVNT_MODEL_DIR\", $EVNT_COUNT)"
root -b -l -q -x "./macro/figure_distpt_proton.C(\"$EVNT_MODEL_DIR\", $EVNT_COUNT)"
root -b -l -q -x "./macro/figure_distmt.C(\"$EVNT_MODEL_DIR\", $EVNT_COUNT)"
root -b -l -q -x "./macro/figure_v2pt.C(\"$EVNT_MODEL_DIR\", $EVNT_COUNT)"
root -b -l -q -x "./macro/figure_v2rap.C(\"$EVNT_MODEL_DIR\", $EVNT_COUNT)"
root -b -l -q -x "./macro/figure_distxyt_pion.C(\"$EVNT_MODEL_DIR\", $EVNT_COUNT)"
root -b -l -q -x "./macro/figure_disteta.C(\"$EVNT_MODEL_DIR\", $EVNT_COUNT)"
root -b -l -q -x "./macro/figure_distrap.C(\"$EVNT_MODEL_DIR\", $EVNT_COUNT)"
root -b -l -q -x "./macro/figure_distrap_prim.C(\"$EVNT_MODEL_DIR\", $EVNT_COUNT)"
root -b -l -q -x "./macro/figure_xpx.C(\"$EVNT_MODEL_DIR\", $EVNT_COUNT)"
root -b -l -q -x "./macro/figure_hsxyt.C(\"$EVNT_MODEL_DIR\", $EVNT_COUNT)"
root -b -l -q -x "./macro/figure_hsret.C(\"$EVNT_MODEL_DIR\", $EVNT_COUNT)"
THEVERYENDOFME
)
}

function prepare_events_ini()
{
EVNTS_INI="#********************************************************************************
#*                                                                              *
#*             THERMINATOR 2: THERMal heavy-IoN generATOR 2                     *
#*                                                                              *
#* Version:                                                                     *
#*      Release, 2.0.3, 1 February 2011                                         *
#*                                                                              *
#* Authors:                                                                     *
#*      Mikolaj Chojnacki   (Mikolaj.Chojnacki@ifj.edu.pl)                      *
#*      Adam Kisiel         (kisiel@if.pw.edu.pl)                               *
#*      Wojciech Broniowski (Wojciech.Broniowski@ifj.edu.pl)                    *
#*      Wojciech Florkowski (Wojciech.Florkowski@ifj.edu.pl)                    *
#*                                                                              *
#* Project homepage:                                                            *
#*      http://therminator2.ifj.edu.pl/                                         *
#*                                                                              *
#* For the detailed description of the program and further references           *
#* to the description of the model please refer to                              *
#* http://arxiv.org/abs/1102.0273                                               *
#*                                                                              *
#* This code can be freely used and redistributed. However if you decide to     *
#* make modifications to the code, please, inform the authors.                  *
#* Any publication of results obtained using this code must include the         *
#* reference to arXiv:1102.0273 and the published version of it, when           *
#* available.                                                                   *
#*                                                                              *
#********************************************************************************/

[FreezeOut]
# ----------------------|-------------------------------|
#   FreezeOutModel      |  FreezeOutModelINI            |
# ----------------------|-------------------------------|
#   KrakowSFO           |  fomodel/krakow.ini           |
#   BlastWave           |  fomodel/blastwave.ini        |
#   BWAVT               |  fomodel/bwa.ini              |
#   BWAVTDelay          |  fomodel/bwa.ini              |
#   BWAVLinear          |  fomodel/bwa.ini              |
#   BWAVLinearDelay     |  fomodel/bwa.ini              |
#   BWAVLinearFormation |  fomodel/bwa.ini              |
#   Lhyquid3D           |  fomodel/lhyquid3d.ini        |
#   Lhyquid2DBI         |  fomodel/lhyquid2dbi.ini      |
# ----------------------|-------------------------------|
# Name of the freeze-out model
# available: see table above
# default:	Lhyquid2DBI
FreezeOutModel = Lhyquid2DBI

# Custom freeze-out model ini file [not used by default]
# default:
# FreezeOutModelINI =

[Event]
# Number of events to generate 
# default:	50000
NumberOfEvents = $EVNT_NR

# Event output file format
# available:	root, root&text, text
# default:	root
EventFileType = root

[Primordial]
# Distribution of primordial particles multiplicity
# available:	Poisson
# default:	Poisson
MultiplicityDistribution = Poisson

# Number of samples used in determination of
# primordial multiplicity and max. integrand value
# default:	5000000
IntegrateSamples = 5000000

[Random]
# Start each event with a new random seed taken from current time (1)
# or do a constant seed (0)
# default:	1
Randomize = 1

[Directories]
# Directory with SHARE input files
# default:	share/
ShareDir = share/

# Directory with Freeze-Out Model parameter files
# default:	fomodels/
FreezeOutDir = fomodel/

# Directory with ROOT macro files *.C
# default:	macro/
MacroDir = macro/

# Directory to write the events
# default:	events/
EventDir = $REL_EVNT_WORKSPACE/
`echo "$_NODELIST" | wc -l`
[Logging]
# Log file
# default:	therminator.log
LogFile = $LOGS/therminator.$PID.log
"
}

function launch_all_drones()
{
	count=$1
	for node in $NODELIST
	do
		for (( i=1 ; i<=$JOBS_PER_NODE ; i++ ))
		do 
			if [ ! -d $WORKSPACE/events/$node.$i ] ; then
				pdsh -w ssh:$node.$NODEDOMAIN "$0 launchdrone $i &" &
				count=$[count-1]
				if [ $count -le 0 ] ; then
					break 2
				fi 	
			fi
		done		
	done
}

# copy the primordial particle multiplicity file to a new folder and create directory tree
function prepare_workspace()
{
	prepare_events_ini
	mkdir $EVNT_WORKSPACE
	for dirname in `ls $EVNT_DIR`
	do
		mkdir $EVNT_WORKSPACE/$dirname/
		cp $EVNT_DIR/$dirname/*.txt $EVNT_WORKSPACE/$dirname/
		echo "$EVNTS_INI" >> $EVNT_WORKSPACE/$dirname/events.ini
	done
}

function launch()
{
	echo "" > $TH2_DIR/$LOGS/lastrun.$PID.log
	. /etc/root_v5.sh
	cd $TH2_DIR
	./$EXECUTABLE $EVNT_WORKSPACE/$dirname/events.ini &> $LOGS/lastrun.$PID.log
}

# merge generated results
function clear_the_mess()
{
	# listing of the workdir
	echo "$EVNT_WORKSPACE"
	for dirname in `ls $EVNT_WORKSPACE`
	do
		# if model subdirectory does not exist - create it
		if [ ! -d $EVNT_DIR/$dirname/ ] ; then
			mkdir $EVNT_DIR/$dirname
		fi
		lastEventFileName=`ls --format=single-column $EVNT_DIR/$dirname | grep .root | grep event | tail -n 1`
		lastEventNr=${lastEventFileName//[^0-9]/}
		lastEventNr=${lastEventNr##*0}
		# copy from workspace to event directory
		for treeFile in `ls --format=single-column $EVNT_WORKSPACE/$dirname | grep .root | grep event`
		do
			# TODO over 1000 events
			while [ -f $EVNT_DIR/$dirname/event`printf "%03d\n" ${lastEventNr//[^0-9]/}`.root ]
			do
				lastEventNr=$(($lastEventNr+1))
				if [ $lastEventNr -gt 999 ] ; then
					touch $WORKSPACE/OVER1000.$PID
					exit
				fi
			done
			mv $EVNT_WORKSPACE/$dirname/$treeFile $EVNT_DIR/$dirname/event`printf "%03d\n" ${lastEventNr//[^0-9]/}`.root
		done
		rm -rf $EVNT_WORKSPACE
	done
}

#dispatches merging through all hosts
function dispatch_merging()
{
	for node in $NODELIST
        do
                for (( i=1 ; i<=$JOBS_PER_NODE ; i++ ))
                do
	                if [ -d $WORKSPACE/events/$node.$i ] ; then
				pdsh -w ssh:$node.$NODEDOMAIN "$0 mergemyself $i &" &
       			fi
		done
        done
}

function show_status()
{
	echo "Gathering information. This may take a while, please wait."
	sum=0
        for node in $NODELIST
        do
		sum=$[`pdsh -N -w ssh:$node.$NODEDOMAIN "ps -u $USER | grep $EXECUTABLE" 2> /dev/null | wc -l`+sum]
        done
#	sum=$[`ls $WORKSPACE/events -l | wc -l`-1]
	events_=$[`ls -lR $WORKSPACE/events | grep event | grep .root | wc -l`*500]
	events_max_=$[sum*EVNT_NR]
	echo -e "Running threads:\t\t$sum"
	if [ $sum -gt 0 ] ; then
		echo -e "Generated events in all threads:\t$events_/$events_max_"
	fi
#	sum=0
#	for node in $NODELIST
#	do
#		sum=$[`pdsh -N -w ssh:$node.$NODEDOMAIN "ps -o cmd -u $USER | grep root.exe | wc -l" 2> /dev/null`+sum-2]
 #       done
#	sum=$[sum+1]
#	echo -e "Running macros:\t\t\t$sum"
}

# executes root's macros ands starts hbt analysis
function dispatch_analysis()
{
	# list all used models in event directory
	# figures generation
	for model in `ls $EVNT_DIR`	
	do
		if [ ! -d $EVNT_DIR/$model ] ; then
			continue
		fi
		EVNT_MODEL_DIR=$EVNT_DIR/$model/
		EVNT_COUNT=`ls -1 $EVNT_MODEL_DIR | grep ^event | grep .root$ | wc -l`
		i=0
		extract_figures_cmds
                NODE=`echo "$_NODELIST" | head -n 1`
		IFS=$'\n'
		for cmd in $FIGURESCMDS
		do
	                if [ `echo "$_NODELIST" | wc -l` -lt 2 ] ; then
	                        _NODELIST="$NODELIST"
	                fi
			NODE=`echo "$_NODELIST" | head -n 1`
			pdsh -w ssh:$NODE.$NODEDOMAIN "cd $TH2_DIR && . /etc/root_v5.sh && $cmd &> $LOGS/figures.$NODE.$i.log" &
                       	if [ ! $i -lt $JOBS_PER_NODE ] ; then
                                _NODELIST=`echo "$_NODELIST" | sed -e '1d'`
                                i=0
                        fi
			i=$[ i+1 ]
		done
		unset IFS
	done
}

function run_hbt()
{
        for model in `ls $EVNT_DIR`
        do
                if [ ! -d $EVNT_DIR/$model ] ; then
                       	continue
                fi
                EVNT_MODEL_DIR=$EVNT_DIR/$model/
                EVNT_COUNT=`ls -1 $EVNT_MODEL_DIR | grep ^event | grep .root$ | wc -l`
               	i=0
              	if [ `echo "$_NODELIST" | wc -l` -lt 2 ] ; then
			_NODELIST="$NODELIST"
                fi
		NODE=`echo "$_NODELIST" | head -n 1`
             	pdsh -w ssh:$NODE.$NODEDOMAIN "cd $TH2_DIR && . /etc/root_v5.sh && ./$HBT_EXEC $EVNT_MODEL_DIR $EVNT_COUNT &> $LOGS/hbt.$i.log" &
#		echo "cd $TH2_DIR && . /etc/root_v5.sh && $HBT_EXEC $EVNT_MODEL_DIR $EVNT_COUNT &> $LOGS/hbt.$i.log"
                if [ ! $i -lt $JOBS_PER_NODE ] ; then
                	_NODELIST=`echo "$_NODELIST" | sed -e '1d'`
                        i=0
                fi
                i=$[ i+1 ]
       	done
}

PID=${HOSTNAME%%.*}.$2
REL_EVNT_WORKSPACE=$REL_WORKSPACE/events/$PID
EVNT_WORKSPACE=$WORKSPACE/events/$PID

INFO="\n  \033[01;32m ::\033[01;36m Skynet - Therminator 2 Manager 0.1a \033[00m \n"

if [ $# == 0 ] ; then
        echo -e "$INFO
\033[01;37m $0 [option] [number of drones] \033[00m
	possible options:
		\033[01;37mstatus		\033[00m- show generation status
		\033[01;37mstart 5		\033[00m- starts 5 drones
		\033[01;37mlaunchdrone 1	\033[00m- run as a drone with id = 1
		\033[01;37mdispatchanalysis	\033[00m- starts figures generation
		\033[01;37mmerge		\033[00m- merges all already gnerated events
                \033[01;37mrunhbt		\033[00m- starts HBT analysis"
       	exit
fi

case $1 in 
	"status")
		echo -e "$INFO"
		show_status
		;;
	"start")
		if [ $# -gt 1  ] ; then
			if [ $2 -gt 0 ] ; then
				launch_all_drones $2
				echo "Drones have started."
				show_status
			else
				echo "The number of drones has to be greater than 0"
			fi
		else
			echo "You have to provide number of drones to launch."
		fi
		;;
	"launchdrone")
		$0 launchmyselfinbackground $2 &
		;;
	"launchmyselfinbackground")
		prepare_workspace
                launch
                clear_the_mess
		;;
	"dispatchanalysis")
		echo -e "$INFO"
		dispatch_analysis
		;;
        "runhbt")
                echo -e "$INFO"
                run_hbt
                ;;
        "merge")
                echo -e "$INFO"
                dispatch_merging
		echo "Merging finished."
                ;;
        "mergemyself")
                clear_the_mess
                ;;
	*)
		echo "Unknown command: $1"
		;;
esac
