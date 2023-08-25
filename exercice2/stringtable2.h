/**
 * \file stringtable2.h
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

#define N_ALPHA 26

typedef struct {
	char				*chaines[N_ALPHA];
	unsigned int		*adresse[N_ALPHA];
	unsigned int		*indicestrie[N_ALPHA];
	unsigned int		id_count;

	unsigned short		n_chaines[N_ALPHA];
	unsigned int		n_car[N_ALPHA];
	unsigned int		max_car[N_ALPHA];
	unsigned short		max_chaines[N_ALPHA];

} stringtable2;

stringtable2 newStringtable();
void freeStringtable(stringtable2 *t);
unsigned int search_id(stringtable2 *t, char *str);
unsigned int adj(stringtable2 *t, char *str);
void sup(stringtable2 *t, char *str);

// usage interne uniquement
unsigned short stringLength(stringtable2 *t, unsigned int k, int pos);
int searchPosInd(stringtable2 *t, char *str);
int searchPos(stringtable2 *t, char *str);
char  *searchAdress(stringtable2 *t, unsigned int k, int id);

  
