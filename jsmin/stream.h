/* Copyright 2011 Michael Steinert
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
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

/**
 * \brief Stream I/O
 */

#ifndef JSMIN_STREAM_H
#define JSMIN_STREAM_H

#include <jsmin/macros.h>
#include <stdarg.h>
#include <stdio.h>

JSMIN_BEGIN_DECLS

typedef struct JsminStream_ JsminStream;

JSMIN_PUBLIC JsminStream *
jsmin_file_stream_create(FILE *file)
	JSMIN_WARN_UNUSED_RESULT;

JSMIN_PUBLIC JsminStream *
jsmin_filename_stream_create(const char *filename, const char *mode)
	JSMIN_WARN_UNUSED_RESULT;

JSMIN_PUBLIC JsminStream *
jsmin_memory_stream_create(char *buf, size_t length, const char *mode)
	JSMIN_WARN_UNUSED_RESULT;

JSMIN_PUBLIC void
jsmin_stream_destroy(JsminStream *self);

JSMIN_PUBLIC int
jsmin_stream_getc(JsminStream *self);

JSMIN_PUBLIC int
jsmin_stream_putc(JsminStream *self, int c);

JSMIN_PUBLIC int
jsmin_stream_printf(JsminStream *self, const char *format, ...);

JSMIN_END_DECLS

#endif /* JSMIN_STREAM_H */
