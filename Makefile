nothing:
	@echo "This makefile doesn't do much! Try using one of the other makefiles instead."

zip:
	@tar czf graviton.tgz *.c *.h Makefile graviton.dsp graviton.dsw 

clean:
	@rm -fr *.o Debug Release
	
