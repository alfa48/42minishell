#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  

// Funções auxiliares (implementar como necessário)  
char **ft_extra_split(const char *str);  
char *ft_get_env(const char *var);  

char *concat_strings(char **str_array) {
    // Concatena todas as strings do array em uma única string
    size_t total_length = 0;
    int i = 0;

    // Calcula o comprimento total necessário, considerando espaços entre as strings
    while (str_array[i]) {
        total_length += strlen(str_array[i]);
        i++;
    }

    // Adiciona espaços entre as strings, exceto após a última
    total_length += (i - 1); // Adiciona (n-1) espaços, onde n é o número total de strings

    // Aloca memória para a string concatenada
    char *result = malloc((total_length + 1) * sizeof(char)); // +1 para o caractere nulo
    if (!result) {
        perror("Memória insuficiente");
        exit(1); // Sair em caso de erro
    }

    // Copia as strings e adiciona espaços entre elas
    result[0] = '\0'; // Inicializa a string resultante
    for (int j = 0; str_array[j]; j++) {
        strcat(result, str_array[j]); // Concatena a string
        if (str_array[j + 1]) {
            strcat(result, " "); // Adiciona espaço entre as strings
        }
    }

    return result;
}

char **IDN(char *str) {  
    char **out;  
    char *tmp;  
    char *env;  
    int i = 0;  

    out = ft_extra_split(str);  
    if (out[i]) {  
        while (out[i]) {  
            if (out[i][0] == '$') {  
                env = ft_get_env(out[i]);  
                if (env) {  
                    tmp = out[i];  
                    out[i] = env;  
                    free(tmp);  
                }  
            }  
            i++;  
        }
       	// Libera a matriz de ponteiros  
    } else {  
        printf("PAU\n");  
    }
    return (out);
}  

// Função para dividir a string mantendo os espaços  
char **ft_extra_split(const char *str) {  
    size_t len = strlen(str);  
    char **result = malloc((len + 1) * sizeof(char*)); // Aloca espaço suficiente  
    if (!result) {  
        perror("Memória insuficiente");  
        exit(1);  
    }  

    int idx = 0;  
    const char *start = str;  
    
    while (*start) {  
        // Encontra o próximo espaço  
        const char *end = start;  
        while (*end && *end != ' ') {  
            end++;  
        }  

        // Se 'start' é diferente de 'end', significa que encontramos uma palavra ou espaços  
        if (end > start) {  
            // Aloca espaço para a substring (inclui os espaços)  
            size_t length = end - start;  
            result[idx] = malloc((length + 1) * sizeof(char)); // +1 para o terminador nulo  
        
            if (!result[idx]) {  
                perror("Memória insuficiente");  
                exit(1);  
            }  

            // Copia a parte da string  
            strncpy(result[idx], start, length);  
            result[idx][length] = '\0'; // Adiciona o terminador nulo  
            idx++;  
        } else { // Se encontrarmos um espaço, tratamos igualmente  
            result[idx] = strdup(" "); // Armazena o espaço como uma string  
            idx++;  
        }  

        start = end; // Move o ponteiro 'start' para o próximo caractere após o espaço  
    }  

    result[idx] = NULL; // Marca o final do array  
    return result;  
}

char *ft_get_env(const char *var) {  
    // Simulação de uma função que obtem o valor de uma variável de ambiente  
    // Aqui você deve implementar a lógica para pegar a variável do ambiente real  
    if (strcmp(var, "$USER") == 0) {  
        return strdup("usuario"); // Retorna um valor fictício para $USER  
    }  
    return NULL; // Retorna NULL se não encontrar a variável  
}  

int main() {  
    char input[256];  

    printf("Digite uma string: ");  
    fgets(input, sizeof(input), stdin);  
    input[strcspn(input, "\n")] = 0; // Remove nova linha  

   printf("%s", concat_strings(IDN(input)));

    return 0;  
}
