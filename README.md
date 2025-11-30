# Tarea 2 Sistemas Operativos

## Parte I. Sincronización con Barrera reutilizable

### Complilación
Para compilar el programa, ejecuta el siguiente comando en tu terminal:
```bash
gcc main.c -o main -pthread
```

### Ejecución
El programa se ejecuta con el siguiente formato:
```bash
./main [num_hebras] [num_etapas]
```

#### Argumentos:
- `[num_hebras]`: Número entero positivo de hebras que se van a utilizar. Si no se declara, su valor será 5.
- `[num_etapas]`: Número entero positivo de etapas en las que las hebras trabajarán. Si no se declara, su valor será 4.

## Parte II. Simulador simple de Memoria Virtual

### Compilación
Para compilar el programa, ejecuta el siguiente comando en tu terminal:
```bash
gcc sim.c -o sim
```

### Ejecución
El programa se ejecuta con el siguiente formato:
```bash
./sim [num_marcos] [tamaño_marco] [--verbose] traza.txt
```

#### Argumentos:
- `[num_marcos]`: Número entero positivo que indica cuántos marcos físicos tendrá la simulación.
- `[tamaño_marco]`: Tamaño del marco en bytes. Debe ser una potencia de 2.
- `[--verbose]`: (Opcional) Si se declara, muestra el detalle completo de cada traducción realizada por el simulador.
- `traza.txt`: Archivo de texto con direcciones virtuales, una por línea, en formato decimal o hexadecimal (`0x...`).


