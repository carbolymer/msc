DIR_HPP=./inc/
DIR_CPP=./src/
DIR_OBJ=./tmp/
DIR_BIN=./
DIR_TMP=./tmp/

CXXFLAGS=`root-config --cflags` -I$(DIR_HPP) -O3 -g 
LFLAGS=`root-config --libs` -lboost_regex -g
OBJS=fit1dcould.o fitshanalyticaaabackshdircovcoulparsqprop.o ktDecoder.o


# search paths
vpath %.xx $(DIR_HPP)
vpath %.cxx $(DIR_CPP)

all: fitsh fit1d ktDecoder

fitsh: fitshanalyticaaabackshdircovcoulparsqprop.o
	echo -e "\033[01;33m[make]\033[00;32m Generating fitsh..."
	echo -e "\033[01;33m[make]\033[01;36m $(addprefix $(DIR_OBJ), $^) \t\033[00;31m$(LFLAGS)\033[00m"
	$(CXX) $(LFLAGS) $(addprefix $(DIR_OBJ), $^) -o $(DIR_BIN)fitsh	

merger: merger.o
	echo -e "\033[01;33m[make]\033[00;32m Generating merger..."
	echo -e "\033[01;33m[make]\033[01;36m $(addprefix $(DIR_OBJ), $^) \t\033[00;31m$(LFLAGS)\033[00m"
	$(CXX) $(LFLAGS) $(addprefix $(DIR_OBJ), $^) -o $(DIR_BIN)merger

fit1d: fit1dcould.o
	echo -e "\033[01;33m[make]\033[00;32m Generating fit1d..."	
	echo -e "\033[01;33m[make]\033[01;36m $(addprefix $(DIR_OBJ), $^) \t\033[00;31m$(LFLAGS)\033[00m"
	$(CXX) $(LFLAGS) $(addprefix $(DIR_OBJ), $^) -o $(DIR_BIN)fit1d

plots:
	echo -e "\033[01;33m[make]\033[00;32m Making plots..."
	root -l -b -q macros/plotter.C

ktDecoder: ktDecoder.o
	echo -e "\033[01;33m[make]\033[00;32m Generating ktDecoder..."
	echo -e "\033[01;33m[make]\033[01;36m $(addprefix $(DIR_OBJ), $^) \t\033[00;31m$(LFLAGS)\033[00m"
	$(CXX) $(LFLAGS) $(addprefix $(DIR_OBJ), $^) -o $(DIR_BIN)ktDecoder

$(OBJS): %.o: %.cxx
	@[ -d $(DIR_OBJ) ] || mkdir -p $(DIR_OBJ)
	@[ -d $(DIR_TMP) ] || mkdir -p $(DIR_TMP)
	echo -e "\033[01;33m[make]\033[01;36m $< \t\033[00;31m$(CXXFLAGS)\033[00m"
	$(CXX) $(CXXFLAGS) $< -o $(DIR_OBJ)$@ -c

clean:
	rm -f $(DIR_OBJ)*.o
	# rm -f $(DIR_OBJ)*.root
	rm -f ./fitsh
	rm -f ./fit1d
	echo -e "\033[01;33m[make]\033[00;32m All *.o and binary files removed.\033[00m"

.PHONY: all clean
.SILENT :
