    set rA, 0
    set rB, 1

!test_one
    cmp rA, 0
    JMP !success_one
    set rB, 0

!test_two
    cmp rA, -1
    JMP !success_two
    set rB, 0

!test_three
    cmp rA, 1
    JMP !success_three
    set rB, 0

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