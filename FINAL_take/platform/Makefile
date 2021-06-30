ifndef IMPERAS_HOME
  IMPERAS_ERROR := $(error "IMPERAS_HOME not defined, please setup Imperas/OVP environment")
endif
IMPERAS_HOME := $(shell getpath.exe "$(IMPERAS_HOME)")
# Build locally without using a VLNV librry
NOVLNV=1

OPTLM=1
SRC=platform.cpp DMA.cpp Adaptor.cpp

include ${IMPERAS_HOME}/ImperasLib/buildutils/Makefile.TLM.platform
