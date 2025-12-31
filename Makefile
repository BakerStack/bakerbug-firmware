.DEFAULT_GOAL := help

VENV := .venv
PIP  := $(VENV)/bin/pip
PIO  := $(abspath $(VENV)/bin/pio)


# --------------------------------------------------------------------
help:
	@echo "--------------------------------------------------"
	@echo " Firmware orchestration"
	@echo "--------------------------------------------------"
	@echo " init              - Create venv and install tools"
	@echo " set-dev-env       - Generate .env for DEV"
	@echo " set-prod-env      - Generate .env for PROD"
	@echo " certs             - Build TLS cert C++ files"
	@echo " build             - Compile firmware"
	@echo " upload            - Flash firmware"
	@echo " serial            - Open device serial console"
	@echo " dev               - set-dev-env + certs + upload"
	@echo " prod              - set-prod-env + certs + upload"
	@echo "--------------------------------------------------"

# --------------------------------------------------------------------
# Python toolchain
# --------------------------------------------------------------------
$(VENV):
	python3 -m venv $(VENV)
	$(PIP) install -r tools/requirements.txt

init: $(VENV)

# --------------------------------------------------------------------
# Environment switching
# --------------------------------------------------------------------
set-dev-env:
	envsubst < tools/dev.env > firmware/.env

set-prod-env:
	envsubst < tools/prod.env > firmware/.env

# --------------------------------------------------------------------
# Certificates
# --------------------------------------------------------------------
certs:
	$(MAKE) -C tools/certs

# --------------------------------------------------------------------
# Build (compile only)
# --------------------------------------------------------------------
build: init certs
	cd firmware && \
	bash -c 'set -a && source .env && set +a && \
	$(PIO) run'

upload: init certs
	cd firmware && \
	bash -c 'set -a && source .env && set +a && \
	$(PIO) run -t upload'

serial:
	cd firmware && $(PIO) device monitor

# --------------------------------------------------------------------
dev: set-dev-env upload
prod: set-prod-env upload

# --------------------------------------------------------------------
clean:
	$(MAKE) -C tools/certs clean
	cd firmware && $(PIO) run --target clean

supa-clean: clean
	rm -rf .venv
	rm -f firmware/.env
	