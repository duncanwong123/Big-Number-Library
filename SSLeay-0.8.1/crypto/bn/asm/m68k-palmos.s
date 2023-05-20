|
| Hand-coded instructions for the routines in bn_mulw.c
|
| Ian Goldberg <ian@cypherpunks.ca>
|
| Last update: 19970822
|
.text
	.even
.globl bn_mul_add_word
bn_mul_add_word:
	movm.l %d3-%d4,-(%sp)
	move.l 12(%sp),%a1
	move.l 16(%sp),%a0
	clr.l %d2
	move.w 20(%sp),%d2
	subq.l #1,%d2
	move.w 22(%sp),%d1
	clr.l %d0
	clr.l %d4
.L10:
	move.w (%a1),%d4
	move.w (%a0),%d3
	add.l %d4,%d0
	mulu.w %d1,%d3
	add.l %d3,%d0
	move.w %d0,(%a1)
	clr.w %d0
	swap %d0
	addq.l #2,%a0
	addq.l #2,%a1
	dbra %d2,.L10
	movm.l (%sp)+,%d3-%d4
	rts
	.even
.globl bn_mul_word
bn_mul_word:
	move.l %d3,-(%sp)
	move.l 8(%sp),%a1
	move.l 12(%sp),%a0
	clr.l %d2
	move.w 16(%sp),%d2
	subq.l #1,%d2
	move.w 18(%sp),%d1
	clr.l %d0
.L20:
	move.w (%a0),%d3
	mulu.w %d1,%d3
	add.l %d3,%d0
	move.w %d0,(%a1)
	clr.w %d0
	swap %d0
	addq.l #2,%a0
	addq.l #2,%a1
	dbra %d2,.L20
	move.l (%sp)+,%d3
	rts
	.even
.globl bn_sqr_words
bn_sqr_words:
	move.l 4(%sp),%a0
	move.l 8(%sp),%a1
	clr.l %d1
	move.w 12(%sp),%d1
	subq.l #1,%d1
.L30:
	move.w (%a1),%d0
	mulu.w %d0,%d0
	move.w %d0,(%a0)
	swap %d0
	move.w %d0,2(%a0)
	addq.l #4,%a0
	addq.l #2,%a1
	dbra %d1,.L30
	rts
	.even
.globl bn_div64
bn_div64:
	move.l 4(%sp),%d0
	| We could try to do just the following, but it breaks if the
	| quotient won't fit in 16 bits.  What is this routine supposed
	| to return in that case? - Ian
	|divu 8(%sp),%d0
	| Instead, we call gcc's 32-bit by 16-bit divide.
	clr.l %d1
	move.w 8(%sp),%d1
	move.l %d1,-(%sp)
	move.l %d0,-(%sp)
	bsr.w __udivsi3
	lea 8(%sp),%sp
	rts
