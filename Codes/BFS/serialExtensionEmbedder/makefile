./bin/serialExtEmbedder : ./obj/mtrand.o ./obj/generatorInjective.o obj/utility.o obj/consDes.o
	g++ -o bin/serialExtEmbedder obj/mtrand.o ./obj/generatorInjective.o obj/utility.o obj/consDes.o
./obj/mtrand.o : src/mtrand.cpp src/mtrand.h src/super.h
	g++ -c  src/mtrand.cpp -o obj/mtrand.o
./obj/generatorInjective.o : src/generatorInjective.cpp  
	g++ -c src/generatorInjective.cpp -o ./obj/generatorInjective.o
./obj/utility.o : src/utility.cpp src/mtrand.h src/super.h
	g++ -c src/utility.cpp -o ./obj/utility.o
./obj/consDes.o : src/consDes.cpp src/mtrand.h src/super.h 
	g++ -c src/consDes.cpp -o ./obj/consDes.o
clean:
	rm ./obj/*.o
