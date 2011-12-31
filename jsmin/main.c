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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <getopt.h>
#include "jsmin/jsmin.h"
#include <stdlib.h>
#include <stdio.h>

/**
 * \brief Minify input and output command line arguments as comments.
 *
 * \param [in] argc The argument count
 * \param [in] argv The argument vector
 *
 * \return The program returns EXIT_SUCCESS or EXIT_FAILURE.
 */
int
main(int argc, char **argv)
{
	Jsmin *jsmin = NULL;
	int i, status = EXIT_SUCCESS;
	JsminStream *in = NULL, *out = NULL;
	in = jsmin_file_stream_create(stdin);
	if (!in) {
		goto error;
	}
	out = jsmin_file_stream_create(stdout);
	if (!out) {
		goto error;
	}
	for (i = 1; i < argc; ++i) {
		jsmin_stream_printf(out, "// %s\n", argv[i]);
	}
	jsmin = jsmin_create(in, out);
	if (!jsmin) {
		return EXIT_FAILURE;
	}
	jsmin_minify(jsmin);
exit:
	jsmin_stream_destroy(in);
	jsmin_stream_destroy(out);
	jsmin_destroy(jsmin);
	return status;
error:
	status = EXIT_FAILURE;
	goto exit;
}
