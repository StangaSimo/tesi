	.text
	.align	2
	.globl	main
	.ent	main
	.type	main, @function        #r4=155 #r7=2147483647  #carry=1  #r1=157

main:
	addi	r2,r0,-55      #r2 = -55
	addi	r3,r0,885      #r3 = 885
	rsub	r4,r3,r2    #r4 = r2-r3 = -55-885 = -950
	rsubi	r1,r4,88          #r1 = 88 - (- 850) = 1038
	addi    r7,r0,-2147483647
	addi    r8,r0,4
	rsubc    r2,r8,r7    #r2= r7-r8 = -2147483647 - 4 = underflow = -2147483648
	add 	r1,r1,r2     #r1=r2+r1 = -2147482620
	
	.end	main
