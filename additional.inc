native 'buf', buf
  push qword user_buf
  jmp next
  
native '.', dot
  pop rdi
  call print_int
  jmp next

native '.S', show_stack
  mov rcx, rsp
  .loop:
    cmp rcx, [stack_start]
    jae next
    mov rdi, [rcx]
    push rcx
    call print_int
    call print_newline
    pop rcx
    add rcx, 8
    jmp .loop
  
native 'pushmode_addr', pushmode_addr
  xor eax, eax
  mov rax, mode
  push rax
  jmp next

native 'isimmediate', isimmediate
  pop rax
  push rax
  xor rdx, rdx
  mov dl, byte[rax - 1]
  push rdx
  jmp next

native 'initcmd', initcmd
  pop rax
  push pc
  mov [program_stub], rax
  mov pc, program_stub
  mov w, [pc]
  pop pc
  jmp [w]

native 'rot', rot
  pop rax
  pop rdx
  pop rcx
  push rdx
  push rcx
  push rax
  jmp next
  
native 'isbranch', isbranch
  pop rax
  push rax
  cmp byte[rax - 1], 2
  jne .continue
  mov byte[was_branch], 1
  .continue:
  jmp next

native 'pushlit', pushlit
  push xt_lit
  jmp next

native 'unsetbranch', unsetbranch
  mov byte[was_branch], 0
  jmp next

native 'saveword', saveword
  pop rax
  mov  qword[here], rax
  xor eax, eax
  mov rax, here
  add rax, word_size
  add qword[here], rax
  jmp next

native 'savenum', savenum
  pop rax
  mov [here], rax
  xor eax, eax
  mov rax, here
  add rax, word_size
  mov qword[here], rax
  jmp next

native 'wasbranch', wasbranch
  xor rdx, rdx
  mov dl, byte[was_branch]
  push rdx
  jmp next

native 'pushmode', pushmode
  xor eax, eax
  mov rax, [mode]
  push rax
  jmp next

native 'buffer', buffer
  push qword input_buf
  jmp next

native 'comp_m', comp_m
  mov  rdi, cmode_message
  call print_string
  call print_newline
  jmp next

native 'inte_m', inte_m
  mov  rdi, imode_message
  call print_string
  call print_newline
  jmp next

native ',', comma
  mov rax, [here]
  pop qword [rax]
  add qword [here], 8
  jmp next

native 'create', create
  ; генерация ссылки
  mov rcx, [last_word]
  mov rsi, [here]
  mov [rsi], rcx
  mov [last_word], rsi
  add rsi, 8
  ;mov byte [rsi], 0
  ;inc rsi

  ; генерация имени
  pop rdi
  push rsi
  call string_copy
  pop rsi
  push rsi
  mov rdi, rsi
  call string_length
  pop rsi
  add rsi, rax
  ; флаги
  inc rsi
  pop rax
  mov [rsi], al
  inc rsi

  mov [here], rsi
  jmp next


native "syscall", syscall
  pop r9
  pop r8
  pop r10
  pop rdx
  pop rsi
  pop rdi
  pop rax
  syscall
  push rax
  push rdx
  jmp next

native 'warn', warn
  mov  rdi, warning_message
  call print_string
  call print_newline
  mov  pc, xt_loop
  jmp next

native 'bye', bye
  mov rax, 60
  xor rdi, rdi
  syscall

native 'read', read
  pop rdi
  call read_word
  push rdx
  jmp next

native 'prints', prints
  pop rdi
  call print_string
  jmp next

native 'printnl', printnl
  call print_newline
  jmp next

native 'parsei', parsei
  call parse_int
  push rax
  push rdx
  jmp next

native 'compare', compare
  pop rdi
  pop rsi
  call string_equals
  push rax
  jmp next

native 'key', key
  call read_char
  push rax
  jmp next

native 'emit', emit
  pop rdi
  call print_char
  jmp next

native 'save_lw', save_lw
  push last_word
  jmp next

;-------------------------

native 'cfa', cfa
  pop rdi
	call cfa
  push rax
  jmp next

native 'find', find
  pop rdi
  call find_word
  push rax
  jmp next
