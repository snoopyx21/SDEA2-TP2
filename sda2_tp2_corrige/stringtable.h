/**
 * \file stringtable.h
 * \brief Stupid wrappers for time measurement.
 * \date 2017
 * \author 
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

#define MAX_CAR 50
#define MAX_CHAINES 5
#define FIRST_ID 1
#define OMEGA 0

typedef struct {
	char				*chaines;
	unsigned int		*adresse;
	unsigned int		*indicestrie;
	unsigned int		id_count;

	unsigned short		n_chaines;
	unsigned int		n_car;
	unsigned int		max_car;
	unsigned short		max_chaines;

} stringtable;

stringtable newStringtable();
void freeStringtable(stringtable *t);
unsigned int search_id(stringtable *t, char *str);
unsigned int adj(stringtable *t, char *str);
void sup(stringtable *t, char *str);

// usage interne uniquement
unsigned short stringLength(stringtable *t, int pos);
int searchPosInd(stringtable *t, char *str);
int searchPos(stringtable *t, char *str);
char  *searchAdress(stringtable *t, int id);

  
