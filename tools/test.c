/* Display module info using libxmp */
/* This file is in public domain */

#include <stdio.h>
#include <stdlib.h>
#include <xmp.h>

int main(int argc, char **argv)
{
	static xmp_context ctx;
	static struct xmp_module_info mi;
	int i;

	ctx = xmp_create_context();

	for (i = 1; i < argc; i++) {
		if (xmp_load_module(ctx, argv[i]) < 0) {
			return -1;
		}

		xmp_release_module(ctx);
	}

	xmp_free_context(ctx);

	return 0;
}
