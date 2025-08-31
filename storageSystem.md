# Guía del Sistema de Almacenamiento de Dibujos ASCII (SADA)

## Descripción


Este sistema permite almacenar dibujos ASCII. El usuario puede:
- Crear el sistema con un tamaño inicial personalizado.
- Agregar dibujos ASCII indicando el nombre y el contenido del dibujo.
- Si el dibujo es muy grande, se particiona en varios bloques.
- Eliminar dibujos existentes.
- Listar todos los dibujos almacenados.
- Visualizar cualquier dibujo guardado.


## Estructura del sistema


El sistema está compuesto por cuatro tipos de bloques:

### 1. MetaData Block

Contiene información global del sistema, como:
- Tamaño total del almacenamiento
- Cantidad de bloques disponibles y usados
- Fecha de creación
- Versión del sistema
- **Bitmap de bloques:** Un arreglo de bits que indica qué bloques están ocupados y cuáles están libres, permitiendo una gestión eficiente del espacio.
- **Tamaño de los bloques:** El tamaño fijo de cada bloque en el sistema.
- **Cantidad total de bloques:** Número total de bloques disponibles en el sistema.

### 2. Directory Block

Almacena la lista de los bloques nodos y metadatos de la "carpeta". Cada entrada incluye:
- Nombre del dibujo
- Referencia(s) al bloque nodo correspondiente
- Fecha de creación/modificación

### 3. Node Block

Actúa como índice para un dibujo. Contiene:
- Ubicación (números de bloque) de los bloques de datos que almacenan el dibujo respectivo. Por ejemplo, el nodo del dibujo "gato" puede tener: ubicación: 5,6,7
- Tamaño del dibujo
- Permisos o atributos adicionales

### 4. Data Block
Almacena el contenido real del dibujo ASCII. Si el dibujo es grande, se divide en varios bloques de datos.

---

## Ejemplo de flujo de uso

1. El usuario crea el sistema con un tamaño inicial.
2. Agrega un dibujo llamado "gato". El sistema lo almacena y lo referencia en el bloque directorio.
3. El usuario puede listar los dibujos:
	 - Salida: `gato, perro, casa`
4. El usuario elimina el dibujo "perro". El sistema libera los bloques asociados.
5. El usuario visualiza "gato" y el sistema reconstruye el dibujo usando los bloques de datos.

---

## Comandos básicos

- **Crear sistema:**
	`crear_sistema <tamaño>`
- **Agregar dibujo:**
	`agregar_dibujo <nombre>`
- **Eliminar dibujo:**
	`eliminar_dibujo <nombre>`
- **Listar dibujos:**
	`listar_dibujos`
- **Visualizar dibujo:**
	`ver_dibujo <nombre>`

---

## Ejemplo de estructura interna (simplificada)

```text
MetaData Block
----------------
Tamaño: 1024 bloques
Usados: 10 bloques
Fecha: 2025-08-31

Directory Block
----------------
gato -> nodo_1
perro -> nodo_2

Node Block (nodo_1)
-------------------
Bloques de datos: [data_5, data_6]
Tamaño: 120 bytes

Data Block (data_5)
-------------------
" /\_/\  "
"( o.o ) "
" > ^ <  "
```

---

## Notas
- El sistema está diseñado para ser eficiente y escalable.
- Los bloques pueden crecer según la necesidad del usuario.
- Se recomienda hacer respaldos periódicos.

