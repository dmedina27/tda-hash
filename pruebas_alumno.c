#include "pa2m.h"
#include "src/hash.h"
#include <stdio.h>
#include <string.h>

void prueba_simple()
{
	int i = 14;
	int j = 16;
	pa2m_afirmar(i != j, "i=14 es diferente de j=16");
}


void prueba_insertado_y_busqueda(){
	size_t capacidad = 300;
	hash_t *hash = hash_crear(capacidad);
	pa2m_afirmar(hash_insertar(hash,"1","ROJO",NULL) != NULL,"Se inserta un elemento");
	pa2m_afirmar(hash_insertar(hash,"350","AZUL",NULL) != NULL,"Se inserta un elemento");
	pa2m_afirmar(hash_insertar(hash,"4","MORADO",NULL) != NULL,"Se inserta un elemento");
	pa2m_afirmar(hash_insertar(hash,"7","AMARILLO",NULL) != NULL,"Se inserta un elemento");
	pa2m_afirmar(hash_insertar(hash,"9","BORDO",NULL) != NULL,"Se inserta un elemento");
	pa2m_afirmar(hash_insertar(hash,"2","CELESTE",NULL) != NULL,"Se inserta un elemento");
	char *string = {"ROJO"};
	char *string2 = {"AZUL"};
	pa2m_afirmar(hash_cantidad(hash) == 6,"El tamanio es 6");
	pa2m_afirmar(strcmp((char*)hash_obtener(hash,"99"), string) == 0, "el elemento es el rojo");
	pa2m_afirmar(strcmp((char*)hash_obtener(hash,"2"), string2) == 0, "el elemento es el rojo");
	pa2m_afirmar(strcmp((char*)hash_obtener(hash,"5"), string) == 0, "el elemento es el rojo");
	pa2m_afirmar(strcmp((char*)hash_obtener(hash,"7"), string) == 0, "el elemento es el rojo");
	pa2m_afirmar(strcmp((char*)hash_obtener(hash,"4"), string) == 0, "el elemento es el rojo");
	pa2m_afirmar(strcmp((char*)hash_obtener(hash,"5"), string) == 0, "el elemento es el rojo");
	hash_destruir(hash);
}

bool es_clave_valida(const char *clave, void *valor, void *aux)
{
	if (!clave)
		return false;
	aux = aux;
	return true;
}
void prueba_iterador(){
	size_t capacidad = 15; 
	hash_t *hash = hash_crear(capacidad);
	hash_insertar(hash,"1","ROJO",NULL);
	hash_insertar(hash,"350","AZUL",NULL);
	hash_insertar(hash,"4","MORADO",NULL);
	hash_insertar(hash,"7","AMARILLO",NULL);
	hash_insertar(hash,"9","BORDO",NULL);
	hash_insertar(hash,"2","CELESTE",NULL);
	hash_insertar(hash,"5","BLANCO",NULL);

	pa2m_afirmar(hash_con_cada_clave(hash,es_clave_valida,NULL) == 7,"Se iteran los elementos correspondientes");
	hash_destruir(hash);
}

bool insertar_claves(hash_t* hash){
	hash_insertar(hash,"1","ROJO",NULL);
	hash_insertar(hash,"350","AZUL",NULL);
	hash_insertar(hash,"4","MORADO",NULL);
	hash_insertar(hash,"7","AMARILLO",NULL);
	hash_insertar(hash,"9","BORDO",NULL);
	hash_insertar(hash,"2","CELESTE",NULL);
	hash_insertar(hash,"5","BLANCO",NULL);
	return true;
}

void pruebas_varias(){
	size_t capacidad = 7; 
	hash_t *hash = hash_crear(capacidad);
	pa2m_afirmar(hash != NULL,"Puedo crear un hash con capacidad validad");
	pa2m_afirmar(hash_cantidad(hash) == 0,"El hash esta vacio");
	pa2m_afirmar(hash_insertar(hash,"1","ROJO",NULL) != NULL,"Se inserta un elemento");
	pa2m_afirmar(hash_cantidad(hash) == 1,"Se actualiza la cantidad");
	char *string = {"ROJO"};
	char *string2 = {"NARANJA"};
	//void **p_string = (void**)string;
	pa2m_afirmar(strcmp((char*)hash_obtener(hash,"1"), string) == 0, "el elemento es el rojo");
	pa2m_afirmar(hash_insertar(hash,"1","NARANJA", NULL) != NULL,"Se inserta un elemento con misma clave: cambio rojo por naranja");
	pa2m_afirmar(strcmp((char*)hash_obtener(hash,"1"), string2) == 0, "el elemento es el naranja");

	pa2m_afirmar(hash_cantidad(hash) == 1,"La cantidad no se modifica");
	pa2m_afirmar(hash_quitar(hash,"1") != NULL,"Quito un elemento");
	pa2m_afirmar(hash_cantidad(hash) == 0,"Queda vacio");
	pa2m_afirmar(insertar_claves(hash) == true,"Se insertan varias claves");
	pa2m_afirmar(hash_cantidad(hash) == 7,"La cantidad se actualiza correctamente");
	hash_destruir(hash);

}

int main()
{
	pa2m_nuevo_grupo(
		"\n======================== XXX ========================");
	//prueba_insertado_y_busqueda();
	pruebas_varias();
	prueba_iterador();


	return pa2m_mostrar_reporte();
}
