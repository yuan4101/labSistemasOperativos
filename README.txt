Laboratorio 2 - Busqueda de archivos
Elaborado por: Juanantec - Delatorre

2. Planteamiento del problema
Deberá desarrollar un programa que, mediante llamada a los servicios del sistema
y las funciones de la biblioteca estándar de C, permita buscar un archivo (o
directorio) dentro de una estructura de directorios del sistema. El programa deberá
recibir por línea de comandos dos parámetros: El directorio base de la búsqueda,
y el patrón (el texto) que debe encontrarse en el nombre del archivo o directorio.
Algunas funciones de la biblioteca estándar de C y/o POSIX que se pueden usar
son:

• stat
• opendir
• readdir
• malloc
• strstr
• realpath