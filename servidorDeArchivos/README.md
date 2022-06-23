# Parcial 2 - Servidor de archivos
### Elaborado por:<br>Juan Manuel Ante (juanantec@unicauca.edu.co)<br>Darwin Estiben Latorre (delatorre@unicauca.edu.co)


### Planteamiento del problema<br>
Se deberá desarrollar dos programas en C que implementen la funcionalidad para enviar y recibir archivos a través de la red. Uno de los dos extremos actuará como cliente, y el otro como servidor. Quien recibe conexiones (el servidor) esperará por comandos del cliente, para transferir archivos desde o hacia el servidor.<br><br>

Los archivos en el servidor se almacenarán en un directorio llamado "files".
Los comandos soportados son:<br><br>

- get ARCHIVO: Transfiere un archivo desde el servidor hacia el cliente.
El cliente deberá enviar un primer mensaje al servidor con la solicitud
(el nombre del archivo a transferir), a lo cual el servidor responderá con un
mensaje que contiene la información del archivo (por ejemplo, el tamaño en bytes),
seguido del contenido del archivo. Si el archivo no existe,
se deberá informar al cliente el error, y no se enviará nada a continuaciónde este mensaje.<br><br>

- put ARCHIVO: Transfiere un archivo desde el cliente hacia el servidor.
El cliente deberá enviar un mensaje al servidor con la solicitud (el nombre
del archivo a transferir y su tamaño en bytes), seguido del contenido del
archivo. El servidor recibirá el mensaje, y con la información suministrada, creará el archivo, leerá el contenido del socket y lo escribirá en el archivo (nuevo o existente).<br><br>

- exit: El cliente deberá enviar un mensaje al servidor indicando que se va a cerrar la conexión.<br><br>
Se debe permitir la comunicación entre un servidor y múltiples clientes,
por lo cual en el servidor se deberá crear y administrar múltiples hilos,
uno por cada conexión. Se debe tener en cuenta que varios clientes pueden
tratar de enviar el mismo archivo al servidor. Si se presenta esta situación,
el servidor deberá rechazar el envío.<br><br>

Tanto el servidor como el cliente deberán terminar cuando se reciba la señal SIGTERM.