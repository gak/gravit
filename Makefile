nothing:
	@echo "Doing nothing!"

zip:
	@tar czf graviton.tgz *.c *.h Makefile graviton.dsp graviton.dsw graviton.sln graviton.vcproj
