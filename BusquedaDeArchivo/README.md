# Laboratorio 2 - Busqueda de archivos
### Elaborado por:<br>Juan Manuel Ante (juanantec@unicauca.edu.co)<br>Darwin Estiben Latorre (delatorre@unicauca.edu.co)

- Link GitHub: (`https://github.com/yuan4101/labSistemasOperativos/tree/main/Busqueda_De_Archivo`)

- Planteamiento del problema<br>
Deberá desarrollar un programa que, mediante llamada a los servicios del sistema
y las funciones de la biblioteca estándar de C, permita buscar un archivo (o
directorio) dentro de una estructura de directorios del sistema. El programa deberá
recibir por línea de comandos dos parámetros: El directorio base de la búsqueda,
y el patrón (el texto) que debe encontrarse en el nombre del archivo o directorio.
Algunas funciones de la biblioteca estándar de C y/o POSIX que se pueden usar son:<br>

- stat
- opendir
- readdir
- malloc
- strstr
- realpath
