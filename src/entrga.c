#include "pokemon.h"
#include "ataque.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define LECTURA "r"
#define MAX_ATAQUES 3
#define MAX_POKEMONES 10000

struct pokemon {
	char nombre[20];
	enum TIPO tipo;
	struct ataque *ataque[MAX_ATAQUES];
	int tope;
};

struct info_pokemon {
	struct pokemon *pokemon[MAX_POKEMONES];
	int cantidad_de_pokemones;
};

void pokemon_destruir(struct pokemon *pokemon)
{
	if (pokemon == NULL) {
		return;
	}

	for (int i = 0; i < pokemon->tope; i++) {
		free(pokemon->ataque[i]);
	}

	free(pokemon);
}

int pokemon_cantidad(informacion_pokemon_t *ip)
{
	if (ip == NULL) {
		return 0;
	}
	return ip->cantidad_de_pokemones;
}

void pokemon_destruir_todo(informacion_pokemon_t *ip)
{
	if (ip == NULL) {
		return;
	}
	for (int i = 0; i < ip->cantidad_de_pokemones; i++) {
		pokemon_destruir(ip->pokemon[i]);
	}
	free(ip);
}

const char *pokemon_nombre(pokemon_t *pokemon)
{
	return (pokemon != NULL) ? pokemon->nombre : NULL;
}

int con_cada_ataque(pokemon_t *pokemon,
		    void (*f)(const struct ataque *, void *), void *aux)
{
	if (pokemon == NULL || f == NULL) {
		return 0;
	}
	int cantidad_aplicada = 0;
	for (int i = 0; i < pokemon->tope; i++) {
		f(pokemon->ataque[i], aux);
		cantidad_aplicada++;
	}
	return cantidad_aplicada;
}

enum TIPO pokemon_tipo(pokemon_t *pokemon)
{
	if (pokemon == NULL) {
		return NORMAL;
	}
	return pokemon->tipo;
}

enum TIPO asignar_poder(char letra)
{
	switch (letra) {
	case 'N':
		return NORMAL;
	case 'F':
		return FUEGO;
	case 'A':
		return AGUA;
	case 'P':
		return PLANTA;
	case 'E':
		return ELECTRICO;
	case 'R':
		return ROCA;
	default:
		return -1;
	}
}

int tiene_tres_ataques(pokemon_t *pokemon)
{
	return (pokemon != NULL && pokemon->tope == 3);
}

void cargar_ataque(pokemon_t *pokemon, char *linea)
{
	if (pokemon == NULL) {
		return;
	}
	if (pokemon->tope >= MAX_ATAQUES) {
		return; 
	}

	struct ataque *ataque = malloc(sizeof(struct ataque));

	if (ataque == NULL) {
		return;
	}
	char variable_tipo;

	if (sscanf(linea, "%49[^;];%c;%u\n", ataque->nombre, &variable_tipo,
		   &ataque->poder) == 3) {
		ataque->tipo = asignar_poder(variable_tipo);
		if (ataque->tipo == -1) {
			free(ataque);
			return;
		}
		pokemon->ataque[pokemon->tope] = ataque; //.
		pokemon->tope++;

	} else {
		free(ataque);
		return;
	}
}

const struct ataque *pokemon_buscar_ataque(pokemon_t *pokemon,
					   const char *nombre)
{
	if (pokemon == NULL || nombre == NULL) {
		return NULL;
	}
	for (int i = 0; i < pokemon->tope; i++) {
		if (strcmp(pokemon->ataque[i]->nombre, nombre) == 0) {
			return pokemon->ataque[i];
		}
	}
	return NULL;
}

int con_cada_pokemon(informacion_pokemon_t *ip, void (*f)(pokemon_t *, void *),
		     void *aux)
{
	if (ip == NULL || f == NULL) {
		return 0;
	}

	int cantidad_aplicada = 0;
	int n = ip->cantidad_de_pokemones;

	for (int i = 0; i < n - 1; i++) {
		for (int j = 0; j < n - i - 1; j++) {
			if (strcmp(ip->pokemon[j]->nombre,
				   ip->pokemon[j + 1]->nombre) > 0) {
				struct pokemon *temp = ip->pokemon[j];
				ip->pokemon[j] = ip->pokemon[j + 1];
				ip->pokemon[j + 1] = temp;
			}
		}
	}

	for (int i = 0; i < n; i++) {
		if (ip->pokemon[i] != NULL) {
			f(ip->pokemon[i], aux);
			cantidad_aplicada++;
		}
	}
	return cantidad_aplicada;
}

void funcion_de_procesamiento(struct pokemon *pokemon, void *aux)
{
	printf("Nombre: %s, Tipo: %d\n", pokemon->nombre, pokemon->tipo);
}

pokemon_t *pokemon_buscar(informacion_pokemon_t *ip,
			  const char *pokemon_que_quiero_encontrar)
{
	if (ip == NULL || pokemon_que_quiero_encontrar == NULL) {
		return NULL;
	}

	for (int i = 0; i < ip->cantidad_de_pokemones; i++) {
		if (strcmp(ip->pokemon[i]->nombre,
			   pokemon_que_quiero_encontrar) == 0) {
			return ip->pokemon[i];
		}
	}
	return NULL;
}
int contar_delimitador(char *linea)
{
	if (strchr(linea, ';') == NULL) {
		return -1;
	}
	int cantida_de_delimitadores = 0;
	for (size_t j = 0; linea[j] != '\0'; j++) {
		if (linea[j] == ';')
			cantida_de_delimitadores++;
		if (linea[j] == ':') {
			cantida_de_delimitadores = -1;
		}
	}
	return cantida_de_delimitadores;
}
void crear_pokemon(informacion_pokemon_t *info, char *linea)
{
	if (info == NULL || info->cantidad_de_pokemones >= MAX_POKEMONES) {
		return; //
	}
	pokemon_t *pokemon_actual = malloc(sizeof(pokemon_t));

	if (pokemon_actual == NULL) {
		return;
	}

	char variable_nombre[20];
	char variable_tipo;

	if (sscanf(linea, "%19[^;];%c\n", variable_nombre, &variable_tipo) ==
	    2) {
		strcpy(pokemon_actual->nombre, variable_nombre);
		pokemon_actual->tipo = asignar_poder(variable_tipo);
		if (pokemon_actual->tipo == -1) {
			free(pokemon_actual);
			return;
		}
		pokemon_actual->tope = 0;
		info->pokemon[info->cantidad_de_pokemones] = pokemon_actual;
		info->cantidad_de_pokemones++;
	} else {
		free(pokemon_actual);
		return;
	}
}

informacion_pokemon_t *pokemon_cargar_archivo(const char *path)
{
	FILE *archivo = fopen(path, LECTURA);

	if (path == NULL) {
		return NULL;
	}
	if (archivo == NULL) {
		perror("Error al abrir el archivo");
		return NULL;
	}

	informacion_pokemon_t *info = malloc(sizeof(informacion_pokemon_t));
	if (info == NULL) {
		fclose(archivo);
		return NULL;
	}
	info->cantidad_de_pokemones = 0;
	char linea[100];
	int cantidad_delimitador = 0;
	while (fgets(linea, sizeof(linea), archivo) != NULL) {
		cantidad_delimitador = contar_delimitador(linea);
		if (cantidad_delimitador == -1) {
			free(info);
			fclose(archivo);
			return NULL;
		}

		if (cantidad_delimitador != 1 && cantidad_delimitador != 2) {
			free(info);
			fclose(archivo);
			return NULL;
		}
		if (cantidad_delimitador == 1) {
			crear_pokemon(info, linea);
		} else if (cantidad_delimitador == 2) {
			cargar_ataque(
				info->pokemon[info->cantidad_de_pokemones - 1],
				linea);
		} else {
			break;
		}
	}

	int nuevos_pokemones = 0;
	for (int i = 0; i < info->cantidad_de_pokemones; i++) {
		if (tiene_tres_ataques(info->pokemon[i])) {
			info->pokemon[nuevos_pokemones] = info->pokemon[i];
			nuevos_pokemones++;
		} else {
			pokemon_destruir(info->pokemon[i]);
		}
	}
	info->cantidad_de_pokemones = nuevos_pokemones;
	fclose(archivo);

	return info;
}

