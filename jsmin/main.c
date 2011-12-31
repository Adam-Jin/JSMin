/* Copyright 2002 Douglas Crockford <http://www.crockford.com>
 * Copyright 2011 Michael Steinert
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * The Software shall be used for Good, not Evil.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#include "jsmin/jsmin.h"
#include <stdlib.h>
#include <stdio.h>

/**
 * \brief Output command line arguments as comments and minify the input.
 *
 * \param [in] argc The argument count
 * \param [in] argv The argument vector
 *
 * \return The program returns EXIT_SUCCESS or EXIT_FAILURE.
 */
int
main(int argc, char **argv)
{
	int i;
	Jsmin *jsmin;
	for (i = 1; i < argc; ++i) {
		fprintf(stdout, "// %s\n", argv[i]);
	}
	jsmin = jsmin_create();
	if (!jsmin) {
		return EXIT_FAILURE;
	}
	jsmin_minify(jsmin);
	jsmin_destroy(jsmin);
	return EXIT_SUCCESS;
}
