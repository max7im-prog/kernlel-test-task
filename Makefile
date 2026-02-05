all:
	$(MAKE) -C module
	$(MAKE) -C params

clean:
	$(MAKE) -C module clean
	$(MAKE) -C params clean

load:
	$(MAKE) -C module load

unload:
	$(MAKE) -C module unload

.PHONY: all clean load unload
