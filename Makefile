
all:
	(cd cmd && make)
	(cd brain && make)
	(cd plugins/car_control && make && cp -v *.so ..)

clean:
	(cd cmd && make clean)
	(cd brain && make clean)
	(cd plugins/car_control && make clean)
	(cd plugins && rm -vf *.so)

.PHONY: all clean
