PROJECT=Simulation
default: all
# Default Options
export ARCH   = PC
export ROBOT ?= gros
export SDL   ?= yes
export DEBUG ?= _ALWAYS_

PARENT_DIR = ../

# Constantes de compilation
EXEC=simulation

include $(PARENT_DIR)/hardware/common.rules
################################################################################
# Fichiers du projet


################################################################################
# Fichiers source

SRC     = $(shell find . -name "*.cpp")
HDR     = $(shell find . -name "*.h")
OBJ     = $(addprefix $(BUILD_DIR)/, $(SRC:%.cpp=%.o) )

#Librairies
CFLAGS += -I. `pkg-config gtkmm-3.0 gtksourceviewmm-3.0 --cflags` \
	-Wno-deprecated -Wno-ignored-qualifiers
LDFLAGS+=     `pkg-config gtkmm-3.0 gtksourceviewmm-3.0 --libs`

################################################################################
# Cibles du projet
all: $(BUILD_DIR)/$(EXEC)

$(BUILD_DIR)/$(EXEC): $(OBJ) libAsser libHardware libCommAsser libCarto
	@echo "	++	$(PROJECT)|$(notdir $@)"
	@$(++) $(CFLAGS) -o $@ $(OBJ) -lAsser -lCommAsser -lCarto -lHardware $(LDFLAGS)



##### Compilation des sources
$(BUILD_DIR)/%.o: %.cpp | $(BUILD_DIR)
	@echo "	++	$(PROJECT)|$(notdir $@)"
	@$(++) $(CFLAGS) -o $@ -c $^


$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)/interfaces/
	@mkdir -p $(BUILD_DIR)/libserial/
