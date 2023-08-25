/**
 * \file stringtable.c
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


#include "stringtable.h"

stringtable newStringtable()
{
	stringtable t;
	t.n_chaines = 0;
	t.n_car = 0;
	t.id_count = FIRST_ID;
	t.max_car = MAX_CAR;
	t.max_chaines = MAX_CHAINES;
	t.chaines = (char *)malloc(sizeof(char)*t.max_car);
	if (t.chaines == NULL) { printf("memory allocation error.\n"); exit(1); }
	unsigned int i;
	for (i = 0; i<MAX_CAR; i++) t.chaines[i] = 0;
	t.adresse = (unsigned int *)malloc(sizeof(unsigned int)*t.max_chaines);
	if (t.adresse == NULL) { printf("memory allocation error.\n"); free(t.chaines); exit(1); }
	t.indicestrie = (int *)malloc(sizeof(int)*t.max_chaines);
	if (t.indicestrie == NULL) { printf("memory allocation error.\n"); free(t.adresse); free(t.chaines); exit(1); }
	return t;
}

void freeStringtable(stringtable *t)
{
	free(t->chaines);
	free(t->indicestrie);
	free(t->adresse);
}

// usage interne
int searchPosInd(stringtable *t, char *str)
{
	int a = 0, b = t->n_chaines - 1;
	while (a<b)
	{
		int m = (a + b) / 2;
		unsigned short lg = strlen(str) < stringLength(t, m) ? strlen(str) : stringLength(t, m);
		int cmp = strncmp(t->chaines + t->adresse[m], str, lg);
		if (cmp<0 || (cmp == 0 && strlen(str) > stringLength(t, m))) a = m + 1; else b = m;
	}
	return a;
}
// usage interne
int searchPos(stringtable *t, char *str)
{
	int a = searchPosInd(t, str);
	if (strlen(str) == stringLength(t, a)) { if (strncmp(t->chaines + t->adresse[a], str, strlen(str)) == 0) return a; }
	return -1; // not found, return value out of range
}

// usage interne
char  *searchAdress(stringtable *t, int id)
{
	unsigned int i;
	while (i < t->n_chaines) { if (t->indicestrie[i] == id) return t->chaines + t->adresse[i]; else i++; }
	return NULL;
}

// usage interne
unsigned short stringLength(stringtable *t, int pos)
{
	return strlen(t->chaines + t->adresse[pos]);
}

///////////////////////////////////////////////////////////
unsigned int search_id(stringtable *t, char *str)
{
	int i = searchPos(t, str);
	if (i == -1) return OMEGA; // not found, return a value out of range
	else return t->indicestrie[i];
}

unsigned int adj(stringtable *t, char *str)
{
	unsigned int ll = strlen(str);
	if (ll + t->n_car >= t->max_car) // debordement memoire
	{
		t->chaines = (char *)realloc(t->chaines, sizeof(char)*(t->max_car + ll + MAX_CAR));
		if (t->chaines == NULL) { printf("memory allocation error.\n"); exit(1); }
		t->max_car += ll + MAX_CAR;
	}
	if (t->n_chaines >= t->max_chaines) // debordement memoire
	{
		t->indicestrie = (int *)realloc(t->indicestrie, sizeof(int)*(t->max_chaines + MAX_CHAINES));
		if (t->indicestrie == NULL) { printf("memory allocation error.\n"); exit(1); }
		t->adresse = (unsigned int *)realloc(t->adresse, sizeof(unsigned int)*(t->max_chaines + MAX_CHAINES));
		if (t->adresse == NULL) { printf("memory allocation error.\n"); exit(1); }
		t->max_chaines += MAX_CHAINES;
	}
	// recherche bonne position dans indicetrie
	int pos = searchPosInd(t, str);
	if (strcmp(t->chaines + t->adresse[pos], str)<0) pos++;
	// decalage
	int i;
	for (i = t->n_chaines; i > pos; i--) {
		t->indicestrie[i] = t->indicestrie[i - 1]; t->adresse[i] = t->adresse[i - 1];
	}
	t->adresse[pos] = t->n_car;
	t->indicestrie[pos] = t->id_count;
	t->n_chaines++;
	// adjonction en queue pour les caracteres
	strcpy(t->chaines + t->n_car, str);
	t->chaines[t->n_car + ll] = 0;
	t->n_car += ll+1;

	t->id_count++;
	return t->id_count - 1;
}

void sup(stringtable *t, char *str)
{
	int i = searchPos(t, str);
	if (i == -1) return;
	unsigned int a = i;
	unsigned int car_pos = t->adresse[a];
	unsigned int length = stringLength(t, a)+1;
	// supression dans un tableau->décaler les cases du tableau
	for (i = a; i<t->n_chaines - 1; i++) t->indicestrie[i] = t->indicestrie[i + 1];
	for (i = a; i<t->n_chaines - 1; i++) t->adresse[i] = t->adresse[i + 1] - (t->adresse[i+1]>car_pos ? length:0); // reajuster les adresses a cause du decalage
	t->n_chaines--;
	// idem pour le tableau contenant les caractères
	unsigned int j;
	for (j = car_pos; j<t->n_car - length; j++) t->chaines[j] = t->chaines[j + length];
	t->n_car -= length;
	t->chaines[t->n_car] = 0;
}


  
