	.text
	.align	2
	.globl	main
	.ent	main
	.type	main, @function      

main:
	addi	r2,r0,20      
	addi	r3,r0,40      
    cmp     r4,r2,r3      
    addi    r5,r0,-146800650
    cmpu    r1,r4,r5         # 4148166646 -20 = 4148166626 signed: -146800670

	.end	main
