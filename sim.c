#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

/*
 Estructuras:
 - page_table: mapea número de página virtual -> marco físico (-1 = no mapeada)
 - frames: cada marco físico guarda: referencia a qué página virtual
 - uso[]: bits de uso (algoritmo de reloj)
*/

int main(int argc, char *argv[]) {

    if (argc < 4) {
        printf("Uso: ./sim Nmarcos tamañoMarco [--verbose] traza.txt\n");
        return 1;
    }

    int Nmarcos = atoi(argv[1]);
    int PAGE_SIZE = atoi(argv[2]);

    int verbose = 0;
    char *filename;

    if (argc == 4) {
        filename = argv[3];
    } else if (argc == 5) {
        if (strcmp(argv[3], "--verbose") == 0) {
            verbose = 1;
            filename = argv[4];
        } else {
            printf("Parámetro desconocido: %s\n", argv[3]);
            return 1;
        }
    } else {
        printf("Cantidad de argumentos inválida.\n");
        return 1;
    }

    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("No se puede abrir el archivo %s\n", filename);
        return 1;
    }

    int b = 0;
    int aux = PAGE_SIZE;
    while (aux > 1) { aux >>= 1; b++; }

    uint64_t MASK = PAGE_SIZE - 1;

    int *page_table = malloc(sizeof(int) * 1000000);
    int i;
    for (i = 0; i < 1000000; i++)
        page_table[i] = -1;

    int *frames = malloc(sizeof(int) * Nmarcos);
    int *uso = malloc(sizeof(int) * Nmarcos);
    for (i = 0; i < Nmarcos; i++) {
        frames[i] = -1;
        uso[i] = 0;
    }

    int reloj = 0;
    int marcos_libres = Nmarcos;

    uint64_t DV;
    char buffer[256];

    long referencias = 0;
    long fallos = 0;

    while (fgets(buffer, sizeof(buffer), fp)) {

        if (buffer[0] == '\n' || buffer[0] == '#')
            continue;

        for (i = 0; buffer[i]; i++)
            buffer[i] = (char)tolower(buffer[i]);

        if (strstr(buffer, "0x") != NULL)
            DV = strtoull(buffer, NULL, 16);
        else
            DV = strtoull(buffer, NULL, 10);

        referencias++;

        uint64_t offset = DV & MASK;
        uint64_t nvp = DV >> b;

        int marco = page_table[nvp];

        if (marco != -1) {
            uso[marco] = 1;

            uint64_t DF = ((uint64_t)marco << b) | offset;

            if (verbose) {
                printf("DV=%llx  nvp=%llu  offset=%llu  HIT  marco=%d  DF=%llx\n",
                       (unsigned long long)DV,
                       (unsigned long long)nvp,
                       (unsigned long long)offset,
                       marco,
                       (unsigned long long)DF);
            }

            continue;
        }

        fallos++;

        int marco_usado;

        if (marcos_libres > 0) {
            marco_usado = Nmarcos - marcos_libres;
            marcos_libres--;

        } else {
            while (uso[reloj] == 1) {
                uso[reloj] = 0;
                reloj = (reloj + 1) % Nmarcos;
            }

            marco_usado = reloj;

            if (frames[marco_usado] != -1) {
                int old_page = frames[marco_usado];
                page_table[old_page] = -1;
            }
            reloj = (reloj + 1) % Nmarcos;
        }

        frames[marco_usado] = nvp;
        page_table[nvp] = marco_usado;
        uso[marco_usado] = 1;

        uint64_t DF = ((uint64_t)marco_usado << b) | offset;

        if (verbose) {
            printf("DV=%llx  nvp=%llu  offset=%llu  FALLO  marco=%d  DF=%llx\n",
                   (unsigned long long)DV,
                   (unsigned long long)nvp,
                   (unsigned long long)offset,
                   marco_usado,
                   (unsigned long long)DF);
        }
    }

    fclose(fp);

    printf("\nTotales:\n");
    printf("Referencias: %ld\n", referencias);
    printf("Fallos de página: %ld\n", fallos);
    printf("Tasa de fallos: %.4f\n",
           referencias > 0 ? (double)fallos / referencias : 0.0);

    free(page_table);
    free(frames);
    free(uso);

    return 0;
}
