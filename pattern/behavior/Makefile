
SUBDIRS:= ChainOfResponsibility Command Mediator Observer State Strategy TemplateMethod Vistor

all: $(SUBDIRS)
	@for dir in $(SUBDIRS); do echo `pwd`/$$dir; make -C $$dir || exit"$$?"; done

clean: $(SUBDIRS)
	@for dir in $(SUBDIRS); do make -C $$dir clean; done
