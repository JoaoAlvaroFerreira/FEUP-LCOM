#include <machine/asm.h>
#include "i8042.h"

.global _C_LABEL(kbc_asm_ih)

.global _C_LABEL(codeScanned)
.global _C_LABEL(asmFailure)

.text
_C_LABEL (kbc_asm_ih):
       xor %eax, %eax
       xor %ebx, %ebx
       inb $STAT_REG
       test $OBF, %eax
       jz  ERR
       mov $PAR_ERR, %ebx
       or   $TO_ERR, %ebx
       test %ebx, %eax
       jnz ERR
       inb $OUT_BUF
       movl %eax, _C_LABEL(codeScanned)
       jmp END
ERR:
       movl $-1, _C_LABEL(asmFailure)
END:
       ret
