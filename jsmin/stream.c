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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#define _GNU_SOURCE
#include "jsmin/stream.h"
#include <stdlib.h>

struct JsminStream_ {
	struct JsminStreamClass *klass;
};

struct JsminStreamClass {
	void (*destroy)(void *self);
	int (*fgetc)(void *self);
	int (*fputc)(void *self, int c);
};

void
jsmin_stream_destroy(JsminStream *self)
{
	if (self) {
		if (self->klass->destroy) {
			self->klass->destroy(self);
		}
		free(self);
	}
}

int
jsmin_stream_getc(JsminStream *self)
{
	return self->klass->fgetc(self);
}

int
jsmin_stream_putc(JsminStream *self, int c)
{
	return self->klass->fputc(self, c);
}

struct JsminFileStream {
	JsminStream stream;
	FILE *file;
};

static int
jsmin_file_stream_getc(void *base)
{
	struct JsminFileStream *self = (struct JsminFileStream *)base;
	return fgetc(self->file);
}

static int
jsmin_file_stream_putc(void *base, int c)
{
	struct JsminFileStream *self = (struct JsminFileStream *)base;
	return fputc(c, self->file);
}

static struct JsminStreamClass JsminFileStreamClass = {
	NULL,
	jsmin_file_stream_getc,
	jsmin_file_stream_putc
};

JsminStream *
jsmin_file_stream_create(FILE *file)
{
	struct JsminFileStream *self = calloc(1, sizeof(*self));
	if (!self) {
		return NULL;
	}
	self->stream.klass = &JsminFileStreamClass;
	self->file = file;
	return (JsminStream *)self;
}

struct JsminFilenameStream {
	struct JsminFileStream file_stream;
};

static void
jsmin_filename_stream_destroy(void *base)
{
	struct JsminFilenameStream *self = (struct JsminFilenameStream *)base;
	fclose(self->file_stream.file);
}

static struct JsminStreamClass JsminFilenameStreamClass = {
	jsmin_filename_stream_destroy,
	jsmin_file_stream_getc,
	jsmin_file_stream_putc
};

JsminStream *
jsmin_filename_stream_create(const char *filename, const char *mode)
{
	struct JsminFilenameStream *self = calloc(1, sizeof(*self));
	if (!self) {
		return NULL;
	}
	self->file_stream.stream.klass = &JsminFilenameStreamClass;
	self->file_stream.file = fopen(filename, mode);
	if (!self->file_stream.file) {
		goto error;
	}
	return (JsminStream *)self;
error:
	free(self);
	return NULL;
}

struct JsminMemoryStream {
	struct JsminFilenameStream filename_stream;
};

JsminStream *
jsmin_memory_stream_create(char *buf, size_t length, const char *mode)
{
	struct JsminMemoryStream *self = calloc(1, sizeof(*self));
	if (!self) {
		return NULL;
	}
	self->filename_stream.file_stream.stream.klass =
		&JsminFilenameStreamClass;
	self->filename_stream.file_stream.file = fmemopen(buf, length, mode);
	if (!self->filename_stream.file_stream.file) {
		goto error;
	}
	return (JsminStream *)self;
error:
	free(self);
	return NULL;
}
