	.text
	.align	2
	.globl	main
	.ent	main
	.type	main, @function
	
main:
	add	r1,r5,r2 
	addk	r1,r1,r2
	rsub	r1,r1,r2
	addi	r1,r1,112
	addik	r1,r1,-20
	rsubik	r1,r1,20
	sra r1,r2
	or r1,r2,r3
	and r1,r2,r3
	xor r1,r2,r3
	andn r1,r2,r3
	ori r1,r2,100
	andi r1,r2,100
	xori r1,r2,100
	andni r1,r2,100
	brid	199
	brlid	r1,19
	rtsd r0,199
	rtid r0,199
	beqi r1,290
	bnei r2,499
	blti r4,438
	blei r1,299
	bgti r2,299
	bgei r2,299
	lw r1,r3,r4
	sw r1,r2,r3	
	lwi r1,r3,232
	swi r1,r3,378

	.end	main
