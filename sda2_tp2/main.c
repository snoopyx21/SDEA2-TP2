/**
 * \file main.c
 * \brief Main.
 * \date 2017
 * \author SDA2
 * \copyright WTFPLv2
 */

/* Copyright © 2017 Harenome Razanajato <hrazanajato@unistra.fr>
 *
 * This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar.
 *
 * See http://www.wtfpl.net/ or read below for more details.
 */
/* DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
 *                    Version 2, December 2004
 *
 * Copyright (C) 2004 Sam Hocevar <sam@hocevar.net>
 *
 * Everyone is permitted to copy and distribute verbatim or modified
 * copies of this license document, and changing it is allowed as long
 * as the name is changed.
 *
 *            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
 *   TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
 *
 *  0. You just DO WHAT THE FUCK YOU WANT TO.
 */

#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#include "stringtable.h"

#define MAX_STR 512
#define MAX_PRENOM 128
#define NN 5

////////////////////////////////////////////////////////////////////////////////
// Main.
////////////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[argc-1])
{
  FILE *fnames;
  
  /* init the table. */
  stringtable tnames = newStringtable();

  fnames = fopen("Prenoms.txt", "r");
  if (fnames == NULL) { perror("File Prenoms.txt error"); exit(1); }
  char buffer[MAX_STR];
  char prenom[MAX_PRENOM];
 
  /* Read file and add all names to the table. */
  unsigned int counter = 0;
  fgets(buffer, MAX_STR, fnames); // jump over first line in file
  fgets(buffer, MAX_STR, fnames);
  do
  { 
	  int i = 0; 
	  bool ignore = false;
	  while (buffer[i] != '\t') 
	  { 
		  // ignore names that contain special caracters
		  if (buffer[i]<'a' || buffer[i]>'z') ignore = true; 
		  prenom[i] = buffer[i]; 
		  i++; 
	  }
	  prenom[i] = 0;
	  if (!ignore)
	  {
		  if (search_id(&tnames, prenom) == 0)
		  {
			  unsigned int id = adj(&tnames, prenom);
			  printf("%s (%u)\n", prenom, id);
			  counter++;
		  }
	  }


	  fgets(buffer, MAX_STR, fnames);
  } while (!feof(fnames));

  printf("Database contains: %u names!\n", counter);

  // Test some names
  printf("\n\nTrying to find names:\n");
  char *n[NN] = { "jean", "pierre", "marcel", "toto", "igor" };
  bool v[NN] = { true,true,true,false,true };
  unsigned int id[NN];
  int i;
  for (i = 0; i < NN; i++)
  {
	  id[i] = search_id(&tnames, n[i]);
	  if (id[i] !=0) printf("%s found, id = %u -> %s\n", n[i], id[i], v[i] ? "correct":"faux");
	  else printf("%s not found -> %s\n", n[i], !v[i] ? "correct" : "faux");
  }
  printf("\ndeleting %s\n", n[0]); sup(&tnames, n[0]); v[0] = false;
  printf("adding %s\n", n[3]); adj(&tnames, n[3]); v[3] = true;
  printf("\n\nNow trying to find names again (id should not change):\n");
  for (i = 0; i < NN; i++)
  {
	  unsigned int idn = search_id(&tnames, n[i]);
	  if (idn!=0 && idn != id[i]) printf("warning %s has new identifier: %u versus %u -> faux\n", n[i], idn, id[i]);
	  else
	  {
		  if (id[i] != 0) printf("%s found, id = %u -> %s\n", n[i], id[i], v[i] ? "correct" : "faux");
		  else printf("%s not found -> %s\n", n[i], !v[i] ? "correct" : "faux");
	  }
  }


  /* Cleanup.
    */
  freeStringtable(&tnames);
  return 0;
}

