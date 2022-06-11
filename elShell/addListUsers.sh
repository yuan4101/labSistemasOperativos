varAchivo="$1"
while read varLinea
do

    IFS=';'
    read -a varIndex <<<"$varLinea"

    varNombre="${varIndex[0]}"
    varApellido="${varIndex[1]}"

    varShell="/bin/bash"
    varNombreCompleto="$varNombre $varApellido"

    varLogin="${varIndex[2]}"
    varHome="/home/$varLogin"

    varPassword="${varIndex[3]}"

    sudo useradd -m -d "$varHome" -s "$varShell" -c "$varNombreCompleto" -p "$varPassword" $varLogin

done < $varAchivo