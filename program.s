# Sample program
# To comment a line, use `#` followed by a space.

li r1 0
li r2 0
li r3 10

sub r4 r2 r3
beqz r4 4
add r1 r1 r2
inc r2
jmp -4

st r1 r0 0
hlt
