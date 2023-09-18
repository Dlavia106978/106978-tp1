<div align="right">
<img width="32px" src="img/algo2.svg">
</div>

# TP1

## Repositorio de (Nombre Apellido) - (Padrón) - (Mail)
DIEGO LAVIA - 106978 - diegolavia12@hotmail.com

- Para compilar:

```bash
línea de compilación
make
```

- Para ejecutar:

```bash
línea de ejecución
```

- Para ejecutar con valgrind:
```bash
línea con valgrind
```
---
##  Funcionamiento

Explicación de cómo funcionan las estructuras desarrolladas en el TP y el funcionamiento general del mismo.

Aclarar en esta parte todas las decisiones que se tomaron al realizar el TP, cosas que no se aclaren en el enunciado, fragmentos de código que necesiten explicación extra, etc.



Incluír **EN TODOS LOS TPS** los diagramas relevantes al problema (mayormente diagramas de memoria para explicar las estructuras, pero se pueden utilizar otros diagramas si es necesario).

### Por ejemplo:
Mi programa funciona abriendo el archivo y lo leo linea por linea con un while, dentro de este reservo memoria para info_pokemon con maloc (heap, memoria dinamica) e inicializo la cantidad de pokemones en cero, dentro del while llamo a una funcion auxiliar contar_delimitador, esta funcion va a recorrer la linea y devolver la cantidad de ; que encontre, si es uno creo un pokemon, si es dos cargo el ataque. tanto en crear pokemon, como en cargar ataque reservo memoria con malloc, esta sera liberada al termino de cada funcion. 
En la funcion delimitador tambien busca si hay ":" esto estaria mal, pero no encontre otra forma de hacerlo. 
El programa funciona abriendo el archivo pasado como parámetro y leyendolo línea por línea. Por cada línea crea un registro e intenta agregarlo al vector. La función de lectura intenta leer todo el archivo o hasta encontrar el primer error. Devuelve un vector con todos los registros creados.

stack: struct pokemon y struct info_pokemon
heap: lo uso en pokemon_destruir y cargar_ataque (asigno memoria dinamica para struct ataque y luego la libero), en crear pokemon hago lo mismo pera esta vez con struct pokemon
en pokemon_cargar_archivo tambien lo hago con informacion_pokemon_t

<div align="center">
<img width="70%" src="img/diagrama1.svg">
</div>

En el archivo `sarasa.c` la función `funcion1` utiliza `realloc` para agrandar la zona de memoria utilizada para conquistar el mundo. El resultado de `realloc` lo guardo en una variable auxiliar para no perder el puntero original en caso de error:

```c
int *vector = realloc(vector_original, (n+1)*sizeof(int));

if(vector == NULL)
    return -1;
vector_original = vector;
```


<div align="center">
<img width="70%" src="img/diagrama2.svg">
</div>

---

## Respuestas a las preguntas teóricas
Explicar cómo se logra que los pokemon queden ordenados alfabéticamente y cuál es el costo computacional de esta operación.
el metodo que use para ordenar los pokemones alfabeticamente fue el metodo de burbujero, este algoritmo
compara elementos uno por uno, comenzando desde el principio, intercambiandia 2 elementos si no estan ordenados alfabeticamente, se intercambian para que esten en el orden correcto, esto se repite hasta que queden todos los pokemones ordenados alfabeticamente. Los pokemones se guardan el info_pokemon por orden alfabetico por nombre. Use un for aparte para saber la cantidad aplicada, este for es linea
El costo de memoria es cuadratico, dado que se usan dos for y su grafico es una parabola. 

Explicar con diagramas la disposición de los diferentes elementos en memoria para las diferentes operaciones implementadas.
Incluír acá las respuestas a las preguntas del enunciado (si aplica).
