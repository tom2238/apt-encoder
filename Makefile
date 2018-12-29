.PHONY: clean All

All:
	@echo "----------Building project:[ noaa_apt - Debug ]----------"
	@$(MAKE) -f  "noaa_apt.mk"
clean:
	@echo "----------Cleaning project:[ noaa_apt - Debug ]----------"
	@$(MAKE) -f  "noaa_apt.mk" clean
