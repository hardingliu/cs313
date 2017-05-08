	.file	"heapsort.c"
	.text
	.p2align 4,,15
	.globl	heapsort
	.type	heapsort, @function
heapsort:
.LFB11:
	.cfi_startproc
	pushq	%rbx				# save current value of %rbx onto the stack
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movl	%edi, %ebx			# %ebx = %edi = last 
	call	heapify_array			# call heapify_array(last)
	testl	%ebx, %ebx			# is last < 0 ? / test whether %eax is >0, =0 or < 0 	
	js	.L1				# jump to L1 if last < 0 
	.p2align 4,,10
	.p2align 3
.L7:
	movl	%ebx, %edi			# %edi = i = last
	call	extract_max			# call extract_max(i), return to %eax
	movslq	%ebx, %rdx			# %rdx = sign-extedned(%ebx)
	subl	$1, %ebx			# i = i - 1
	cmpl	$-1, %ebx			# compare i:-1		
	movl	%eax, heap(,%rdx,4)		# heap[i]=extract_max[i]
	jne	.L7				# jump to L7 if i is not equal to -1
.L1:
	popq	%rbx				# restore old %rbx from stack
	.cfi_def_cfa_offset 8
	ret					# done
	.cfi_endproc
.LFE11:
	.size	heapsort, .-heapsort
	.ident	"GCC: (SUSE Linux) 4.8.5"
	.section	.note.GNU-stack,"",@progbits
