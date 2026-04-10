all:
	@(cd src && $(MAKE))

clean:
	@(cd src && $(MAKE) clean)
	@(cd doc && $(MAKE) clean)
	rm -rf include/*~ *~
