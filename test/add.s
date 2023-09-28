	.text
	.align	2
	.globl	main
	.ent	main
	.type	main, @function        #r4=155 #r7=2147483647  #carry=1  #r1=157

main:
	addi	r2,r0,55      #r2 = 55
	addi	r3,r0,100     #r3 = 100
	add	r4,r2,r3          #r4 = 155 
	addi	r5,r0,2147483640 #r5 = 2147483640
	addi	r6,r0,8       #r6 = 8
	addk	r7,r5,r6      #r7 = 2147483640 + 8  overflow c =1   
    addic   r1,r4,1       #r1 = 155 + 1 + 1 = 157
    addi   r1,r1,2       #r1 = 157 + 2 = 159
	
	.end	main
