# Primer parcial - Sistema de Control de Versiones
### Elaborado por:<br>Juan Manuel Ante (juanantec@unicauca.edu.co)<br>Darwin Estiben Latorre (delatorre@unicauca.edu.co)

- Link GitHub: (`https://github.com/yuan4101/labSistemasOperativos/tree/main/parcial_1_delatorrre_juanantec`)

Los Sistemas de Control de Versiones (VCS) permiten guardar el rastro de las modificaciones sobre determinados elementos.
En el contexto de este examen, se gestionarán versiones de archivos y directorios.
Se deberá implementar un sistema de control de versiones simple, que permita:

- Adicionar un archivo al repositorio de versiones.
- Listar las versiones de un archivo en el repositorio de versiones.
- Obtener la versión de un archivo del repositorio de versiones. <br>

En esta implementación sólo se deberá realizar el control de versiones por directorio,
en el cual sólo se pueden agregar archivos que se encuentren en el directorio actual. <br>

La aplicación implementada se deberá llamar versions. Su uso será el siguiente: <br>

$ ./versions <br>
Uso: versions add archivo "Comentario" <br>
     versions list archivo <br>
     versions get numver archivo <br>
     
## Repositorio de versiones
Al invocar el comando ./versions, se deberá crear un sub-directorio llamado .versions, si no existe, en el directorio actual de trabajo.
Dentro de este directorio se almacenarán las diferentes versiones del archivo, y un archivo llamado versions.db
que contendrá la descripción de cada versión. <br>

Al invocar el comando: <br>
  versions add test.c "Primera version" <br>
  
Se deberá verificar si la versión actual del archivo es diferente a las anteriores versiones guardadas.
Si se cumple esta condición, se creará la nueva versión del archivo en el sub-directorio
y se añadirá una línea al archivo versions.db. <br>

Tenga en cuenta que la función adicionar_version adiciona un nuevo registro al archivo "versions.db".
Esto se puede implementar de dos maneras: <br>
- Leyendo el contenido completo del archivo, almacenarlo en una estructura de datos temporal, adicionar el nuevo registro al final y escribir de nuevo todo el contenido del archivo.
- Abrir el archivo "versions.db.en modo .adicionar"(append), y escribir únicamente el nuevo registro. <br>

## Listado de versiones
El comando: <br>
  versions list test.c <br>
  
Deberá imprimir la información que se encuentra dentro del archivo versions.db del sub-directorio correspondiente para el archivo. <br>

## Obtener una version del archivo
El comando: <br>
  ./versions get 1 test.c <br>
  
Obtendrá la versión solicitada del archivo del repositorio, sobreescribiéndolo si éste ya existe.
