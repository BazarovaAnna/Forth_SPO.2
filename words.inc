; Стековые комбинаторы
native 'drop', drop
  add rsp, 8
  jmp next

native 'swap', swap
    pop rdx
    pop rax
    push rdx
    push rax
    jmp next

native 'dup', dup
  push qword [rsp]
  jmp next
  
; Арифметические операции
native '+', plus
  pop rdx
  pop rax
  add rax, rdx
  push rax
  jmp next
  
native '*', mul
  pop rdx
  pop rax
  imul rdx
  push rax
  jmp next
  
native '/', div
  pop rax
  pop rcx
  cqo
  idiv rcx
  push rax
  jmp next
  
native '%', mod
  pop rcx
  pop rax
  cqo
  idiv rcx
  push rdx
  jmp next
  
native '-', minus
  pop rdx
  pop rax
  sub rax, rdx
  push rax
  jmp next
  
native '<', less
  pop rdx
  pop rax
  cmp rax, rdx
  setl al
  movzx rax, al
  push rax
  jmp next
  
native '=', equals
  pop rdx
  pop rax
  cmp rax, rdx
  jne .nequals
  xor rax, rax
  mov rax, 1
  push rax
  jmp next
  .nequals:
  xor rax, rax
  push rax
  jmp next
  
; Логические операции
native 'not', not
  pop rax
  test rax, rax
  jz .setone
  xor rax, rax
  push rax
  jmp next
  .setone:
    xor rax, rax
    mov rax, 1
    push rax
    jmp next
    
native 'and', and
  pop rdx
  pop rax
  and rax, rdx
  push rax
  jmp next

native 'or', or
  pop rax
  or [rsp], rax
  jmp next

native 'land', land
  pop rax
  pop rdx
  test rax, rax
  jz .no
  push rdx
  jmp next
  .no:
    push rax
    jmp next
    
native 'lor', lor
  pop rax
  pop rdx
  test rax, rax
  jnz .yes
  push rdx
  jmp next
  .yes:
    push rax
    jmp next
    
; Работа со стеком возвратов
native '>r', to_r
  pop rax
  rpush rax
  jmp next

native 'r>', from_r
  rpop rax
  push rax
  jmp next

native "r@", r_fetch
  push qword [rstack]
  jmp next
  
; Работа с памятью
native '@', fetch
  pop rax
  push qword[rax]
  jmp next
  
native '!', write
  pop rax
  pop rdx
  mov [rax], rdx
  jmp next
  
native 'c!', write_char
  pop rax
  pop rdx
  mov [rax], dl
  jmp next
  
native 'c@', fetch_char
  pop rax
  movzx rax, byte [rax]
  push rax
  jmp next

native 'execute', execute
	pop rax
	mov w, rax
	jmp [rax]
	
; Управление выполнением
native 'docol', docol
   sub rstack, 8
   mov [rstack], pc
   add w, 8
   mov pc, w
   jmp next

native 'branch', branch
   mov pc, [pc]
   jmp next

native 'branch0', branch0
   pop rax
   test rax, rax
   jnz .skip
   mov pc, [pc]
   jmp next

.skip:
   add pc, 8
   jmp next
   
native 'exit', exit
   mov pc, [rstack]
   add rstack, 8
   jmp next 

; Служебные
native 'lit', lit
   push qword [pc]
   add pc, 8
   jmp next
