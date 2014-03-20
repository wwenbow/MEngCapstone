#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
include Makefile

# Environment
SHELL=cmd.exe
# Adding MPLAB X bin directory to path
PATH:=C:/Program Files/Microchip/MPLABX/mplab_ide/mplab_ide/modules/../../bin/:$(PATH)
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=MX460
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/HelloWorld.X.${IMAGE_TYPE}.elf
else
IMAGE_TYPE=production
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/HelloWorld.X.${IMAGE_TYPE}.elf
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/main.o

# Object Files
OBJECTFILES=${OBJECTDIR}/main.o


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

# Path to java used to run MPLAB X when this makefile was created
MP_JAVA_PATH="C:\Program Files\Java\jre6/bin/"
OS_CURRENT="$(shell uname -s)"
############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
MP_CC="C:\Program Files\Microchip\mplabc32\v2.01\bin\pic32-gcc.exe"
# MP_BC is not defined
MP_AS="C:\Program Files\Microchip\mplabc32\v2.01\bin\pic32-as.exe"
MP_LD="C:\Program Files\Microchip\mplabc32\v2.01\bin\pic32-ld.exe"
MP_AR="C:\Program Files\Microchip\mplabc32\v2.01\bin\pic32-ar.exe"
DEP_GEN=${MP_JAVA_PATH}java -jar "C:/Program Files/Microchip/MPLABX/mplab_ide/mplab_ide/modules/../../bin/extractobjectdependencies.jar" 
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps
MP_CC_DIR="C:\Program Files\Microchip\mplabc32\v2.01\bin"
# MP_BC_DIR is not defined
MP_AS_DIR="C:\Program Files\Microchip\mplabc32\v2.01\bin"
MP_LD_DIR="C:\Program Files\Microchip\mplabc32\v2.01\bin"
MP_AR_DIR="C:\Program Files\Microchip\mplabc32\v2.01\bin"
# MP_BC_DIR is not defined

.build-conf:  ${BUILD_SUBPROJECTS}
	${MAKE}  -f nbproject/Makefile-MX460.mk dist/${CND_CONF}/${IMAGE_TYPE}/HelloWorld.X.${IMAGE_TYPE}.elf

MP_PROCESSOR_OPTION=32MX460F512L
MP_LINKER_FILE_OPTION=
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/main.o: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${FIXDEPS} "${OBJECTDIR}/main.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"." -I"../Microchip/Include" -I"." -MMD -MF "${OBJECTDIR}/main.o.d" -o ${OBJECTDIR}/main.o main.c  
	
else
${OBJECTDIR}/main.o: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${FIXDEPS} "${OBJECTDIR}/main.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"." -I"../Microchip/Include" -I"." -MMD -MF "${OBJECTDIR}/main.o.d" -o ${OBJECTDIR}/main.o main.c  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/HelloWorld.X.${IMAGE_TYPE}.elf: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)    -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/HelloWorld.X.${IMAGE_TYPE}.elf ${OBJECTFILES_QUOTED_IF_SPACED}        -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__ICD2RAM=1,--defsym=__DEBUG=1,-L".",-Map="${DISTDIR}/HelloWorld.X.${IMAGE_TYPE}.map" 
else
dist/${CND_CONF}/${IMAGE_TYPE}/HelloWorld.X.${IMAGE_TYPE}.elf: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/HelloWorld.X.${IMAGE_TYPE}.elf ${OBJECTFILES_QUOTED_IF_SPACED}        -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),-L".",-Map="${DISTDIR}/HelloWorld.X.${IMAGE_TYPE}.map"
	${MP_CC_DIR}\\pic32-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/HelloWorld.X.${IMAGE_TYPE}.elf  
endif


# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf:
	${RM} -r build/MX460
	${RM} -r dist/MX460

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(wildcard $(addsuffix .d, ${OBJECTFILES}))
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
