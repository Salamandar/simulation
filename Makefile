PROJECT=Simulation
default: all
# Default Options
export ARCH   = PC
export DEBUG ?= _ALWAYS_

PARENT_DIR = ../

# Constantes de compilation
EXEC=simulation

SIMU_WITH_ASSER = 1
SIMU_WITH_CARTO = 1

include $(PARENT_DIR)/hardware/common.rules
################################################################################
# Fichiers du projet
SRC     = main.cpp		\
		communication_view.cpp	\
		plateau.cpp		\
		simulation.cpp	\
		interfaces/communication.cpp	\
		$(shell find libserial -name "*.cpp")

SRC_ASSER=interfaces/asservissement.cpp
SRC_CARTO=interfaces/cartographie.cpp
################################################################################
# Fichiers source

ifeq ($(SIMU_WITH_ASSER), 1)
	CFLAGS+=-DSIMU_WITH_ASSER
	SRC+=$(SRC_ASSER)
	LINK_LIBS +=  -lAsser
	DEPS      += libAsser
endif
ifeq ($(SIMU_WITH_CARTO), 1)
	CFLAGS+=-DSIMU_WITH_CARTO
	SRC+=$(SRC_CARTO)
	LINK_LIBS +=  -lCarto
	DEPS      += libCarto
endif


#SRC     = $(shell find . -name "*.cpp")
HDR     = $(shell find . -name "*.h")
OBJ     = $(addprefix $(BUILD_DIR)/, $(SRC:%.cpp=%.o) )

#Librairies
CFLAGS += -I. `pkg-config gtkmm-3.0 gtksourceviewmm-3.0 --cflags` \
	-Wno-deprecated -Wno-ignored-qualifiers
LDFLAGS+=     `pkg-config gtkmm-3.0 gtksourceviewmm-3.0 --libs`

################################################################################
# Cibles du projet
all: $(BUILD_DIR)/$(EXEC)

$(BUILD_DIR)/$(EXEC): $(OBJ) $(DEPS) libComm libHardware
	@echo "	++	$(PROJECT)|$(notdir $@)"
	$(++) $(CFLAGS) -o $@ $(OBJ) $(LINK_LIBS) -lComm -lHardware $(LDFLAGS)



##### Compilation des sources
$(BUILD_DIR)/%.o: %.cpp | $(BUILD_DIR)
	@echo "	++	$(PROJECT)|$(notdir $@)"
	@$(++) $(CFLAGS) -o $@ -c $^


$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)/interfaces/
	@mkdir -p $(BUILD_DIR)/libserial/
