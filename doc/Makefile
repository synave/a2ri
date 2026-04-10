all:
	@(cd src && $(MAKE))
	mv src/*.pdf pdf/

clean:
	@(cd src && $(MAKE) clean)
	rm -rf *~ pdf/*.pdf
