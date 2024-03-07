#include <iostream>
#include <thread>
#include <fstream>
#include <math.h>
#include <string.h>
#include <chrono>

using namespace std;

// Número máximo de threads
const int MAX_THREADS = 100;

/**
 * @brief Calcula o número de ocorrências de uma palavra em um texto.
 *
 * Esta função calcula o número de ocorrências de uma palavra em um texto dados os limites de início e fim.
 *
 * @param text O texto onde a palavra será buscada.
 * @param word A palavra a ser buscada.
 * @param start O índice de início da busca.
 * @param end O índice de fim da busca.
 * @return O número de ocorrências da palavra no texto.
 */
int wordCount(string *text, const char *word, int start, int end) {
    int count = 0;
    int pos = start;
    while ((pos = (*text).find(word, pos)) != string::npos and pos < end) {
        pos += strlen(word);
        count++;
    }
    return count;
}

/**
 * @brief Função que calcula o número de ocorrências de um conjunto de palavras em um texto.
 *
 * Esta função é responsável por calcular o número de ocorrências de um conjunto de palavras em um texto dado os limites de início e fim. O resultado é armazenado em um vetor de inteiros.
 *
 * @param text O texto onde a palavra será buscada.
 * @param words O conjunto de palavras a serem buscadas.
 * @param n O número de palavras a serem buscadas.
 * @param start O índice de início da busca.
 * @param end O índice de fim da busca.
 * @param results O vetor onde os resultados serão armazenados.
 */
void wordsCount(string *text, const char *const *words, const int n, int start, int end, int *results) {
    for (int i = 0; i < n; i++) {
        results[i] += wordCount(text, words[i], start, end);
    }
}

int main() {
    // Obter o caminho do arquivo
    string filePath;
    cout << "Insira o endereço do arquivo: ";
    cin >> filePath;

    const auto start = chrono::high_resolution_clock::now();
    // Ler o arquivo
    ifstream file (filePath);
    if (!file.is_open()) {
        cout << "Error: file not found" << endl;
        return 1;
    }
    string text(
        (istreambuf_iterator<char>(file)),
        istreambuf_iterator<char>()
    );
    file.close();

    const auto readDuration = chrono::high_resolution_clock::now() - start;
    const int readMs = chrono::duration_cast<chrono::milliseconds>(readDuration).count();

    // Palavras a serem buscadas
    constexpr const char *words[] = {"love", "hate"};
    constexpr const int nElements = size(words);

    // Array para armazenar os resultados
    int results[nElements];

    // Array de threads
    thread t[MAX_THREADS];

    ofstream resultsFile("results.csv");
    resultsFile << "threads,blockSize,readTime,searchTime,totalTime" << endl;

    for (int threadNumber = 1; threadNumber <= MAX_THREADS; threadNumber++) {
        // Inicializa o vetor de resultados
        for (int i = 0; i < nElements; i++) {
            results[i] = 0;
        }

        // Calcula o tamanho do bloco
        float blockSize = text.length() / (float) threadNumber;

        // Inicializa as variáveis de início e fim
        int start;
        int end = 0;

        const auto startSearch = chrono::high_resolution_clock::now();

        for (int i = 0; i < threadNumber; i++) {
            // O início do bloco é o final do bloco anterior
            start = end;
            // Calcula o final do bloco
            end = round((i + 1) * blockSize);

            // Criar threads
            t[i] = thread(
                wordsCount,
                &text,
                words,
                nElements,
                start,
                end,
                results
            );
        }

        // Aguardar as threads terminarem
        for (int i = 0; i < threadNumber; i++) {
            t[i].join();
        }

        const auto searchDuration = chrono::high_resolution_clock::now() - startSearch;
        const auto totalTime = readDuration + searchDuration;

        const int searchMs = chrono::duration_cast<chrono::milliseconds>(searchDuration).count();
        const int totalMs = chrono::duration_cast<chrono::milliseconds>(totalTime).count();

        resultsFile << fixed << threadNumber << "," << blockSize << "," << readMs << "," << searchMs << "," << totalMs << endl;

        // Imprimir os resultados
        cout << "Número de threads: " << threadNumber << endl;
        // fixed para imprimir sem notação científica
        cout << fixed << "Tamanho médio do bloco: " << blockSize << endl;
        cout << "Tempo de leitura: " << readMs << "ms" << endl;
        cout << "Tempo de busca: " << searchMs << "ms" << endl;
        cout << "Tempo total: " << totalMs << "ms" << endl;

        int max = 0;
        char *word;
        for (int i = 0; i < nElements; i++) {
            cout << "A palavra '" << words[i] << "' aparece " << results[i] << " vezes" << endl;
            if (results[i] > max) {
                max = results[i];
                word = (char *) words[i];
            }
        }
        
        cout << "A palavra que aparece mais vezes é '" << word << "'" << endl;
        cout << endl;
    }
}