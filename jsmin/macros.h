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
 * \brief Common macros
 */

#ifndef JSMIN_MACROS_H
#define JSMIN_MACROS_H

#ifdef __cplusplus
#define JSMIN_BEGIN_DECLS extern "C" {
#define JSMIN_END_DECLS }
#else
#define JSMIN_BEGIN_DECLS
#define JSMIN_END_DECLS
#endif

JSMIN_BEGIN_DECLS

#if __GNUC__ >= 4
#define JSMIN_PUBLIC __attribute__ ((visibility ("default")))
#define JSMIN_PRIVATE __attribute__ ((visibility ("hidden")))
#else
#define JSMIN_PUBLIC
#define JSMIN_PRIVATE
#endif

#if __GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 4)
#define JSMIN_WARN_UNUSED_RESULT __attribute__ ((warn_unused_result))
#else
#define JSMIN_WARN_UNUSED_RESULT
#endif

JSMIN_END_DECLS

#endif /* JSMIN_MACROS_H */
