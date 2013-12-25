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
THERMINATOR_DIRECTORY=$dir

# Number of events to generate per job
EVNTS_PER_JOB=$events
# EVNTS_PER_JOB=50

# Executable file
EXECUTABLE="therm2_events"


### End of configuration
SKYNET_DIR=$THERMINATOR_DIRECTORY/skynet
LOG_DIR=$SKYNET_DIR/log
JOB_DIR=$SKYNET_DIR/$PBS_JOBID
EVENT_DIR=$JOB_DIR


EVNTS_INI="[FreezeOut]
FreezeOutModel = Lhyquid3V
# FreezeOutModelINI =

[Event]
NumberOfEvents = $EVNTS_PER_JOB
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

function prepare_workspace()
{
  mkdir -p $EVENT_DIR
  mkdir -p $LOG_DIR
  echo "$EVNTS_INI" >> $JOB_DIR/events.ini  

# copy fmultiplicity_*.txt to job's event directory
  for dirname in `ls $THERMINATOR_DIRECTORY/events`
  do
    mkdir -p $EVENT_DIR/$dirname/
    cp $THERMINATOR_DIRECTORY/events/$dirname/*.txt $EVENT_DIR/$dirname/
  done
}

function show_help() 
{
  echo "Przyklad uruchomienia:\r\tqsub -q long -t 0-19 skynet.sh -v dir=th_5.7,events=6000"
}

if [ -z $dir ]
then
  echo "Podaj katalog z Therminatorem uruchomionym przez skynet"
  show_help
  exit
fi

if [ ! -d $dir/events ] && [ ! -d $dir/skynet ]
then
  echo "Nieprawidlowy katalog z Therminatorem"
  show_help
  exit
fi

if [ -z $events ]
then
  echo "Podaj liczbe eventow to wygenerowania przez kazdy proces"
  show_help
  exit
fi

prepare_workspace
. /etc/root_v5.sh
cd $THERMINATOR_DIRECTORY
./$EXECUTABLE ../$JOB_DIR/events.ini
