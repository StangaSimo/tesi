	.text
	.align	2
	.globl	main
	.ent	main
	.type	main, @function
main:
	.frame	r19,20,r15		# vars= 12, regs= 1, args= 0
	.mask	0x00080000
	addik	r1,r1,-20
	swi	r19,r1,16
	addk	r19,r1,r0
	addik	r3,r0,11	# 0xb
	swi	r3,r19,4
	addik	r3,r0,2	# 0x2
	swi	r3,r19,8
	lwi	r4,r19,4
	lwi	r3,r19,8
	addk	r3,r4,r3
	swi	r3,r19,12
	nop
	addk	r1,r19,r0
	lwi	r19,r1,16
	addik	r1,r1,20
	rtsd	r15,8 
	nop		# Unfilled delay slot

	.end	main
$Lfe1:
	.size	main,$Lfe1-main
