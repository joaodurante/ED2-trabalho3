/***************************************************************************************************
* @title Trabalho Aula 8 ED2
* @version 1.0
* @author Guilherme Krambeck
* @author João Pedro Durante
 ***************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

/********************************* CONSTANTS *********************************/

#define BIBLIOTECA_FILENAME "resources/biblioteca.bin"
#define BUSCA_FILENAME "resources/busca.bin"
#define CONSULTA_CASADA_FILENAME "resources/consulta_casada.bin"
#define REMOVE_FILENAME "resources/remove.bin"
#define BTREE_FILENAME "btree.bin"
#define REGISTERS_FILENAME "registers.bin"
#define MERGE_MATCH_FILE_FILENAME "merge_match_file.bin"
#define CONSULTA_CASADA_KEY_1_FILENAME "consulta_casada_key1.bin"
#define CONSULTA_CASADA_KEY_2_FILENAME "consulta_casada_key2.bin"
#define WRITE_MODE "r+b"
#define CREATE_WRITE_MODE "w+b"
#define READ_MODE "rb"
#define VECTOR_SIZE 100
#define ISBN_LENGTH 14
#define NO_KEY "@@@@@@@@@@@@@"
#define MAX_KEYS 3
#define MIN_KEYS 1
#define NIL -1
#define HEADER_SIZE sizeof(int)
#define BIBLIOTECA_TEST_INDEX 1
#define BUSCA_TEST_INDEX 2
#define CONSULTA_CASADA_TEST_INDEX 3

/********************************* REGISTERS *********************************/
struct book {
    char isbn[ISBN_LENGTH];
    char titulo[50];
    char autor[50];
    char ano[5];
};
typedef struct book Book;

struct testIndex {
	int biblioteca;
	int busca;
	int consultaCasada;
};
typedef struct testIndex TestIndex;

struct isbnList {
    char code[ISBN_LENGTH];
	int offset;
};
typedef struct isbnList IsbnList;

struct consultaCasadaList {
    char key1[ISBN_LENGTH];
	char key2[ISBN_LENGTH];
};
typedef struct consultaCasadaList ConsultaCasadaList;

struct btPage{
	int keyCount; // number of keys in page
	IsbnList key[MAX_KEYS]; // the actual keys
	int child[MAX_KEYS+1]; // pointers to rrns of childs
};
typedef struct btPage BTPage;
#define PAGESIZE sizeof(BTPage)

/********************************* PROTOTYPES *********************************/
/**
 * Responsável pela exibicao e distribuicao das operacoes
 */
void menu();

/**
 * Aguarda o usuário pressionar qualquer tecla para
 * continuar a execução do programa
 */
void waitForUserResponse();

/**
 * Efetua a limpeza da tela
 */
void clearScreen();

/**
 * Efetua a abertua do arquivo no modo de abertura informado
 *
 * @param file ponteiro que receberá o arquivo aberto
 * @param filename nome do arquivo a ser aberto
 * @param openMode modo de abertura do arquivo
 * @param createIfNotExists flag para criar o arquivo caso o mesmo não esteja criado ainda
 * @return sucesso ou falha na abertura do arquivo
 */
bool openFile(FILE** file, const char* filename, bool writeMode, bool createIfNotExists);

/**
 * Carrega os arquivos
 *
 * @return verdadeiro se todos os arquivos foram carregados ou falso se falhou para carregar algum arquivo

 */
bool loadFilesToMemory();

/**
 * Inserir novos livros no sistema
 */
void insert();

/**
 * Recupera o RRN do elemento root da árvore B
 *
 * @return RRN do elemento root do arquivo btFile
 */
int getRoot();

/**
 * Cria o elemento root na árvore B
 *
 * @return RRN do elemento root do arquivo btFile
 */
int createRoot(IsbnList *key, int left, int right);

/**
 * Recupera o número da página
 *
 * @return número da página
 */
int getPage();

/**
 * Inicia uma nova página na árvore B
 *
 * @param page Ponteiro onde será criada a nova página
 */
void pageInit(BTPage *page);

/**
 * Escreve a página no rrn do arquivo btFile
 */
void btWrite(int rrn, BTPage *page_ptr);

/**
 * Insere o RRN no elemento root da árvore B
 *
 * @param root RRN a ser colocado no elementp root do arquivo btFile
 */
void putRoot(int root);

/**
 * Insere chave na árvore B
 *
 * @param key IsbnList a ser inserido na árvore B
 * @return verdadeiro se a inserção ocorreu com sucesso ou falso se houve algum erro
 */
bool insertInBTree(IsbnList *key);

/**
 * Função de apoio utilizada na inserção de uma chave na árvore B recursivamente
 *
 * @param rrn
 * @param key
 * @param promoChild
 * @param promoKey
 * @param success verdadeiro se a inserção ocorreu com sucesso ou falso se houve algum erro
 * @return verdadeiro ou falso
 */
bool insertInBTreeRecursively(int rrn, IsbnList *key, int *promoChild, IsbnList *promoKey, bool *success);

/**
 * Split tree
 *
 * @param key
 * @param child
 * @param oldPage
 * @param promoKey
 * @param promoChild
 * @param newPage
 */
void split(IsbnList *key, int child, BTPage *oldPage, IsbnList *promoKey, int *promoChild, BTPage *newPage);

/**
 * Recupera a página de um RRN na árvore B
 *
 * @param rrn RRN da página a ser recuperada
 * @param page variável que receberá a página recuperada
 */
void btRead (int rrn, BTPage *page);

/**
 * Busca um nó dentro de uma página
 *
 * @return true
 */
bool searchKeyInPage(IsbnList *key, BTPage *page, int *pos);

/**
 * Insere um chave dentro de uma página
 *
 * @param key chave que será colocada na página
 * @param child RRN do filho
 * @param page página onde será inserida a chave
 */
void insertKeyInPage(IsbnList *key, int child, BTPage *page);

/**
 * Incrementa o indice de registros no arquivo principal
 * @param file arquivo principal de registros
 * @param index índice atual dos registros
 * @param testIndexControl arquivo que será lido o indice
 */
void incrementNumberOfRegisters(FILE* file, int index, int testIndexControl);

/**
 * Funcao que pega a quantidade de registros escritos no arquivo principal
 * @param file arquivo principal com os registros
 * @param testIndexControl arquivo que será lido o indice
 * @return inteiro com a quantidade de registros  (indice)
 */
int getIndex(FILE* file, int testIndexControl);

/**
 * Funcao recursiva que percorre a arvore para em seguida printar
 * @param rrn da raiz da arvore
 */
void printTree(int rrn);

/**
 * Funcao que printa o registro de acordo com o offset recebido
 * @param offset do registro no arquivo
 */
void printBook(int offset);

/**
 * Funcao que fara a busca do ISBN presente no arquivo de buscas
 */
void searchByISBN();

/**
 * Funcao que buscara e exibira o isbn recebido por parametro de maneira recursiva
 * @param rrn pagina da arvore
 * @param isbn que sera buscado
 */
void searchKey(int rrn, char isbn[]);

/**
 * Funcao que fara a busca dos ISBNs utiizando Consulta Casada presente no arquivo de consulta casada
 */
void findBy2ISBN();

/**
 * Funcao que buscara o isbn recebido por parametro de maneira recursiva
 *
 * @param rrn pagina da arvore
 * @param key1 que sera buscado
 * @param key2 que sera buscado
 */
void findBothKeysAndSaveToFiles(int rrn, const char *key1, const char *key2, FILE *consultaCasadaKey1, FILE *consultaCasadaKey2);

/**
 * Faz o merge dos dois arquivos no arquivo descrito em filename
 *
 * @param file1 arquivo 1
 * @param file1 arquivo 2
 * @param filename nome do arquivo que receberá o merge
 */
void mergeFiles(FILE *file1, FILE* file2, const char *filename);

/**
 * Faz o match dos dois arquivos no arquivo descrito em filename
 *
 * @param file1 arquivo 1
 * @param file1 arquivo 2
 * @param filename nome do arquivo que receberá o match
 */
void matchFiles(FILE *file1, FILE* file2, const char *filename);

/********************************* GLOBAL VARIABLES *********************************/
Book biblioteca[VECTOR_SIZE];
char busca[ISBN_LENGTH][VECTOR_SIZE];
ConsultaCasadaList consultaCasada[VECTOR_SIZE];
FILE *btFile;

/**
 * Init function
 */
int main() {
	menu();
}

void menu(){
	bool allFilesLoaded = false;
    int resp;
    do {
    	clearScreen();
		printf("Menu");
		printf("\n1.Insercao");
		printf("\n2.Listar todos os livros");
		printf("\n3.Listar livro especifico");
		printf("\n4.Consulta casada");
		printf("\n5.Carrega Arquivos");
		printf("\n0.Sair");
		printf("\nEscolha: ");
		scanf("%d", &resp);
		fflush(stdin);
        if(resp != 5 && resp != 0 && !allFilesLoaded){
            printf("\nE preciso carregar todos os arquivos de testes primeiro");
        }else{
            switch(resp){
                case 1:
                    insert();
                    break;
                case 2:
                    if (!openFile(&btFile, BTREE_FILENAME, false, true)){
                        break;
                    }
                    clearScreen();
                    printTree(getRoot());
                    fclose(btFile);
                    break;
                case 3:
                    searchByISBN();
                    break;
                case 4:
                    findBy2ISBN();
                    break;
                case 5:
                    allFilesLoaded = loadFilesToMemory();
                    break;
                case 0:
                    return;
                default: printf("\nEscolha Invalida");
            }
        }
	    waitForUserResponse();
    } while(resp != 0);
}

void insert(){
	FILE *file;
	if(!openFile(&file, REGISTERS_FILENAME, true, true)){
		return;
	}

	int index = getIndex(file, BIBLIOTECA_TEST_INDEX);
	Book book = biblioteca[index];
	if(strlen(book.isbn) == 0){
		printf("\nNao ha mais registros para inserir!");
		fclose(file);
		return;
	}
	IsbnList key;
	memcpy(&key, book.isbn, sizeof(IsbnList));
	fseek(file, 0, SEEK_END);
	key.offset = ftell(file);
	if (insertInBTree(&key)){
		fwrite(&book, sizeof(Book), 1, file);
		incrementNumberOfRegisters(file, index, BIBLIOTECA_TEST_INDEX);
	}
	fclose(file);
}

void incrementNumberOfRegisters(FILE* file, int index, int testIndexControl){
    rewind(file);
	TestIndex testIndex;
	index++;
	fread(&testIndex, sizeof(TestIndex), 1, file);
	switch (testIndexControl){
		case BIBLIOTECA_TEST_INDEX:
			testIndex.biblioteca = index;
			break;
		case BUSCA_TEST_INDEX:
			testIndex.busca = index;
			break;
		case CONSULTA_CASADA_TEST_INDEX:
			testIndex.consultaCasada = index;
			break;
	}
	rewind(file);
    fwrite(&testIndex, sizeof(TestIndex), 1, file);
}

bool insertInBTree(IsbnList *key){
	bool promoted,
		 success = true;
	int root, //RRN of the root page
		  promoRRN; //RRN promoted from below
	IsbnList promoKey; //key promoted from below

	printf("\n\n%s", key->code);
	openFile(&btFile, BTREE_FILENAME, true, true);

	root = getRoot();
	if (root == NIL){
		root = createRoot(key, NIL, NIL);
	}else{
		promoted = insertInBTreeRecursively(root, key, &promoRRN, &promoKey, &success);
		if(promoted){
			root  = createRoot(&promoKey, root, promoRRN);
		}
	}
	if(success){
		printf("\nChave %s inserida com sucesso", key->code);
	}
	fclose(btFile);
	return success;
}

bool insertInBTreeRecursively(int rrn, IsbnList *key, int *promoChild, IsbnList *promoKey, bool *success){
	BTPage page, //current page
		   newPage; //new page created if split occurs
	bool found, promoted;
	int pos,
		belowPromoRRN; //RRN promoted from below
	IsbnList belowPromoKey; //key promoted from below

	if (rrn == NIL){
		memcpy(promoKey, key, sizeof(IsbnList));
		*promoChild = NIL;
		return true;
	}
	btRead(rrn, &page);
	found = searchKeyInPage(key, &page, &pos);
	if(found){
		printf ("\nErro: Chave %s duplicada\n", key->code);
		*success = false;
		return false;
	}
    promoted = insertInBTreeRecursively(page.child[pos], key, &belowPromoRRN, &belowPromoKey, success);
    if (!promoted){
        return false;
    }
    if(page.keyCount < MAX_KEYS){
        insertKeyInPage(&belowPromoKey, belowPromoRRN, &page);
        btWrite(rrn, &page);
        return false;
    }else{
        split(&belowPromoKey, belowPromoRRN, &page, promoKey, promoChild, &newPage);
		printf("\nChave %s promovida", promoKey->code);
        btWrite(rrn, &page);
        btWrite(*promoChild, &newPage);
        return true;
    }
}

void insertKeyInPage(IsbnList *key, int child, BTPage *page){
    int j;
    for(j = page-> keyCount; strcmp(key->code, page->key[j-1].code) < 0 && j > 0; j--) {
        page->key[j] = page->key[j-1];
        page->child[j+1] = page->child[j];
    }
    page->keyCount++;
    memcpy(&page->key[j], key, sizeof(IsbnList));
    page->child[j+1] = child;
}

void btRead (int rrn, BTPage *page){
    long addr = ((long) rrn * (long) PAGESIZE) + HEADER_SIZE;
    fseek(btFile, addr, 0);
    fread(page, PAGESIZE, 1, btFile);
}

int getRoot(){
	int root;
	rewind(btFile);
	if(!fread(&root, HEADER_SIZE, 1, btFile)){
		return NIL;
	}
	return root;
}

int createRoot(IsbnList *key, int left, int right){
    BTPage page;
    int rrn;
    rrn = getPage();
    pageInit(&page);
    memcpy(&page.key[0], key, sizeof(IsbnList));
    page.child[0] = left;
    page.child[1] = right;
    page.keyCount = 1;
    btWrite(rrn, &page);
    putRoot(rrn);
    return rrn;
}

int getPage(){
	int addr = ftell(btFile);
	addr = addr == 0 ? HEADER_SIZE : addr;
	return (int) (addr - HEADER_SIZE) / PAGESIZE;
}

void pageInit(BTPage *page){
    int j;
    for (j = 0; j < MAX_KEYS; j++) {
		memcpy(&page->key[j], &NO_KEY, sizeof(IsbnList));
        page->child[j] = NIL;
    }
    page->child[MAX_KEYS] = NIL;
}

void btWrite(int rrn, BTPage *page_ptr){
    long addr = (long) (rrn * PAGESIZE) + HEADER_SIZE;
    fseek(btFile, addr, 0);
    fwrite(page_ptr, PAGESIZE, 1, btFile);
}

void putRoot(int rrn){
    rewind(btFile);
    fwrite(&rrn, HEADER_SIZE, 1, btFile);
}

bool searchKeyInPage(IsbnList *key, BTPage *page, int *pos){
    int i;
    for (i = 0; i < page->keyCount && strcmp(key->code, page->key[i].code) > 0; i++);
    *pos = i;
    if (*pos < page->keyCount && strcmp(key->code, page->key[*pos].code) == 0){
        return true;
    }else{
        return false;
    }
}

void split(IsbnList *key, int child, BTPage *oldPage, IsbnList *promoKey, int *promoChild, BTPage *newPage){
	printf("\nDivisao de Nó");
	int j;
    IsbnList workKeys[MAX_KEYS+1];

    int workChilds[MAX_KEYS+2];
	//Copy from page to temp
    for (j = 0; j < MAX_KEYS; j++) {
        memcpy(&workKeys[j], &oldPage->key[j], sizeof(IsbnList));
        workChilds[j] = oldPage->child[j];
    }
    workChilds[j] = oldPage->child[j];

	//Reorder and insert the new key in temp
    for (j = MAX_KEYS; strcmp(key->code, workKeys[j-1].code) < 0 && j > 0; j--) {
        memcpy(&workKeys[j], &workKeys[j-1], sizeof(IsbnList));
        workChilds[j+1] = workChilds[j];
    }
    memcpy(&workKeys[j], key, sizeof(IsbnList));
	workChilds[j+1] = child;

	//Init new page
	fseek(btFile, 0, SEEK_END);
    *promoChild = getPage();
    pageInit(newPage);

	//insert in oldPage (LEFT)
	for (j = 0; j < MIN_KEYS+1; j++) {
		memcpy(&oldPage->key[j], &workKeys[j], sizeof(IsbnList));
		oldPage->child[j] = workChilds[j];
		memcpy(&oldPage->key[j+MIN_KEYS], &NO_KEY, sizeof(IsbnList));
		oldPage->child[j+1+MIN_KEYS] = NIL;
	}

	//Reset last position of oldPage (exclusive for ordem par)
	memcpy(&oldPage->key[MAX_KEYS-1], &NO_KEY, sizeof(IsbnList));
	oldPage->child[MAX_KEYS-1] = NIL;

	//newPage (RIGHT)
    for (j = 0; j < MIN_KEYS; j++) {
		memcpy(&newPage->key[j], &workKeys[j+2+MIN_KEYS], sizeof(IsbnList));
		newPage->child[j] = workChilds[j+2+MIN_KEYS];
    }

    oldPage->child[MIN_KEYS+1] = workChilds[MIN_KEYS+1];
    newPage->child[MIN_KEYS+1] = workChilds[j+2+MIN_KEYS];
    newPage->keyCount = MAX_KEYS - MIN_KEYS -1;
    oldPage->keyCount = MIN_KEYS+1;
    memcpy(promoKey, &workKeys[MIN_KEYS+1], sizeof(IsbnList)); //Key to be promoted = indice 2
}

void findBy2ISBN(){
	FILE *file;
	if(!openFile(&file, REGISTERS_FILENAME, true, true)){
		return;
	}
	if(!openFile(&btFile, BTREE_FILENAME, false, true)){
		return;
	}
	FILE *consultaCasadaKey1 = fopen(CONSULTA_CASADA_KEY_1_FILENAME, CREATE_WRITE_MODE);
	FILE *consultaCasadaKey2 = fopen(CONSULTA_CASADA_KEY_2_FILENAME, CREATE_WRITE_MODE);

	int index = getIndex(file, CONSULTA_CASADA_TEST_INDEX);
	ConsultaCasadaList key = consultaCasada[index];

	if(strlen(key.key1) == 0 || strlen(key.key2) == 0){
		printf("\nNao ha mais ISBNs de Consulta Casada a serem buscados!\n");
		fclose(consultaCasadaKey1);
		fclose(consultaCasadaKey2);
		fclose(btFile);
		fclose(file);
		return;
	}

	printf("\nKey1: %s\nKey2: %s", key.key1, key.key2);
	findBothKeysAndSaveToFiles(getRoot(), key.key1, key.key2, consultaCasadaKey1, consultaCasadaKey2);

	int resp;
	printf("\nDeseja realizar o merge ou match dos arquivos (1.Merge ou 2.Match)?: ");
	scanf("%d", &resp);
	switch(resp){
		case 1:
			mergeFiles(consultaCasadaKey1, consultaCasadaKey2, MERGE_MATCH_FILE_FILENAME);
			break;
		case 2:
			matchFiles(consultaCasadaKey1, consultaCasadaKey2, MERGE_MATCH_FILE_FILENAME);
			break;
		default: printf("\nEscolha inválida!: %d", resp);
	}
	incrementNumberOfRegisters(file, index, CONSULTA_CASADA_TEST_INDEX);
	fclose(consultaCasadaKey1);
	fclose(consultaCasadaKey2);
	fclose(btFile);
	fclose(file);
}

void mergeFiles(FILE *file1, FILE* file2, const char *filename){
	FILE *mergeFile = fopen(filename, CREATE_WRITE_MODE);

	rewind(file1);
	rewind(file2);

	IsbnList prev1;
	IsbnList prev2;
	bool fileEnd1 = false;
	bool fileEnd2 = false;

	if(!fread(&prev1, 1, sizeof(IsbnList), file1))
		fileEnd1 = true;
	if(!fread(&prev2, 1, sizeof(IsbnList), file2))
		fileEnd2 = true;

	printf("\n--MERGE RESULT--\n");

	while(!fileEnd1 && !fileEnd2){

		if(strcmp(prev1.code, prev2.code) < 0){ // Caso prev1 < prev2, escreve prev1
			fwrite(&prev1, 1, sizeof(IsbnList), mergeFile);
			printBook(prev1.offset);
			if(!fread(&prev1, 1, sizeof(IsbnList), file1))
				fileEnd1 = true;
		}else if(strcmp(prev1.code, prev2.code) > 0){ // Caso prev1 > prev2, escreve prev2
			fwrite(&prev2, 1, sizeof(IsbnList), mergeFile);
			printBook(prev2.offset);
			if(!fread(&prev2, 1, sizeof(IsbnList), file2))
				fileEnd2 = true;
		}else{ // Caso sejam iguais, escreve um deles
			fwrite(&prev1, 1, sizeof(IsbnList), mergeFile);
			printBook(prev1.offset);
			if(!fread(&prev1, 1, sizeof(IsbnList), file1))
				fileEnd1 = true;
			if(!fread(&prev2, 1, sizeof(IsbnList), file2))
				fileEnd2 = true;
		}
	}

	//Continuar o la�o indivualmente, caso um dos arquivos tenha chegado ao fim
	if(fileEnd1 == false){
		fwrite(&prev1, 1, sizeof(IsbnList), mergeFile);
		printBook(prev1.offset);
		while(fread(&prev1, 1, sizeof(IsbnList), file1)){
			if (prev1.offset > 0){
				fwrite(&prev1, 1, sizeof(IsbnList), mergeFile);
				printBook(prev1.offset);
			}
		}
	}
	if(fileEnd2 == false){
		fwrite(&prev2, 1, sizeof(IsbnList), mergeFile);
		printBook(prev2.offset);
		while(fread(&prev2, 1, sizeof(IsbnList), file2)){
			if (prev2.offset > 0){
				fwrite(&prev2, 1, sizeof(IsbnList), mergeFile);
				printBook(prev2.offset);
			}
		}
	}

	fclose(mergeFile);
}

void matchFiles(FILE *file1, FILE *file2, const char *filename){
	FILE *matchFile = fopen(filename, CREATE_WRITE_MODE);

	rewind(file1);
	rewind(file2);

	IsbnList prev1;
	IsbnList prev2;
	bool fileEnd1 = false;
	bool fileEnd2 = false;

	if(!fread(&prev1, 1, sizeof(IsbnList), file1))
		fileEnd1 = true;
	if(!fread(&prev2, 1, sizeof(IsbnList), file2))
		fileEnd2 = true;

	printf("\n--MATCH RESULT--\n");

	while(!fileEnd1 && !fileEnd2){
		if(strcmp(prev1.code, prev2.code) < 0){ //Compara se prev1 e menor que prev2
			if(!fread(&prev1, 1, sizeof(IsbnList), file1)) // Caso nao consiga ler, seta a variavel booleana
				fileEnd1 = true;

		}else if(strcmp(prev1.code, prev2.code) > 0){ // Compara se prev1 e maior que prev2
			if(!fread(&prev2, 1, sizeof(IsbnList), file2)) // Caso nao consiga ler, seta a variavel booleana
				fileEnd2 = true;

		}else{
			//Escreve no arquivo o elemento em comum (match)
			fwrite(&prev1, 1, sizeof(IsbnList), matchFile);
			printBook(prev1.offset);

			//Le os proximos elementos de cada arquivo
			if(!fread(&prev1, 1, sizeof(IsbnList), file1))
				fileEnd1 = true;
			if(!fread(&prev2, 1, sizeof(IsbnList), file2))
				fileEnd2 = true;
		}
	}

	fclose(matchFile);
}

void findBothKeysAndSaveToFiles(int rrn, const char *key1, const char *key2, FILE *consultaCasadaKey1, FILE *consultaCasadaKey2){
	if(rrn == NIL)
		return;
	else{
		BTPage page;
		btRead(rrn, &page);
		for(int i=0; i <= page.keyCount; i++){
			findBothKeysAndSaveToFiles(page.child[i], key1, key2, consultaCasadaKey1, consultaCasadaKey2);

			if (strcmp(page.key[i].code, NO_KEY) != 0){
				if (strcmp(key1, page.key[i].code) <= 0){
					fwrite(&page.key[i], sizeof(IsbnList), 1, consultaCasadaKey1);
				}
				if (strcmp(key2, page.key[i].code) >= 0){
					fwrite(&page.key[i], sizeof(IsbnList), 1, consultaCasadaKey2);
				}
			}
		}
	}
}

bool loadFilesToMemory(){
	bool allFilesLoaded = true;
	FILE *bibliotecaFile, *buscaFile, *consultaCasadaFile;
	int i=0;
	if (openFile(&bibliotecaFile, BIBLIOTECA_FILENAME, false, false)){
		i=0;
		while(fread(&biblioteca[i], sizeof(Book), 1, bibliotecaFile)){
			i++;
		}
		fclose(bibliotecaFile);
	}else{
		allFilesLoaded = false;
	}

	if (openFile(&buscaFile, BUSCA_FILENAME, false, false)){
		i=0;
		while(fread(&busca[i], sizeof(char), ISBN_LENGTH, buscaFile)){
			i++;
		}
		fclose(buscaFile);
	}else{
		allFilesLoaded = false;
	}

	if (openFile(&consultaCasadaFile, CONSULTA_CASADA_FILENAME, false, false)){
		i=0;
		while(fread(&consultaCasada[i], sizeof(ConsultaCasadaList), 1, consultaCasadaFile)){
			i++;
		}
		fclose(consultaCasadaFile);
	}else{
		allFilesLoaded = false;
	}
	if (allFilesLoaded){
        printf("\nTodos os arquivos foram carregados com sucesso!");
	}
	return allFilesLoaded;
}

bool openFile(FILE** file, const char* filename, bool writeMode, bool createIfNotExists){
	*file = fopen(filename, writeMode ? WRITE_MODE : READ_MODE);

	if (*file == NULL && createIfNotExists){
		*file = fopen(filename, CREATE_WRITE_MODE);
	}
	if (*file == NULL){
		printf("\nNao foi possivel abrir o arquivo %s", filename);
		return false;
	}
	return true;
}

void waitForUserResponse(){
    printf("\n\nPressione qualquer tecla para continuar...");
    getch();
	//getchar();
	//getchar();
}

void clearScreen(){
   system("cls");
   //system("clear");
}

int getIndex(FILE* file, int testIndexControl){
	TestIndex testIndex;
	rewind(file);
	if(!fread(&testIndex, sizeof(TestIndex), 1, file)){
		rewind(file);
		testIndex.biblioteca = 0;
		testIndex.busca = 0;
		testIndex.consultaCasada = 0;
		fwrite(&testIndex, sizeof(TestIndex), 1, file);
	}
	switch (testIndexControl){
		case BIBLIOTECA_TEST_INDEX:
			return testIndex.biblioteca;
			break;
		case BUSCA_TEST_INDEX:
			return testIndex.busca;
			break;
		case CONSULTA_CASADA_TEST_INDEX:
			return testIndex.consultaCasada;
			break;
	}
	return NIL;
}

void printTree(int rrn){
	if(rrn == NIL)
		return;
	else{
		BTPage page;
		btRead(rrn, &page);
		for(int i=0; i <= page.keyCount; i++){
			printTree(page.child[i]);
			if(strcmp(page.key[i].code, NO_KEY) != 0)
				printBook(page.key[i].offset);
		}
	}
}

void printBook(int offset){
	FILE *file;
	Book book;
	if(openFile(&file, REGISTERS_FILENAME, false, true)){
		fseek(file, offset, SEEK_SET);
		if(fread(&book, 1, sizeof(Book), file) == sizeof(Book)){
			if(strlen(book.isbn) > 0){
				printf("\n\nISBN: %s\n", book.isbn);
				printf("TITULO: %s\n", book.titulo);
				printf("AUTOR: %s\n", book.autor);
				printf("ANO: %s", book.ano);
			}
		}
		fclose(file);
	}else{
		printf("\nNAO FOI POSSIVEL ABRIR O ARQUIVO\n");
	}
}

void searchByISBN(){
	FILE *principalFile;
	if (!openFile(&btFile, BTREE_FILENAME, false, true)){
        return;
	}
	if (!openFile(&principalFile, REGISTERS_FILENAME, true, true)){
        return;
	}

	char isbn[ISBN_LENGTH];
	int root = getRoot();
	int index = getIndex(principalFile, BUSCA_TEST_INDEX);

	strcpy(isbn, busca[index]);
	if(strlen(isbn) == 0){
		printf("\nNao ha mais ISBNs a serem buscados!\n");
		fclose(btFile);
		fclose(principalFile);
		return;
	}

	searchKey(root, isbn);
	incrementNumberOfRegisters(principalFile, index, BUSCA_TEST_INDEX);
	fclose(btFile);
	fclose(principalFile);
}

void searchKey(int rrn, char isbn[]){
	int compare, i;
	BTPage page;
	btRead(rrn, &page);

	for(i=0; i<=page.keyCount; i++){
		compare = strcmp(isbn, page.key[i].code);
		if(compare == 0){
			printf("\nISBN %s encontrado, pagina %d, posicao %d\n", isbn, getPage(), i);
			printBook(page.key[i].offset);
			return;
		}else if(compare < 0 && page.child[i] != NIL)
			return searchKey(page.child[i], isbn);
	}
	printf("\nISBN %s nao encontrado", isbn);
}
