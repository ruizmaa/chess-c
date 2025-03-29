#!/bin/bash

DIRECTORIO="."

for fichero in "$DIRECTORIO"/*; do
    if [[ -f "$fichero" ]]; then
        echo "=== Contenido de: $fichero ==="
        cat "$fichero"
        echo -e "\n"
    fi
done
