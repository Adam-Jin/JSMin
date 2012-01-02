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
#define _BSD_SOURCE
#include <getopt.h>
#include "jsmin/jsmin.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef void (*JsminOptionCallbackFunction)(int index, const char *argument,
		const char *value, void *user_data);

static int
jsmin_getopt_long(int *argc, char ***argv, const struct option *options,
		  const char *sopts, JsminOptionCallbackFunction callback,
		  void *user_data)
{
	int i, opt, new_argc = 1;
	char *old_optarg = optarg;
	int old_opterr = opterr, old_optind = optind, old_optopt = optopt;
	char **new_argv = calloc(*argc, sizeof(char *));
	if (!new_argv) {
		return -1;
	}
	optind = opterr = 0;
	while (-1 != (opt = getopt_long(*argc, *argv, "", options, &i))) {
		switch (opt) {
		case '?':
			new_argv[new_argc++] = (*argv)[optind - 1];
			break;
		default:
			callback(opt, options[i].name, optarg, user_data);
			break;
		}
	}
	/* construct new argv array */
	for (i = 1; i < new_argc; ++i) {
		(*argv)[i] = new_argv[i];
	}
	free(new_argv);
	for (i = optind; i < *argc; ++i) {
		(*argv)[new_argc++] = (*argv)[i];
	}
	*argc = new_argc;
	/* restore getopt globals */
	optind = old_optind;
	opterr = old_opterr;
	optopt = old_optopt;
	optarg = old_optarg;
	return 0;
}

typedef struct Main_ Main;

struct Main_ {
	Jsmin *min;
	JsminStream *in;
	JsminStream *out;
	char *outfile;
};

typedef enum {
	MAIN_OPTION_MAX
} MainOption;

static void
main_option_callback(int index, const char *argument, const char *value,
		     void *user_data)
{
	Main *self = (Main *)user_data;
	switch (index) {
	case 'o':
		free(self->outfile);
		self->outfile = strdup(argument);
		break;
	default:
		break;
	}
}

static int
main_parse_args(Main *self, int *argc, char ***argv)
{
	int status;
	const struct option long_options[] = {
		{ NULL }
	};
	status = jsmin_getopt_long(argc, argv, long_options, "o:",
			main_option_callback, self);
	if (status) {
		return -1;
	}
	return 0;
}

static void
main_destroy(Main *self)
{
	if (self) {
		jsmin_destroy(self->min);
		jsmin_stream_destroy(self->in);
		jsmin_stream_destroy(self->out);
		free(self->outfile);
		free(self);
	}
}

Main *
main_create(int *argc, char ***argv)
{
	int i;
	Main *self = calloc(1, sizeof(*self));
	if (!self) {
		return NULL;
	}
	if (argv && argc) {
		int status = main_parse_args(self, argc, argv);
		if (status) {
			goto error;
		}
	}
	if (!self->in) {
		self->in = jsmin_file_stream_create(stdin);
		if (!self->in) {
			goto error;
		}
	}
	if (!self->out) {
		self->out = jsmin_file_stream_create(stdout);
		if (!self->out) {
			goto error;
		}
	}
	self->min = jsmin_create(self->in, self->out);
	if (!self->min) {
		goto error;
	}
	for (i = 1; i < *argc; ++i) {
		jsmin_stream_printf(self->out, "// %s\n", (*argv)[i]);
	}
	return self;
error:
	main_destroy(self);
	return NULL;
}

int
main_run(Main *self)
{
	jsmin_minify(self->min);
	return EXIT_SUCCESS;
}

static FILE *
help(FILE *stream, const char *name)
{
	fprintf(stream, "usage: %s [OPTIONS]...\n\n", name);
	return stream;
}

static FILE *
version(FILE *stream)
{
	fprintf(stream, "%s/%s\n\n", PACKAGE_NAME, VERSION);
	return stream;
}

/**
 * \brief Minify a JavaScript file.
 *
 * The input is minified and unrecognized command line arguments are output
 * as comments.
 *
 * \param [in] argc The argument count
 * \param [in] argv The argument vector
 *
 * \return The program returns EXIT_SUCCESS or EXIT_FAILURE.
 */
int
main(int argc, char **argv)
{
	Main *app = NULL;
	int i, opt, status = EXIT_SUCCESS;
	const struct option options[] = {
		{ "help", no_argument, NULL, 'h' },
		{ "version", no_argument, NULL, 'v' },
		{ NULL }
	};
	opterr = 0;
	while (-1 != (opt = getopt_long(argc, argv, "hv", options, &i))) {
		switch (opt) {
		case 'h': /* help */
			help(version(stdout), argv[0]);
			goto exit;
		case 'v': /* version */
			version(stdout);
			goto exit;
		default:
			break;
		}
	}
	app = main_create(&argc, &argv);
	if (!app) {
		goto error;
	}
	status = main_run(app);
exit:
	main_destroy(app);
	return status;
error:
	status = EXIT_FAILURE;
	goto exit;
}
