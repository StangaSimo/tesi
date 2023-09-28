	.text
	.align	2
	.globl	main
	.ent	main
	.type	main, @function        #r4=155 #r7=2147483647  #carry=1  #r1=157

main:
	addi	r2,r0,55 
	addi	r3,r0,100 
	add	r4,r2,r3           
	addi	r5,r0,2147483640
	addi	r6,r0,9
	addk	r7,r5,r6              
    addic   r1,r4,1
	
	.end	main
