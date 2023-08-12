.PHONY: all clean run
.SUFFIXES: .c 

main: src/main.c 
	@$(CC) src/main.c src/util.c src/cpu.c -o bin/main 
	@./bin/main proba.text

test: 
	@/tools/Xilinx/Vivado/2023.1/gnu/microblaze/lin/bin/mb-gcc -o prova.o -c prova.s 
	@objcopy -j .text -O binary -I elf32-little prova.o proba.text  
	@/tools/Xilinx/Vivado/2023.1/gnu/microblaze/lin/bin/mb-objdump -dS prova.o > prova.txt  
	@make	

clean: 
	-@rm bin/main
