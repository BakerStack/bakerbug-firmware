.DEFAULT_GOAL := help

VENV := .venv
PIP  := $(VENV)/bin/pip
PIO  := $(abspath $(VENV)/bin/pio)
APPS_DIR := firmware/apps/examples
APPS := $(shell ls $(APPS_DIR) 2>/dev/null)
YELLOW := \033[33m
GREEN  := \033[32m
RED    := \033[31m
BOLD   := \033[1m
RESET  := \033[0m


# --------------------------------------------------------------------
help:
	@echo "--------------------------------------------------"
	@echo " Firmware orchestration"
	@echo "--------------------------------------------------"
	@echo " init              - Create venv and install tools"

	@echo " certs             - Build TLS cert C++ files"
	@echo " upload            - Flash firmware"
	@echo " serial            - Open device serial console"
	@echo " dev               - set-dev-env + certs + upload"
	@echo " prod              - set-prod-env + certs + upload"
	@echo " clean             - Clean build files"
	@echo " supa-clean        - Clean all generated files (expert mode!)"

	@echo "--------- Example Applications -------------------"

	@echo " Applications (use via BB_APPLICATION=...):"
	@for app in $(APPS); do \
		echo "make BB_APPLICATION=applications/examples/$$app build"; \
	done

# --------------------------------------------------------------------
# Check that BB_APPLICATION is set
ifndef BB_APPLICATION
$(error BB_APPLICATION is not set. Please set it to the application path, e.g., applications/examples/mqtt_basic_tls)
endif	
ifndef BB_ARCH
$(error BB_APPLICATION is not set. Please set it to the application path, e.g., applications/examples/mqtt_basic_tls)
endif	
# Check that firmware/.env files exist
ifneq ($(wildcard firmware/$(BB_APPLICATION)/dev.env),firmware/$(BB_APPLICATION)/dev.env)
$(error firmware/$(BB_APPLICATION)/dev.env file not found)
endif

# --------------------------------------------------------------------
# Python toolchain
# --------------------------------------------------------------------
$(VENV):
	python3 -m venv $(VENV)
	$(PIP) install -r tools/requirements.txt

init: $(VENV)

# --------------------------------------------------------------------
# Environment switching
# Secrets are injected via envsubst. They should be defined in
# dev.env and prod.env files located in the application directory.
# All that is set from the serets.sh file that you should customize and
# NOT check into git. This is VERY important for security.
# --------------------------------------------------------------------
_iternal_set-dev-env:
	@echo "\n ------------------------------------------------"
	@echo " $(BOLD)$(GREEN)Environment:$(RESET)$(YELLOW)[firmware/$(BB_APPLICATION)/dev.env]$(RESET)"
	@echo " $(BOLD)$(GREEN)Architecture:$(RESET)$(YELLOW)[$(BB_ARCH)]$(RESET)"
	
	@echo " ------------------------------------------------"
	@cat firmware/$(BB_APPLICATION)/dev.env
	envsubst < firmware/$(BB_APPLICATION)/dev.env > firmware/.env

_iternal_set-prod-env:
	@echo " ------------------------------------------------"
	@echo " $(BOLD)$(GREEN)Environment:$(RESET)$(YELLOW)[firmware/$(BB_APPLICATION)/prod.env]$(RESET)"
	@echo " $(BOLD)$(GREEN)Architecture:$(RESET)$(YELLOW)[$(BB_ARCH)]$(RESET)"

	@echo " ------------------------------------------------"
	@cat firmware/$(BB_APPLICATION)/prod.env
	envsubst < firmware/$(BB_APPLICATION)/prod.env > firmware/.env

# --------------------------------------------------------------------
# Certificates
# --------------------------------------------------------------------
certs:
	$(MAKE) -C tools/certs all

# --------------------------------------------------------------------
# Build (compile only)
# --------------------------------------------------------------------
_iternal_build: init certs
	@echo " ------------------------------------------------"
	@echo "  $(BOLD)$(GREEN)Building application: $(RESET)$(YELLOW)$(BB_APPLICATION)$(RESET)"
	@echo " ------------------------------------------------"
	@echo ""
	cd firmware && \
	bash -c 'set -a && source .env && set +a && \
	$(PIO) run -e $(BB_ARCH)'

upload: init certs
	cd firmware && \
	bash -c 'set -a && source .env && set +a && \
	$(PIO) run -e $(BB_ARCH) -t upload'

serial:
	cd firmware && $(PIO)  -e $(BB_ARCH) device monitor

# --------------------------------------------------------------------
dev: _iternal_set-dev-env _iternal_build
prod: _iternal_set-prod-env _iternal_build

# --------------------------------------------------------------------
clean:
	$(MAKE) -C tools/certs clean
	cd firmware && $(PIO) run  -e $(BB_ARCH) --target clean

supa-clean: clean
	rm -rf .venv
	rm -f firmware/.env
	