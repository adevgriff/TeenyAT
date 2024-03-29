    set rA, 0
    set rB, 1

!test_one
    cmp rA, 0
    JE !success_one
    JMP !fail

!test_two
    cmp rA, -1
    JE !fail
    JMP !success_two

!test_three
    cmp rA, 1
    JE !fail
    JMP !success_three

!success_one
    JMP !test_two

!success_two
    JMP !test_three

!success_three
    JMP !end

!fail
    set rB, 0

!end
    set rA, 1