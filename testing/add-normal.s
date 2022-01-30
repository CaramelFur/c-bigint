	.file	"add-normal.c"
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	movl	$-44, -4(%rsp)
	movl	$55, -8(%rsp)
	movl	-4(%rsp), %edx
	movl	-8(%rsp), %eax
	addl	%edx, %eax
	movl	%eax, -12(%rsp)
	movl	-12(%rsp), %eax
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (GNU) 11.2.1 20211203 (Red Hat 11.2.1-7)"
	.section	.note.GNU-stack,"",@progbits
