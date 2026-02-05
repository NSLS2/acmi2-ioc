TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = acmi2
# acmi2ioc.dbd will be created and installed
DBD += acmi2.dbd

# acmi2ioc.dbd will be made up from these files:
acmi2_DBD += base.dbd

# Include dbd files from all support applications:
acmi2_DBD += acmi2Subs.dbd
acmi2_DBD += pscCore.dbd

# Add all the support libraries needed by this IOC
acmi2_LIBS += pscCore 

# acmi2ioc_registerRecordDeviceDriver.cpp derives from acmi2ioc.dbd
acmi2_SRCS += acmi2_registerRecordDeviceDriver.cpp
acmi2_SRCS += ADCsub.c
acmi2_SRCS += TPsub.c
acmi2_SRCS += BMsub.c
acmi2_SRCS += COWsub.c
acmi2_SRCS += TParraySub.c
acmi2_SRCS += BMarraySub.c
acmi2_SRCS += WriteFileSub.c
acmi2_SRCS += LoadFileSub.c
acmi2_SRCS += TPwriteSub.c
acmi2_SRCS += BMwriteSub.c
acmi2_SRCS += PMsub.c
acmi2_SRCS += PMTP.c
acmi2_SRCS += PMBM.c
acmi2_SRCS += PMCOW.c
acmi2_SRCS += PMwrite.c
acmi2_SRCS += COWFiles.c
acmi2_SRCS += TailSub.c

# Build the main IOC entry point on workstation OSs.
acmi2_SRCS_DEFAULT += acmi2Main.cpp
acmi2_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#acmi2ioc_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
acmi2_LIBS += $(EPICS_BASE_IOC_LIBS)
acmi2_SYS_LIBS += fftw3

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

