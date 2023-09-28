	.text
	.align	2
	.globl	main
	.ent	main
	.type	main, @function    

main:
	addi	r3,r0,8       
	addi	r2,r0,-8
	addi	r1,r0,0
	addi 	r1,r1,1
	addi 	r3,r3,-1
	bge r3,r2
	nop
	addi	r3,r0,8
	addi 	r1,r1,1
	addi 	r3,r3,-1
	bgei r3,-8
	nop
	addi	r3,r0,8
	addi 	r1,r1,1
	addi 	r3,r3,-1
	bgeid r3,-8
	addi 	r1,r1,1 #r1=18+9+9+1=37
	nop
	addi	r3,r0,-8
	addi	r2,r0,-8
	addi 	r1,r1,1
	addi 	r3,r3,1
	ble r3,r2  #r1 = 37+9 = 46 

	.end	main
