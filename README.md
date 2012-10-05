#composite_vm


The implementation of virtual machine experimental architecture «Compozit». 
Refer for more information about architecture: http://skylark.tsu.ru/compozit/index-en.html.



##Compiling:
You need to install boost (header only), cmake, doxygen(if you want to assemble documentation).

	cd trunk/src/ 

	cmake ./ 

	make 


Use `make test` (in trunk/src/)for testing


##Generate documentation(used doxygen):
	cd trunc/docs

	doxygen doxy_config
