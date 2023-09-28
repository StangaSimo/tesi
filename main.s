	.text
	.align	2
	.globl	main
	.ent	main
	.type	main, @function
main:
	.frame	r19,12,r15		# vars= 4, regs= 1, args= 0
	.mask	0x00080000
	addik	r1,r1,-12
	swi	r19,r1,8
	addk	r19,r1,r0
	swi	r0,r19,4
	lwi	r3,r19,4
	addik	r3,r3,1
	swi	r3,r19,4
	nop
	addk	r1,r19,r0
	lwi	r19,r1,8
	addik	r1,r1,12
	rtsd	r15,8 
	nop		# Unfilled delay slot

	.end	main
$Lfe1:
	.size	main,$Lfe1-main
