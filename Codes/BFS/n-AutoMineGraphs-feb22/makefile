#OPTIONS=-Wno-deprecated -g
#LIBS=/usr/lib/liblapack++.a /usr/lib/liblamatrix++.a -lm /usr/lib/liblapack.a /usr/lib/libblas.a -lgfortran
#LIBS=/usr/lib/x86_64-linux-gnu/lapack/liblapack++.a /usr/lib/liblamatrix++.a -lm /lib/x86_64-linux-gnu/lapack/liblapack.a /usr/lib/libblas.a -lgfortran
#LIBS=/usr/lib/liblapack++.a /usr/lib/liblamatrix++.a -lm /lib/x86_64-linux-gnu/lapack/liblapack.a /usr/lib/x86_64-linux-gnu/blas/libblas.a -lgfortran

./bin/n-graph-miner : ./obj/utility.o obj/consDes.o obj/readfrmfile.o obj/countingNew.o obj/candidateGenNew.o obj/filterParserNew.o obj/mainfile.o
	g++ -o ./bin/n-graph-miner ./obj/utility.o obj/consDes.o obj/readfrmfile.o obj/countingNew.o obj/candidateGenNew.o obj/filterParserNew.o obj/mainfile.o
obj/utility.o : src/utility.cpp src/super.h
	g++ $(OPTIONS) -c src/utility.cpp -o obj/utility.o
obj/consDes.o : src/consDes.cpp src/super.h
	g++ $(OPTIONS) -c src/consDes.cpp -o obj/consDes.o
obj/readfrmfile.o : src/readfrmfile.cpp src/super.h
	g++ $(OPTIONS) -c src/readfrmfile.cpp -o obj/readfrmfile.o
obj/countingNew.o : src/countingNew.cpp src/super.h
	g++ $(OPTIONS) -c src/countingNew.cpp -o obj/countingNew.o
obj/candidateGenNew.o : src/candidateGenNew.cpp src/super.h
	g++ $(OPTIONS) -c src/candidateGenNew.cpp -o obj/candidateGenNew.o
obj/filterParserNew.o : src/filterParserNew.cpp src/super.h
	g++ $(OPTIONS) -c src/filterParserNew.cpp -o obj/filterParserNew.o
obj/mainfile.o : src/mainfile.cpp src/super.h
	g++ $(OPTIONS) -c src/mainfile.cpp -o obj/mainfile.o
clean:
	rm ./obj/*.o
