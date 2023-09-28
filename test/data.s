	.text
	.align	2
	.globl	main
	.ent	main
	.type	main, @function      

main:
	addi	r2,r0,55     
	addi	r3,r0,111      
    sw      r3,r2,r0  
    nop
    nop
    nop
    lw      r4,r2,r0  #r4 = 111
	addi	r7,r0,88     
    swi     r7,r0,99
    nop
    nop
    nop
    add    r3,r0,r0
    lwi     r3,r0,99
    add     r1,r3,r4  #111+88 =  199

	
	.end	main
