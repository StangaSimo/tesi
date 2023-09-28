	.text
	.align	2
	.globl	main
	.ent	main
	.type	main, @function    

main:
    nop
	bri 28 #line 8
    nop
    nop
    addi r1,r1,20
    addi r2,r0,44
    bra r2 #line 12
    nop
    nop
    addi r1,r0,30
    brai 8 #line 3
    nop 
    addi r1,r1,10

	.end	main
