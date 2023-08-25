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


#include "stringtable2.h"

stringtable2 newStringtable()
{
	unsigned int i, j, k;
	stringtable2 t;
	t.id_count = FIRST_ID;
	for (j = 0; j < N_ALPHA; j++)
	{
		t.n_chaines[j] = 0;
		t.n_car[j] = 0;
		t.max_car[j] = MAX_CAR;
		t.max_chaines[j] = MAX_CHAINES;
		t.chaines[j] = (char *)malloc(sizeof(char)*t.max_car[j]);
		if (t.chaines[j] == NULL) 
		{ 
			printf("memory allocation error.\n"); 
			for (k = 0; k < j; k++) free(t.chaines[k]);
			exit(1);
		}
		for (i = 0; i < MAX_CAR; i++) t.chaines[j][i] = 0;
		t.adresse[j] = (unsigned int *)malloc(sizeof(unsigned int)*t.max_chaines[j]);
		if (t.adresse[j] == NULL) 
		{ 
			printf("memory allocation error.\n"); 
			for (k = 0; k < j; k++) free(t.chaines[k]);
			for (k = 0; k < j; k++) free(t.adresse[k]);
			exit(1);
		}
		t.indicestrie[j] = (int *)malloc(sizeof(int)*t.max_chaines[j]);
		if (t.indicestrie[j] == NULL) 
		{ 
			printf("memory allocation error.\n"); 
			for (k = 0; k < j; k++) free(t.chaines[k]);
			for (k = 0; k < j; k++) free(t.adresse[k]);
			for (k = 0; k < j; k++) free(t.indicestrie[k]);
			exit(1);
		}
	}
	return t;
}

void freeStringtable(stringtable2 *t)
{
	unsigned int k;
	for (k = 0; k < N_ALPHA; k++) free(t->chaines[k]);
	for (k = 0; k < N_ALPHA; k++) free(t->adresse[k]);
	for (k = 0; k < N_ALPHA; k++) free(t->indicestrie[k]);
}

// usage interne
int searchPosInd(stringtable2 *t, char *str)
{
	if (str[0] == 0) return 0;
	unsigned int k = str[0] - 'a';
	if (k >= N_ALPHA) return 0;
	int a = 0, b = t->n_chaines[k] - 1;
	while (a<b)
	{
		int m = (a + b) / 2;
		unsigned short lg = strlen(str) < stringLength(t, k, m) ? strlen(str) : stringLength(t,k, m);
		int cmp = strncmp(t->chaines[k] + t->adresse[k][m], str, lg);
		if (cmp<0 || (cmp == 0 && strlen(str) > stringLength(t,k, m))) a = m + 1; else b = m;
	}
	return a;
}
// usage interne
int searchPos(stringtable2 *t, char *str)
{
	if (str[0] == 0) return -1;
	unsigned int k = str[0] - 'a';
	if (k >= N_ALPHA) return -1;
	int a = searchPosInd(t, str);
	if (strlen(str) == stringLength(t,k, a)) { if (strncmp(t->chaines[k] + t->adresse[k][a], str, strlen(str)) == 0) return a; }
	return -1; // not found, return value out of range
}

// usage interne
char  *searchAdress(stringtable2 *t, unsigned int k,  int id)
{
	unsigned int i;
	while (i < t->n_chaines[k]) { if (t->indicestrie[k][i] == id) return t->chaines[k] + t->adresse[k][i]; else i++; }
	return NULL;
}

// usage interne
unsigned short stringLength(stringtable2 *t, unsigned int k, int pos)
{
	return strlen(t->chaines[k] + t->adresse[k][pos]);
}

///////////////////////////////////////////////////////////
unsigned int search_id(stringtable2 *t, char *str)
{
	unsigned int k = str[0] - 'a';
	if (k >= N_ALPHA) return OMEGA;
	int i = searchPos(t, str);
	if (i == -1) return OMEGA; // not found, return a value out of range
	else return t->indicestrie[k][i];
}

unsigned int adj(stringtable2 *t, char *str)
{
	unsigned int k = str[0] - 'a';
	if (k >= N_ALPHA) return OMEGA;
	unsigned int ll = strlen(str);
	if (ll + t->n_car[k] >= t->max_car[k]) // debordement memoire
	{
		t->chaines[k] = (char *)realloc(t->chaines[k], sizeof(char)*(t->max_car[k] + ll + MAX_CAR));
		if (t->chaines[k] == NULL) { printf("memory allocation error.\n"); freeStringtable(t);  exit(1); }
		t->max_car[k] += ll + MAX_CAR;
	}
	if (t->n_chaines[k] >= t->max_chaines[k]) // debordement memoire
	{
		t->indicestrie[k] = (int *)realloc(t->indicestrie[k], sizeof(int)*(t->max_chaines[k] + MAX_CHAINES));
		if (t->indicestrie[k] == NULL) { printf("memory allocation error.\n"); freeStringtable(t); exit(1); }
		t->adresse[k] = (unsigned int *)realloc(t->adresse[k], sizeof(unsigned int)*(t->max_chaines[k] + MAX_CHAINES));
		if (t->adresse[k] == NULL) { printf("memory allocation error.\n"); freeStringtable(t); exit(1); }
		t->max_chaines[k] += MAX_CHAINES;
	}
	// recherche bonne position dans indicetrie
	int pos = searchPosInd(t, str);
	if (strcmp(t->chaines[k] + t->adresse[k][pos], str)<0) pos++;
	// decalage
	int i;
	for (i = t->n_chaines[k]; i > pos; i--) {
		t->indicestrie[k][i] = t->indicestrie[k][i - 1]; t->adresse[k][i] = t->adresse[k][i - 1];
	}
	t->adresse[k][pos] = t->n_car[k];
	t->indicestrie[k][pos] = t->id_count;
	t->n_chaines[k]++;
	// adjonction en queue pour les caracteres
	strcpy(t->chaines[k] + t->n_car[k], str);
	t->chaines[k][t->n_car[k] + ll] = 0;
	t->n_car[k] += ll+1;

	t->id_count++;
	return t->id_count - 1;
}

void sup(stringtable2 *t, char *str)
{
	unsigned int k = str[0] - 'a';
	if (k >= N_ALPHA) return;
	int i = searchPos(t, str);
	if (i == -1) return;
	unsigned int a = i;
	unsigned int car_pos = t->adresse[k][a];
	unsigned int length = stringLength(t, k,a)+1;
	// supression dans un tableau->décaler les cases du tableau
	for (i = a; i<t->n_chaines[k] - 1; i++) t->indicestrie[k][i] = t->indicestrie[k][i + 1];
	for (i = a; i<t->n_chaines[k] - 1; i++) t->adresse[k][i] = t->adresse[k][i + 1] - (t->adresse[k][i+1]>car_pos ? length:0); // reajuster les adresses a cause du decalage
	t->n_chaines[k]--;
	// idem pour le tableau contenant les caractères
	unsigned int j;
	for (j = car_pos; j<t->n_car[k] - length; j++) t->chaines[k][j] = t->chaines[k][j + length];
	t->n_car[k] -= length;
	t->chaines[k][t->n_car[k]] = 0;
}


  
