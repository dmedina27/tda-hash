#include <string.h>
#include <stdlib.h>

#include "hash.h"

#define FACTOR_CARGA_MAXIMO 0.7
#define CAPACIDAD_MINIMA 3

struct nodo {
    const char* clave;
    void* valor;
    struct nodo *siguiente;
};

struct hash {
    struct nodo** vector;
    size_t capacidad;
    size_t cantidad;
};

hash_t *hash_crear(size_t capacidad)
{
	hash_t* nuevo_hash = calloc(1,sizeof(struct hash));
	if(!nuevo_hash)
		return NULL;
	nuevo_hash->capacidad = capacidad;
	if(capacidad < CAPACIDAD_MINIMA)
		nuevo_hash->capacidad = CAPACIDAD_MINIMA;
	nuevo_hash->cantidad = 0;
	nuevo_hash->vector = malloc(sizeof(struct nodo*)*capacidad);
	if(!nuevo_hash->vector){
		free(nuevo_hash);
		return NULL;
	}
	return nuevo_hash;
}

/*
*	funcion de hash
*/
size_t funcion_hash(const char *str)
{
    size_t hash = 5381;
    size_t c;
    while((c = (size_t)*str++))
        hash = ((hash << 5) + hash) + c;
    return hash;
}
/*
*	funcion que dada una clave y un valor 
*	reserva memoria y genera un nodo con dichos elementos guardados
*	y su direccion siguiente en NULL
*	con la particularidad que genera una copia de la clave
*	los parametros tienen que ser validos
*	devuelve el nodo o NULL en caso de error
*/
struct nodo* nodo_crear(const char* clave, void* valor){
    struct nodo *nuevo_nodo = calloc(1, sizeof(struct nodo));
    if(!nuevo_nodo){
        return NULL;
    }
    //me guardo una copia del string
    //size_t tamanio_clave = strlen(clave);
    //char copia_clave[tamanio_clave];
    //strcpy(copia_clave,clave);
    //nuevo_nodo->clave = copia_clave;
    
	nuevo_nodo->clave = clave;
	nuevo_nodo->valor = valor;
    nuevo_nodo->siguiente = NULL;
    return nuevo_nodo;
}

/*
*	funcion que agranda mi vector al doble de la capacidad
*	ordenando los elementos en sus nuevas posiciones
*	liberando el vector antiguo y devolviendo el hash actualizado
*	el parametro tiene que ser valido
*/
hash_t* rehashear(hash_t* hash){
    struct nodo** vector_viejo = hash->vector;
    hash->vector = malloc(sizeof(hash->capacidad)*2);
    hash->capacidad *= 2;
    hash->cantidad = 0;
    //copio todos los elementos
    for(int i = 0; i < hash->capacidad; i++){
        struct nodo *actual = vector_viejo[i];
        while(actual){
            hash_insertar(hash,actual->clave,actual->valor,NULL);
            actual = actual->siguiente;
        }
    }
    free(vector_viejo);
    return hash;
}


hash_t *hash_insertar(hash_t *hash, const char *clave, void *elemento,
		      void **anterior)
{
	if (!hash || !clave || !hash->vector)
		return NULL;
	size_t posicion = funcion_hash(clave) % hash->capacidad;
	float factor_carga = (float)hash->cantidad / (float)hash->capacidad;
	if(factor_carga > FACTOR_CARGA_MAXIMO)
    	rehashear(hash);
	//si el lugar esta vacio
	if(!hash->vector[posicion]){
		struct nodo* nuevo_nodo = nodo_crear(clave, elemento);
		if(!nuevo_nodo)
			return NULL;
		hash->vector[posicion] = nuevo_nodo;
		hash->cantidad ++;
		if(anterior)
			*anterior = NULL;
		return hash;
	}
	//si existe una clave igual, actualizo el valor
	struct nodo* actual = hash->vector[posicion];
	while(actual){
		if(strcmp(actual->clave,clave) == 0){
			if(anterior)	
				*anterior = actual->valor;
			actual->valor = elemento;
			return hash;
		}
		actual = actual->siguiente;
	}
	//si recorro y no esta la clave
	struct nodo* nuevo_nodo = nodo_crear(clave, elemento);
	if(!nuevo_nodo)
		return NULL;
	nuevo_nodo->siguiente = hash->vector[posicion];
	hash->vector[posicion] = nuevo_nodo;
	hash->cantidad ++;
	if(anterior)
		*anterior = NULL;
	return hash;
}


void *hash_quitar(hash_t *hash, const char *clave)
{
	if(!hash || !clave)
		return NULL;
	size_t posicion = funcion_hash(clave) % hash->capacidad;
	// Se busca el nodo correspondiente a la clave
	struct nodo* actual = hash->vector[posicion];
	struct nodo* anterior = NULL;
	while (actual && strcmp(actual->clave, clave) != 0) {
		anterior = actual;
		actual = actual->siguiente;
	}
	// Si se encontró el nodo, se elimina y se actualiza la tabla
	if (actual != NULL) {
		void* valor = actual->valor;
		if (anterior == NULL) {
			hash->vector[posicion] = actual->siguiente;
		}
		else {
			anterior->siguiente = actual->siguiente;
		}
		free(actual);
		hash->cantidad--;
		return valor;
	}
	return NULL;
}

void *hash_obtener(hash_t *hash, const char *clave)
{
	if (!hash || !clave)
		return NULL;
	size_t posicion = funcion_hash(clave) % hash->capacidad;
	if(!hash->vector[posicion])
		return NULL;
	struct nodo* actual = hash->vector[posicion];
	while(actual){
		if(strcmp(actual->clave, clave) == 0)
			return actual->valor;
		actual = actual->siguiente;
	}
	return NULL;
}

bool hash_contiene(hash_t *hash, const char *clave)
{
	if (!hash || !clave)
		return false;
	if(!hash_obtener(hash,clave))
		return false;
	return true;
}

size_t hash_cantidad(hash_t *hash)
{
	if (!hash)
		return 0;
	return hash->cantidad;
}

void hash_destruir(hash_t *hash)
{
	hash_destruir_todo(hash, NULL);
}

void hash_destruir_todo(hash_t *hash, void (*destructor)(void *))
{
	if (!hash)
		return;
	if(!hash->vector){
		free(hash);
		return ;
	}
	for(int i = 0; i < hash->capacidad;i++){
		if(hash->vector[i]){
			struct nodo* actual = hash->vector[i];
			while(actual){
				struct nodo* nodo_a_liberar = actual;
				actual = actual->siguiente;
				if(destructor)
					destructor(nodo_a_liberar->valor);
				free(nodo_a_liberar);
			}
		}
	}
	free(hash->vector);
	free(hash);
}

size_t hash_con_cada_clave(hash_t *hash,
			   bool (*f)(const char *clave, void *valor, void *aux),
			   void *aux)
{
	size_t n = 0;
	if (!hash || !f)
		return n;
	for(int i = 0; i < hash->capacidad;i++){
		struct nodo* actual = hash->vector[i];
		while(actual){
			if(!f(actual->clave,actual->valor, aux))
				return n;
			n++;
			actual = actual->siguiente;
		}
	}
	return n;
}
