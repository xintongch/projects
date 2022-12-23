

all: central.cpp serverT.cpp serverS.cpp serverP.cpp clientA.cpp clientB.cpp graph.cpp graph.h scores.cpp scores.h calculator.cpp calculator.h
	g++ -c graph.cpp 
	g++ -c scores.cpp 
	g++ -c calculator.cpp 
	g++ -o serverC central.cpp 
	g++ -o serverT serverT.cpp graph.o 
	g++ -o serverS serverS.cpp scores.o 
	g++ -o serverP serverP.cpp calculator.o 
	g++ -o clientA clientA.cpp 
	g++ -o clientB clientB.cpp 
	


serverC: serverC
	./serverC

serverT: serverT
	./serverT 

serverS: serverS
	./serverS

serverP: serverP
	./serverP



.PHONY: serverC serverT serverS serverP

