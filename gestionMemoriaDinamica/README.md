# Tercer Parcial - Gestion de memoria dinamica
### Elaborado por:<br>Juan Manuel Ante (juanantec@unicauca.edu.co)<br>Darwin Estiben Latorre (delatorre@unicauca.edu.co)

- Planteamiento del problema<br>
La gestión de memoria dinámica es uno de los servicios críticos que un sistema operativo debe
ofrecer a los programas. Dentro de esta categoría de servicios podemos tener un gestor de
memoria especializado, que permite obtener unidades de un tamaño de bytes fijo, potencia de
2 (1, 2, 4, 16, 32, 64, ... etc.) de un “kbuf” de ítems. De esta forma, cuando un usuario requiere
un ítem de, por ejemplo, 1 byte, el sistema asigna una unidad dentro del conjunto de ítems
disponibles. Debido que en una arquitectura de 32 bits es más fácil alinear la memoria a límites
de 4 bytes (32 bits), cuando se solicita un byte, realmente se asignan 4 bytes.<br>