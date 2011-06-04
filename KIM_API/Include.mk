#                                                                      
# Copyright 2011 Ellad B. Tadmor, Ryan S. Elliott, and James P. Sethna 
# All rights reserved.                                                 
#                                                                     
# Author: Valeriu Smirichinski                                         
#
# This make file needs to be included by the makefiles in 
# the KIM_API, MODELs/*/ and TESTs/*/ directories.
# It contains definitions for the GNU and Intel compiler sets. 
# It also contains definitions for patern compilation rules.
# 

# The openkim-api subdirectories:
ifndef KIM_API_DIR
KIM_API_DIR :=$(KIM_DIR)KIM_API/
endif
ifndef KIM_MODELS_DIR
KIM_MODELS_DIR:=$(KIM_DIR)MODELs/
endif
ifndef KIM_TESTS_DIR
KIM_TESTS_DIR:=$(KIM_DIR)TESTs/
endif


# All Model .o files and library definitions
# (In the future, this will be generated by the processing pipeline)

MODELOBJ =
ifeq ($(strip $(wildcard $(KIM_API_DIR)model_obj_lib.mk)),$(KIM_API_DIR)model_obj_lib.mk) 
MODELSTOINCLUDE = $(KIM_API_DIR)model_obj_lib.mk
else
MODELSTOINCLUDE =
endif
include $(MODELSTOINCLUDE)

# Determine whether a 32 bit or 64 bit compile should be use
ifdef KIM_SYSTEM32
MACHINESYSTEM=SYSTEM32
else
MACHINESYSTEM=SYSTEM64
endif

ifdef KIM_INTEL

# Define Intel compiler switches
OBJONLY=-c
OUTPUTIN=-o
FORTRANFLAG =-module $(KIM_API_DIR) -D $(MACHINESYSTEM) #-nofor_main
CCOMPILER = icc
CPPCOMPILER = icpc
CPPFLAG = -O3 -I$(KIM_API_DIR) -D KIM_DIR_MODELS=\"$(KIM_MODELS_DIR)\" -D KIM_DIR_API=\"$(KIM_API_DIR)\" -D KIM_DIR_MODELS=\"$(KIM_MODELS_DIR)\"
CPPFLAG += -D KIM_DIR_TESTS=\"$(KIM_TESTS_DIR)\"

CPPLIBFLAG = -nofor_main -cxxlib
FORTRANLIBFLAG = -cxxlib
FORTRANCOMPILER = ifort
LINKCOMPILER = $(FORTRANCOMPILER)

else

# Define GNU compiler switches
OBJONLY=-c
OUTPUTIN=-o
FORTRANFLAG = -fcray-pointer -O3 -J$(KIM_API_DIR) -D $(MACHINESYSTEM)
CCOMPILER   = gcc
CPPCOMPILER = g++
#CCOMPILER   = gcc-fsf-4.4  # for OS X using fink compilers
#CPPCOMPILER = g++-fsf-4.4  # for OS X using fink compilers
CPPFLAG = -O3 -I$(KIM_API_DIR) -Wno-write-strings -D KIM_DIR_MODELS=\"$(KIM_MODELS_DIR)\" -D KIM_DIR_API=\"$(KIM_API_DIR)\"
CPPFLAG += -D KIM_DIR_TESTS=\"$(KIM_TESTS_DIR)\"
FORTRANCOMPILER = gfortran
#CPPLIBFLAG = -lgfortran           #if GNU version 4.5 and up. tested on suse 
#LINKCOMPILER = $(CPPCOMPILER)     #if GNU version 4.5 and up. tested on suse
CPPLIBFLAG = -lstdc++             #if GNU version 4.4.1. tested on suse
LINKCOMPILER = $(FORTRANCOMPILER) #if GNU version 4.4.1. tested on suse

ifdef KIM_DYNAMIC
CPPLIBFLAG += -ldl
endif

endif

# Set max neighbors
ifdef KIM_API_MAX_NEIGHBORS #MAX NEIGHBORS FOR AN ATOM default is 512
CPPFLAG += -D KIM_API_MAX_NEIGHBORS=$(KIM_API_MAX_NEIGHBORS)
endif

# Set common compiler flags for dynamic linking
ifdef KIM_DYNAMIC
CPPFLAG += -D KIM_DYNAMIC=\"$(KIM_DYNAMIC)\" -fPIC
FORTRANFLAG += -fPIC
endif


# Definition of c and fortran .o file list
OBJC = $(KIM_API_DIR)KIMservice.o $(KIM_API_DIR)KIMserviceC.o  
OBJF90 =$(KIM_API_DIR)KIMserviceF.o

#fortran on/of
ifdef KIM_NO_FORTRAN
ALLOBJ = $(OBJC)
F_FILTER_OUT = %_f/ %_F/ %_f77/ %_F90/ %_f90/
else
ALLOBJ = $(OBJC) $(OBJF90)
F_FILTER_OUT = %_nothinghere2filter/
endif
KIM_LIB = -L$(KIM_API_DIR) -lkim
#set default goals allways all
.DEFAULT_GOAL := all
#set returning directory
RETURN_DIR = $(PWD)

#build target .a or .so for models
ifdef KIM_DYNAMIC
MODEL_BUILD_TARGET += $(patsubst %.a,%.so, $(MODEL_BUILD_TARGET))
SHARED_LIB_FLAG = -shared
ifeq ($(OSTYPE),FreeBSD)
SHARED_LIB_FLAG = -dynamiclib
endif
endif

#reconfigure models
reconfig:
	$(MAKE) -C $(KIM_API_DIR) configmodels; $(MAKE) -C $(KIM_API_DIR) all

.PHONY: reconfig

# Compiler pattern rules
%.o:%.c
	$(CCOMPILER) $(CPPFLAG) $(OBJONLY) $<
%.o:%.cpp
	$(CPPCOMPILER) $(CPPFLAG) $(OBJONLY) $<
%.o:%.f90
	$(FORTRANCOMPILER) $(FORTRANFLAG) $(OBJONLY) $<
%.o:%.F90
	$(FORTRANCOMPILER) $(FORTRANFLAG) $(OBJONLY) $<

%.o:%.F
	$(FORTRANCOMPILER) $(FORTRANFLAG) $(OBJONLY) $<

%.so: %.a
	$(LINKCOMPILER) $(SHARED_LIB_FLAG)  $(CPPLIBFLAG) -o $@  *.o $(ALLOBJ)
