.PHONY: all clean run
.SUFFIXES: .c 

mbgcc=/tools/Xilinx/Vivado/2023.1/gnu/microblaze/lin/bin/mb-gcc
mbobj=/tools/Xilinx/Vivado/2023.1/gnu/microblaze/lin/bin/mb-objdump

main: src/main.c 
	@$(CC) src/main.c src/util.c src/cpu.c -o bin/main 
	@./bin/main prova.text

prova1: 
	@$(mbgcc) -o prova.o -c prova.s 
	@objcopy -j .text -O binary -I elf32-little prova.o prova.text  
	@$(mbobj) -dS prova.o > prova.txt  
	@make	

prova2: 
	@$(CC) main.c 
	@./a.out 

test1: 
	@$(mbgcc) -o test/add.o -c test/add.s 
	@objcopy -j .text -O binary -I elf32-little test/add.o test/add.text 
	@$(mbobj) -dS test/add.o > test/add.txt

	@$(mbgcc) -o test/sub.o -c test/sub.s 
	@objcopy -j .text -O binary -I elf32-little test/sub.o test/sub.text 
	@$(mbobj) -dS test/sub.o > test/sub.txt

	@$(mbgcc) -o test/bitop.o -c test/bitop.s 
	@objcopy -j .text -O binary -I elf32-little test/bitop.o test/bitop.text 
	@$(mbobj) -dS test/bitop.o > test/bitop.txt

	@$(mbgcc) -o test/branch.o -c test/branch.s 
	@objcopy -j .text -O binary -I elf32-little test/branch.o test/branch.text 
	@$(mbobj) -dS test/branch.o > test/branch.txt

	@$(mbgcc) -o test/absolute_branch.o -c test/absolute_branch.s 
	@objcopy -j .text -O binary -I elf32-little test/absolute_branch.o test/absolute_branch.text 
	@$(mbobj) -dS test/absolute_branch.o > test/absolute_branch.txt

	@$(mbgcc) -o test/cmp.o -c test/cmp.s 
	@objcopy -j .text -O binary -I elf32-little test/cmp.o test/cmp.text 
	@$(mbobj) -dS test/cmp.o > test/cmp.txt

	@$(mbgcc) -o test/data.o -c test/data.s 
	@objcopy -j .text -O binary -I elf32-little test/data.o test/data.text 
	@$(mbobj) -dS test/data.o > test/data.txt

	$(CC) src/util.c src/cpu.c test/test.c
	./a.out

scp: 
	scp -r . stanganini@pianosa.di.unipi.it:tesi
	
clean: 
	-@rm bin/main
