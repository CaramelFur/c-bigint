	.file	"add-indirect.c"
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	movl	$-44, -24(%rsp)
	movl	$55, -28(%rsp)
	leaq	-24(%rsp), %rax
	movq	%rax, -8(%rsp)
	leaq	-28(%rsp), %rax
	movq	%rax, -16(%rsp)
	movq	-8(%rsp), %rax
	movl	(%rax), %edx
	movq	-16(%rsp), %rax
	movl	(%rax), %eax
	addl	%edx, %eax
	movl	%eax, -20(%rsp)
	movl	-20(%rsp), %eax
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (GNU) 11.2.1 20211203 (Red Hat 11.2.1-7)"
	.section	.note.GNU-stack,"",@progbits
