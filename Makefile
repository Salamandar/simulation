PROJECT=Simulation
default: simulation
# Default Options
export ARCH   = PC
export ROBOT ?= gros
export SDL   ?= yes
export DEBUG ?= _ALWAYS_

export PARENT_DIR = ../
include $(PARENT_DIR)/hardware/common.mk

################################################################################
# Fichiers du projet

EXEC=simulation

################################################################################
# Fichiers source



SRC     = $(shell find . -name "*.cpp")
HDR     = $(shell find . -name "*.h")
OBJ     = $(addprefix $(BUILD_DIR)/, $(SRC:%.cpp=%.o) )

#Librairies
CFLAGS += -I. `pkg-config gtkmm-3.0 gtksourceviewmm-3.0 --cflags` -Wno-deprecated
LDFLAGS+=     `pkg-config gtkmm-3.0 gtksourceviewmm-3.0 --libs`

################################################################################
# Cibles du projet

hardware-lib: $(BUILD_DIR)/lib$(ARCH).a

$(BUILD_DIR)/lib$(ARCH).a: $(OBJ) $(OBJ_S)

$(EXEC): $(OBJ) $(HARDW_LIB)
#	@make -C $(ASSER_DIR) asser_robot
	@echo "	++	$(PROJECT)|$(notdir $@)"
	@$(++) $(CFLAGS) -o $@ $^ $(LDFLAGS)


$(HARDW_LIB): hardware_lib

##### Compilation des sources
$(BUILD_DIR)/%.o: %.cpp | $(BUILD_DIR)
	@echo "	++	$(PROJECT)|$(notdir $@)"
	@$(++) $(CFLAGS) -o $@ -c $^

$(BUILD_DIR):
	@mkdir $(BUILD_DIR) $ -p
