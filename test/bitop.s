	.text
	.align	2
	.globl	main
	.ent	main
	.type	main, @function      

main:
	addi	r2,r0,2999
	addi	r3,r0,23939
    and r1,r2,r3          #2435
	addi	r4,r0,112231
    or r1,r1,r4          #114663
    sra r1,r1            #57331 
    add r4,r1,r0            
    andn r1,r2,r3         #564
    xor r1,r1,r4       #56775
    ori r1,r1,99999 #122847
    andi r1,r1,232 #200
	
	.end	main
