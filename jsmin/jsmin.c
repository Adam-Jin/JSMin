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
#include "jsmin/jsmin.h"
#include <stdlib.h>
#include <stdio.h>

struct Jsmin_ {
	int a;
	int b;
	int lookahead;
};

Jsmin *
jsmin_create(void)
{
	Jsmin *self = calloc(1, sizeof(*self));
	if (!self) {
		return NULL;
	}
	self->lookahead = EOF;
	return self;
}

void
jsmin_destroy(Jsmin *self)
{
	if (self) {
		free(self);
	}
}

/**
 * \brief Test the class of a character.
 *
 * \param [in] self A Jsmin object
 * \param [in] c The character to test
 *
 * \return True if c is a letter, digit, underscore, dollar sign, or
 *         non-ASCII character.
 */
static int
jsmin_isalnum(Jsmin *self, int c)
{
	return ((c >= 'a' && c <= 'z')
	     || (c >= '0' && c <= '9')
	     || (c >= 'A' && c <= 'Z')
	      || c == '_'
	      || c == '$'
	      || c == '\\'
	      || c > 126);
}

/**
 * \brief Return the next character from STDIN.
 *
 * Watch out for lookahead. If the character is a control character, translate
 * it to a space or linefeed.
 *
 * \param [in] self A Jsmin object
 *
 * \return The next character to process.
 */
static int
jsmin_get(Jsmin *self)
{
	int c = self->lookahead;
	self->lookahead = EOF;
	if (c == EOF) {
		c = getc(stdin);
	}
	if (c >= ' ' || c == '\n' || c == EOF) {
		return c;
	}
	if (c == '\r') {
		return '\n';
	}
	return ' ';
}

/**
 * \brief Get the next character without getting it.
 *
 * \param [in] self A Jsmin object.
 *
 * \return The next character in the stream.
 */
static int
jsmin_peek(Jsmin *self)
{
	self->lookahead = jsmin_get(self);
	return self->lookahead;
}

/**
 * \brief Get the next character, excluding comments.
 *
 * This function is used to see if a '/' is followed by a '/' or '*'.
 *
 * \param [in] self A Jsmin object.
 *
 * \return The next character in the stream.
 */
static int
jsmin_next(Jsmin *self)
{
	int c = jsmin_get(self);
	if  ('/' == c) {
		switch (jsmin_peek(self)) {
		case '/':
			while (1) {
				c = jsmin_get(self);
				if (c <= '\n') {
					return c;
				}
			}
		case '*':
			jsmin_get(self);
			while (1) {
				switch (jsmin_get(self)) {
				case '*':
					if (jsmin_peek(self) == '/') {
						jsmin_get(self);
						return ' ';
					}
					break;
				case EOF:
					fprintf(stderr, "Error: JSMIN Unterminated comment.\n");
					exit(1);
				}
			}
		default:
			return c;
		}
	}
	return c;
}

/**
 * \brief Do something!
 *
 * The action performed is determined by the argument:
 * 	1. Output A. Copy B to A. Get the next B.
 *	2. Copy B to A. Get the next B. (Delete A).
 *	3. Get the next B. (Delete B).
 *
 * This function treats a string as a single character and recognizes a
 * regular expression if it is preceded by ( or , or =.
 *
 * \param [in] self A Jsmin object
 * \param [in] d The action to perform
 */
static void
jsmin_action(Jsmin *self, int d)
{
	switch (d) {
	case 1:
		putc(self->a, stdout);
	case 2:
		self->a = self->b;
		if (self->a == '\'' || self->a == '"' || self->a == '`') {
			while (1) {
				putc(self->a, stdout);
				self->a = jsmin_get(self);
				if (self->a == self->b) {
					break;
				}
				if (self->a == '\\') {
					putc(self->a, stdout);
					self->a = jsmin_get(self);
				}
				if (self->a == EOF) {
					fprintf(stderr, "Error: JSMIN unterminated string literal.");
					exit(1);
				}
			}
		}
	case 3:
		self->b = jsmin_next(self);
		if (self->b == '/' && (self->a == '(' || self->a == ','
				    || self->a == '=' || self->a == ':'
				    || self->a == '[' || self->a == '!'
				    || self->a == '&' || self->a == '|'
				    || self->a == '?' || self->a == '{'
				    || self->a == '}' || self->a == ';'
				    || self->a == '\n'))
		{
			putc(self->a, stdout);
			putc(self->b, stdout);
			while (1) {
				self->a = jsmin_get(self);
				if (self->a == '[') {
					while (1) {
						putc(self->a, stdout);
						self->a = jsmin_get(self);
						if (self->a == ']') {
							break;
						}
						if (self->a == '\\') {
							putc(self->a, stdout);
							self->a = jsmin_get(self);
						}
						if (self->a == EOF) {
							fprintf(stderr, "Error: JSMIN unterminated set in Regular Expression literal.\n");
							exit(1);
						}
					}
				} else if (self->a == '/') {
					break;
				} else if (self->a =='\\') {
					putc(self->a, stdout);
					self->a = jsmin_get(self);
				}
				if (self->a == EOF) {
					fprintf(stderr, "Error: JSMIN unterminated Regular Expression literal.\n");
					exit(1);
				}
				putc(self->a, stdout);
			}
			self->b = jsmin_next(self);
		}
	}
}

void
jsmin_minify(Jsmin *self)
{
	self->a = '\n';
	jsmin_action(self, 3);
	while (EOF != self->a) {
		switch (self->a) {
		case ' ':
			if (jsmin_isalnum(self, self->b)) {
				jsmin_action(self, 1);
			} else {
				jsmin_action(self, 2);
			}
			break;
		case '\n':
			switch (self->b) {
			case '{':
			case '[':
			case '(':
			case '+':
			case '-':
				jsmin_action(self, 1);
				break;
			case ' ':
				jsmin_action(self, 3);
				break;
			default:
				if (jsmin_isalnum(self, self->b)) {
					jsmin_action(self, 1);
				} else {
					jsmin_action(self, 2);
				}
				break;
			}
			break;
		default:
			switch (self->b) {
			case ' ':
				if (jsmin_isalnum(self, self->a)) {
					jsmin_action(self, 1);
					break;
				}
				jsmin_action(self, 3);
				break;
			case '\n':
				switch (self->a) {
				case '}':
				case ']':
				case ')':
				case '+':
				case '-':
				case '"':
				case '\'':
				case '`':
					jsmin_action(self, 1);
					break;
				default:
					if (jsmin_isalnum(self, self->a)) {
						jsmin_action(self, 1);
					} else {
						jsmin_action(self, 3);
					}
					break;
				}
				break;
			default:
				jsmin_action(self, 1);
				break;
			}
		}
	}
}
