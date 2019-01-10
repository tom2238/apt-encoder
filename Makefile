.PHONY: clean All

All:
	@echo "----------Sestavování projektu:[ noaa_apt - Debug ]----------"
	@$(MAKE) -f  "noaa_apt.mk"
clean:
	@echo "----------Čištění projektu:[ noaa_apt - Debug ]----------"
	@$(MAKE) -f  "noaa_apt.mk" clean
