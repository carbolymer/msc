#!/bin/bash
#PBS -N Therminator2
#PBS -k o
#PBS -j oe
#PBS -o ~/log/$PBS_JOBID.log
#PBS -e ~/log/$PBS_JOBID.err


#
#         -== SKYNET ==-
# The Therminator 2 manager 0.1.2a
#       by Mateusz Galazyn
#

# Therminator directory
THERMINATOR_DIRECTORY=th_5.7

# Number of events to generate per job
# EVNTS_PER_JOB=3750
EVNTS_PER_JOB=2

# Executable file
EXECUTABLE="therm2_events"


### End of configuration
SKYNET_DIR=$THERMINATOR_DIRECTORY/skynet
LOG_DIR=$SKYNET_DIR/log
JOB_DIR=$SKYNET_DIR/$PBS_JOBID
EVENT_DIR=$JOB_DIR/events


function prepare_events_ini()
{
EVNTS_INI="[FreezeOut]
FreezeOutModel = Lhyquid3V
# FreezeOutModelINI =

[Event]
NumberOfEvents = $EVNT_NR
EventFileType = root

[Primordial]
MultiplicityDistribution = Poisson
IntegrateSamples = 5000000

[Random]
Randomize = 1

[Directories]
ShareDir = share/
FreezeOutDir = fomodel/
MacroDir = macro/
EventDir = ../$EVENT_DIR/

[Logging]
LogFile = ../$LOG_DIR/$PBS_JOBID.log
"
}

function prepare_workspace()
{
  prepare_events_ini
  mkdir -p $EVENT_DIR
  mkdir -p $LOG_DIR
  echo "$EVNTS_INI" >> $JOB_DIR/events.ini  
}

prepare_workspace
. /etc/root_v5.sh
cd $THERMINATOR_DIRECTORY
./$EXECUTABLE ../$JOB_DIR/events.ini
