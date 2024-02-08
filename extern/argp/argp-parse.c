/* Hierarchial argument parsing, layered over getopt
   Copyright (C) 1995-2023 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Written by Miles Bader <miles@gnu.ai.mit.edu>.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <https://www.gnu.org/licenses/>.  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <getopt.h>
#include "getopt_int.h"


#include "argp.h"
#include "argp-namefrob.h"


int argp_end_options(struct argp_option opt)	{
	return	!opt.name &&
			!opt.key  &&
			!opt.arg  &&
			!opt.flags &&
			!opt.doc &&
			!opt.group;
	return 0;
}

/* Parse the options strings in ARGC & ARGV according to the argp in ARGP.
   FLAGS is one of the ARGP_ flags above.  If END_INDEX is non-NULL, the
   index in ARGV of the first unparsed option is returned in it.  If an
   unknown option is present, EINVAL is returned; if some parser routine
   returned a non-zero value, it is returned; otherwise 0 is returned.  */
error_t
argp_parse (const struct argp *argp, int argc, char **argv, unsigned flags,
	      int *end_index, void *input)
{
  error_t err;
  const struct argp * backend_argp = argp;
 	struct argp_state state = {backend_argp,argc,argv,0,flags,0,0,input,0,0,argv[0],0,0,0};
  void *child_inputs[16];
  state.child_inputs = child_inputs;

  char buf[256];
  for (int i = 1; i < argc; i++) {
  strcpy ( buf, argv[i]);
      char *arg = buf;

      char *equal_sign = strchr(arg, '=');
      char *param = NULL, *value = NULL;
      if (equal_sign) {
          *equal_sign = '\0';
          param = arg + 2;
          value = equal_sign + 1;
      }
      else	{
    param = arg+2;
  }

  int argp_argc = 0;

  while( argp_end_options( backend_argp->options[argp_argc]) == 0)	{

    if ( strcasecmp( backend_argp->options[argp_argc].name, param) == 0)
    {
      backend_argp->parser(backend_argp->options[argp_argc].key, value, &state);
    }
    argp_argc++;
  }
  }
  return 0;
}
