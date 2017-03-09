
SUBDIRS:= Adapter Bridge Composite Decorator Facade Flyweight Proxy

all: $(SUBDIRS)
	@for dir in $(SUBDIRS); do echo `pwd`/$$dir; make -C $$dir || exit"$$?"; done

clean: $(SUBDIRS)
	@for dir in $(SUBDIRS); do make -C $$dir clean; done
