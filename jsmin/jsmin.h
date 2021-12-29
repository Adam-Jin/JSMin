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
 * \brief Minify JavaScript source code
 */

#ifndef JSMIN_H
#define JSMIN_H

#include <jsmin/macros.h>
#include <jsmin/stream.h>

JSMIN_BEGIN_DECLS

typedef struct Jsmin_ Jsmin;

/**
 * \brief Create a new Jsmin object.
 *
 * \return A new Jsmin object or NULL if an error occurred.
 */
JSMIN_PUBLIC Jsmin *
jsmin_create(JsminStream *in, JsminStream *out)
	JSMIN_WARN_UNUSED_RESULT;

/**
 * \brief Free resources used by a Jsmin object.
 *
 * \param [in] self A Jsmin object
 */
JSMIN_PUBLIC void
jsmin_destroy(Jsmin *self);

/**
 * \brief Minify input from STDIN.
 *
 * Copy the input to the output, deleting the characters which are
 * insignificant to JavaScript. Comments will be removed. Tabs will be
 * replaced with spaces. Carriage returns will be replaced with linefeeds.
 * Most spaces and linefeeds will be removed.
 *
 * \param [in] self A Jsmin object.
 */
JSMIN_PUBLIC void
jsmin_minify(Jsmin *self);

JSMIN_END_DECLS

#endif /* JSMIN_H */
