###############################################################################
################### MOOSE Application Standard Makefile #######################
###############################################################################
#
# Optional Environment variables
# MOOSE_DIR        - Root directory of the MOOSE project
#
###############################################################################
# Use the MOOSE submodule if it exists and MOOSE_DIR is not set
MOOSE_SUBMODULE    := $(CURDIR)/moose
ifneq ($(wildcard $(MOOSE_SUBMODULE)/framework/Makefile),)
  MOOSE_DIR        ?= $(MOOSE_SUBMODULE)
else
  MOOSE_DIR        ?= $(shell dirname `pwd`)/moose
endif

# framework
FRAMEWORK_DIR      := $(MOOSE_DIR)/framework
include $(FRAMEWORK_DIR)/build.mk
include $(FRAMEWORK_DIR)/moose.mk

# Make sure no warnings are permitted
ADDITIONAL_CPPFLAGS += -Wall -Wextra -Werror

################################## MODULES ####################################
# To use certain physics included with MOOSE, set variables below to
# yes as needed.  Or set ALL_MODULES to yes to turn on everything (overrides
# other set variables).

ALL_MODULES         := no

CHEMICAL_REACTIONS  := no
CONTACT             := no
FLUID_PROPERTIES    := no
HEAT_TRANSFER       := yes
MISC                := no
NAVIER_STOKES       := no
PHASE_FIELD         := yes
RDG                 := no
RICHARDS            := no
STOCHASTIC_TOOLS    := no
SOLID_MECHANICS     := yes
XFEM                := no
POROUS_FLOW         := no

include $(MOOSE_DIR)/modules/modules.mk
###############################################################################

# dep apps
APPLICATION_DIR    := $(CURDIR)
APPLICATION_NAME   := raccoon
BUILD_EXEC         := yes
GEN_REVISION       := yes
INSTALLABLE_DIRS   := test/tests->tests tutorials
include            $(FRAMEWORK_DIR)/app.mk

###############################################################################
# Additional special case targets should be added here
