	.text
	.align	2
	.globl	main
	.ent	main
	.type	main, @function
main:
	addi	r1,r0,0
	addi	r2,r0,-20

$L2:
	addi    r1,r1,10
	addi    r2,r2,1
	beqi    r2,$L2	

	.end	main
