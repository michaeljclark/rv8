//
//  test-fpu.c
//
//  DANGER - This is machine generated code
//

#include <stdio.h>
#include <assert.h>

#include "test-fpu.h"

int main()
{
	printf("#include <stdio.h>\n");
	printf("#include <assert.h>\n");
	printf("\n");
	printf("#include \"test-fpu.h\"\n");
	printf("\n");
	printf("int main()\n");
	printf("{\n");
	/* TODO - loop over ISA and fuzz operands using type metadata */
	FPU_IDENTITY(fmadd_s, 1.0f, 2.0f, 3.0f);
	printf("\treturn 0;\n");
	printf("}\n");	return 0;
}
