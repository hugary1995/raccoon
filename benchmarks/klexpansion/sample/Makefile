######################################################################
# Use the MOOSE submodule if it exists and MOOSE_DIR is not set
MOOSE_SUBMODULE    := $(abspath $(dir $(lastword $(MAKEFILE_LIST)))/../../../moose)
ifneq ($(wildcard $(MOOSE_SUBMODULE)/framework/Makefile),)
  MOOSE_DIR        ?= $(MOOSE_SUBMODULE)
else
  MOOSE_DIR        ?= $(abspath $(dir $(lastword $(MAKEFILE_LIST)))/../../../../moose)
endif

######################################################################
#
# Set LIBMESH_DIR if it is not already set in the environment (try our best to guess!)
LIBMESH_DIR       ?= $(MOOSE_DIR)/libmesh/installed


# include the library options determined by configure
include $(LIBMESH_DIR)/Make.common

target     := ./sample-$(METHOD)

libmesh_CXXFLAGS += -I$(BOOST_DIR)
###############################################################################
# File management.  This is where the source, header, and object files are
# defined

#
# source files
srcfiles 	:= $(wildcard *.C)

#
# object files
objects		:= $(patsubst %.C, %.$(obj-suffix), $(srcfiles))
###############################################################################



.PHONY: dust clean distclean

###############################################################################
# Target:
#

all:: $(notdir $(target))

# Production rules:  how to make the target - depends on library configuration
$(notdir $(target)): $(objects)
	@echo $(LIBMESH_DIR)
	@echo "Linking "$@"..."
	@$(libmesh_LIBTOOL) --tag=CXX $(LIBTOOLFLAGS) --mode=link \
	  $(libmesh_CXX) $(libmesh_CXXFLAGS) $(objects) -o $@ $(libmesh_LIBS) $(libmesh_LDFLAGS) $(EXTERNAL_FLAGS)


# Useful rules.
dust:
	@echo "Deleting old output and runtime files"
	@rm -f out*.m job_output.txt output.txt* *.gmv.* *.plt.* out*.xdr* out*.xda* PI* complete

clean: dust
	@rm -f $(objects) *.$(obj-suffix) *.lo

clobber: clean
	@rm -f $(target)

distclean: clean
	@rm -rf *.o .libs

###############################################################################
