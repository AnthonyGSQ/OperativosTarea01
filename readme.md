# Tarea01 Sistema de archivos indexados para dibujos ascii

## Integrantes

### Carlos A. Obando López C35655
### Geiner Montoya Barrientos C25063
### Andres Camacho C01544
### Anthony Sanchez C37331


## Preguntas a responder

### 1. ¿Cuáles serían los metadatos del filesystem?
Los atributos globales que definen el sistema (almacenados en el **bloque 0**):
- Tamaño del bloque (256 bytes).
- Número total de bloques.
- Número de bloques libres/ocupados.
- Bitmap que con 1 indica si dicho bloque esta en uso, 0 significa que esta libre
- Ubicación de estructuras clave (root directory, tabla de inodes).
- Nombre/versión del FS.

---

### 2. ¿Cuáles serían los metadatos de un archivo?
Los atributos de cada archivo (se almacenan en un **inode**):
- Nombre del archivo.
- Tamaño total (en bytes).
- Tipo (archivo normal o carpeta).
- Permisos (lectura, escritura, ejecución).
- Propietario/usuario.
- Fechas (creación, modificación, acceso).
- Lista de punteros a bloques de datos.

---

### 3. ¿Quiero poder direccionar bloques de datos directamente?

Si debido a que a la hora de unicamente querer acceder a un dibujo ascii, no hay necesidad de detenerse a mirar bloques que no sean de interes para el dibujo solicitado, vease bloques nodos de otras figuras por ejemplo o bloques de datos no pertenecientes a la figura solicitada.

---

### 4. Quiero poder hacer que los archivos sean más grandes <- Entonces el nodo del índice qué tendría para esto? Cómo se hace para que un archivo pueda ser más grande?

Primeramente si, es indispensable que un archivo pueda ser mas grande en caso de que el contenido de este sea expandido bien sea por inputs dados por el usuario o la manipulacion del mismo. En este caso el nodo indice deberia buscar bloques disponibles para la asignacion de los mismos para el contenido del archivo. Para que un archivo pueda ser mas grande existen dos alternativas para este sistema de archivos:

- Aumentar el tamaño de cada bloque de 256 a uno mayor
- Asignar mas bloques de datos al dibujo ascii que necesita dicha expancion de espacio

### 5. Cómo se implementan los folders? <- Donde se guarda el hecho de que un folder es un folder?

Un folder no es mas que un archivo especial indicado como directorio en sus meta-datos, dicho archivo especial directorio contiene una lista de nombres de archivos asociados a los punteros de sus respectivos nodos bloque.


### 6. Cómo se mantiene el control de permisos de acceso a archivos?

Cada archivo contiene en sus meta-datos sus respectivos permisos, por temas de simplicidad para esta tarea dichos permisos seran:
- 1 para solo lectura
- 2 para solo escritura

### 7. Cómo se controla que no haya errores en el funcionamiento general del file system?

El control de errores recae en los bloques de meta-datos, directorios y bloques nodo, esto porque primeramente si se desea ingresar un nuevo dibujo, a travez del bitmap se podria verificar si existen bloques suficientes para almacenar el dibujo por completo, en caso de no existir suficiente espacio, se informa del problema al usuario. Si el usuario solicita un dibujo inexistente, se puede verificar rapidamente mediante el bloque directorio.
Con respecto a la ubicacion de los dibujos, se accede primeramente al bloque nodo correspondiente para saber que bloques son los que contienen el dibujo solicitado, evitando asi mostrar dibujos no solicitados por el usuario o combinaciones de dibujos ascii.
Por ultimo las funciones insertDraw y deleteDraw en caso de lograr su objetivo, informan al sistema de archivos para que el bitmap del bloque meta-datos sea modificado, el bloque directorio agregue/elimine los datos de dicho dibujo y que el bloque nodo correspondiente sea creado/eliminado correctamente.

### 8. Cómo se pueden modificar los metadatados de un archivo?

Para el sistema de archivos de esta tarea, los meta-datos pueden ser modificados mediante la insercion y borrado de los dibujos ascii.
Al modificar dibujos, puede cambiar la fecha de creacion, dueño, ubicacion de los bloques de datos, bitmap del sistema, datos del bloque directorio, etc.

### 9. Cómo se puede conocer el tamaño de un archivo?

Debido a que cada bloque contiene un total de 256 bytes, para conocer el tamaño de un archivo, bastaria con contar cuantos bloques de datos le corresponden y multiplicar dicha cantidad por 256.

### 10. Qué estructuras de datos son indispensables?

Las estructuras indispensables serian:
- Bloques: Para particionar, almacenar y mantener organizado cosas como los meta-datos de archivos/directorios, ubicacion del contenido del sistema de archivos y el contenido de los archivos (dibujos ascii).
- Bitmap: Estructura indispensable para saber en todo momento que bloques estan libres/ocupados
- Tabla de inodos: Donde se almacenan los meta-datos de cada dibujo ascii guardado en el sistema de archivos.