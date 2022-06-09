# Laboratorio 3 - El Shell
### Elaborado por:<br>Juan Manuel Ante (juanantec@unicauca.edu.co)<br>Darwin Estiben Latorre (delatorre@unicauca.edu.co)

- Link GitHub: (`https://github.com/yuan4101/labSistemasOperativos/tree/main/El_Shell`)

- Planteamiento del problema<br>
1. Suponga que se tiene una gran cantidad de videos en formato “MKV”.
Se necesita subir estos videos a Google Drive, pero este sitio acepta
preferentemente archivos en formato MP4.<br>Construya un script de shell que
tome todos los videos MKV en el directorio actual, y mediante el comando
“ffmpeg”, realice la conversión a MP4.<br>Para probar deberá descargar el
paquete ffmpeg de los repositorios oficiales de Debian.<br>
2. Se tiene un archivo de texto con líneas que tienen el siguiente formato: nombres;apellidos;login;password.<br>
Se requiere construir un script de shell que reciba como parámetro el nombre
del archivo de texto, y cree los usuarios con los datos especificados. El script
sólo deberá crear los usuarios que no existan.