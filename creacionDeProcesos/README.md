# Laboratorio 4 - Creacion de procesos
### Elaborado por:<br>Juan Manuel Ante (juanantec@unicauca.edu.co)<br>Darwin Estiben Latorre (delatorre@unicauca.edu.co)

- Planteamiento del problema<br>
Desarrolle un programa que mediante las llamadas al sistema fork, exec, 
wait y exit permita la creación de un mini-shell.<br>

El programa deberá validar las condiciones de error de cada una de las
llamadas al sistema que se usan. Éstas se pueden consultar en las páginas del
manual.<br>

Se recomienda usar la función execvp, la cual toma como parámetro una
cadena de caracteres y un arreglo de cadenas.