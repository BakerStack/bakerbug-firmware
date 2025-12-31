.DEFAULT_GOAL := help

help:
	@echo "--------------------------------------------"
	@echo " Tools"
	@echo "--------------------------------------------"
	@echo " certs     - Build TLS cert C++ headers"
	@echo " clean     - Clean all generated artifacts"
	@echo "--------------------------------------------"

certs:
	$(MAKE) -C certs

clean:
	$(MAKE) -C certs clean

.PHONY: help certs clean

