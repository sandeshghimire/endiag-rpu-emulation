SUBDIRS =  src

all: subdirs

subdirs:
	for n in $(SUBDIRS); do $(MAKE) -C $$n || exit 1; done

clean:
	for n in $(SUBDIRS); do $(MAKE) -C $$n clean; done

install:
	for n in $(SUBDIRS); do $(MAKE) -C $$n install; done