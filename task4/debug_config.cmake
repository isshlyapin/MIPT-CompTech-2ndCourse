set(MY_DEBUG_FLAGS
    -D_DEBUG
    -ggdb3
    -O1
    -Wall
    -Wextra
    -Wmissing-declarations
    -Wcast-align
    -Wcast-qual
    -Wchar-subscripts
    -Wconversion
    -Wempty-body
    -Wfloat-equal
    -Wformat-security
    -Winline
    -Wpointer-arith
    -Winit-self
    -Wredundant-decls
    -Wshadow
    -Wsign-conversion
    -Wstrict-overflow=2
    -Wswitch-default
    -Wswitch-enum
    -Wundef
    -Wunreachable-code
    -Wunused
    -Wvariadic-macros
    -Wno-missing-field-initializers
    -Wno-narrowing
    -Wno-varargs
    -Wstack-protector
    -fstack-protector
    -fstrict-overflow
    -fno-omit-frame-pointer
    -Wlarger-than=8192
    -fPIE
    -Werror=vla
    -fsanitize=alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,nonnull-attribute,null,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr
)

set(ASAN_FLAG -fsanitize=address)
set(TSAN_FLAG -fsanitize=thread)
set(MSAN_FLAG -fsanitize=memory)
