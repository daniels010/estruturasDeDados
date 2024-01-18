#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef char String50[50], String11[11];

typedef struct{
    String50 nome;
    String11 cpf, contato;
    float divida;
    float val_parc;
    int num_parcelas;
}cliente;

typedef cliente Cliente;

Cliente Pessoas;

FILE *Crediario;

long int tamALOC = 72*sizeof(char) + 2*sizeof(float) + sizeof(int);

void Incluir(){
    char Opt;
    String50 chave_CPF;
    int encontra;
    do{
        system("cls");
        printf("\n\tCRIAR NOVO CADASTRO:\n");
        printf("\nNome: ");
        scanf("%s", &Pessoas.nome);
        printf("CPF: ", &Pessoas.cpf);
        scanf("%s", &Pessoas.cpf);
        printf("Contato: ", &Pessoas.contato);
        scanf("%s", &Pessoas.contato);
        Pessoas.divida = 0;
        Pessoas.num_parcelas = 0;
        Pessoas.val_parc = 0;
        fseek(Crediario, 0, SEEK_END);
        fwrite(&Pessoas, tamALOC, 1, Crediario);
        printf("\n\tCrediario criado!\n\t*** SIIIII ***\n\n");
        system("pause");
        printf("\nCriar novo crediario? S/N: \n");
        scanf(" %c", &Opt);
        Opt = toupper(Opt);
    }while(Opt != 'N');
    return;
}

void Listar(){
    system("cls");
    printf("\n\tCREDIARIOS ATIVOS:\n");
    fseek(Crediario, 0 , SEEK_SET);
    do{
        fread(&Pessoas, tamALOC, 1, Crediario);
        if(feof(Crediario) == 0 && Pessoas.divida != -1){
            printf("\nNome: %s\n", Pessoas.nome);
            printf("CPF: %s\n", Pessoas.cpf);
            printf("Contato: %s\n", Pessoas.contato);
            printf("Divida: %.2f\n", Pessoas.divida);
            if(Pessoas.divida > 0){
                printf("Parcelas: %d\n", Pessoas.num_parcelas);
                printf("Valor da Parcela: %.2f\n\n", Pessoas.val_parc);
            }
        }
    }while(feof(Crediario) == 0);
    printf("\n");
    system("pause");
    return;
}


void Consultar(){
    system("cls");
    printf("\n\tCONSULTAR CLIENTE:\n");
    String50 chave_CPF;
    int encontra;
    fseek(Crediario, 0, SEEK_SET);
    printf("\nInsira o CPF: ");
    scanf("%s", &chave_CPF);
    do{
        fread(&Pessoas, tamALOC, 1, Crediario);
        if(strcmp(Pessoas.cpf, chave_CPF) == 0){
            encontra = 1;
            if(Pessoas.divida == -1){
                printf("\t\nCREDIARIO DESATIVADO, PARA REATIVAR ACESSE O MENU PRINCIPAL\n");
            }
            printf("\nNome: %s\n", Pessoas.nome);
            printf("CPF: %s\n", Pessoas.cpf);
            printf("Contato: %s\n", Pessoas.contato);
            printf("Divida: %.2f\n", Pessoas.divida);
            if(Pessoas.divida > 0){
                printf("Parcelas: %d\n", Pessoas.num_parcelas);
                printf("Valor da Parcela: %.2f\n\n", Pessoas.val_parc);
            } 
        }
        printf("\n");
    }while(feof(Crediario) == 0 && (encontra == 0));
    if(encontra == 0){
        printf("\n\tCLIENTE NAO CADASTRADO\t\n");
    }
    system("pause");
    return;
}

long int K;
long int tamARQ(){
    fseek(Crediario, 0, SEEK_END);
    K = ftell(Crediario)/tamALOC;
    return K;
}

void Comprar(){
    system("cls");
    printf("\n\tEFETUAR COMPRA:\n");
    String50 chave_CPF;
    int encontra, parcelas;
    float val_compra;
    char Opt;
    if(tamARQ() != 0){
        fclose(Crediario);
        Crediario = fopen("Listacred.txt", "r+");
        fseek(Crediario, 0, SEEK_SET);
        printf("\nInsira o CPF: ");
        scanf("%s", &chave_CPF);
        do{
            fread(&Pessoas, tamALOC, 1, Crediario);
            if(strcmp(Pessoas.cpf, chave_CPF) == 0){
                encontra = 1;
                printf("\nNome: %s\n", Pessoas.nome);
                printf("CPF: %s\n", Pessoas.cpf);
                printf("Contato: %s\n", Pessoas.contato);
                printf("Divida: %.2f\n", Pessoas.divida);
                if(Pessoas.divida > 0){
                    printf("Parcelas: %d\n", Pessoas.num_parcelas);
                    printf("Valor da Parcela: %.2f\n\n", Pessoas.val_parc);
                }
            }
        }while(feof(Crediario) == 0 && encontra == 0);
        if(encontra == 0){
            printf("\n\tCLIENTE NAO CADASTRADO\n\n");
            system("pause");
        }
        else{
            printf("\nInsira o valor da compra: \n");
            scanf("%f", &val_compra);
            if(Pessoas.divida > 0){
                do{
                    printf("\nGostaria de aumentar o numero de parcelas?\n\n"
                    "Se sim, digite a nova quantidade de parcelas (Max: 24)\n"
                    "Se nao, digite 0: \n");
                    scanf("%d", &parcelas);
                }while(parcelas < 0 || parcelas > 24);
            }
            else{
                do{
                    printf("Insira a quantidade de parcelas: (1 a 24) \n");
                    scanf("%d", &parcelas);
                }while(parcelas <= 0 || parcelas > 24);
            }
            Pessoas.divida += val_compra;
            Pessoas.num_parcelas += parcelas;
            Pessoas.val_parc = Pessoas.divida/Pessoas.num_parcelas;
            fseek(Crediario, -tamALOC, SEEK_CUR);
            fwrite(&Pessoas, tamALOC, 1, Crediario);
            printf("\t\nCOMPRA EFETUADA, RECEBA!\n\n");
            system("pause");
        }
        fclose(Crediario);
        Crediario = fopen("Listacred.txt", "a+");
    }
    else{
        printf("Lista vazia");
    }
    return;
}

void Pagar(){
    system("cls");
    printf("\n\tEFETUAR PAGAMENTO:\n");
    String50 chave_CPF;
    int encontra;
    int pag_parc;
    char Opt;
    if(tamARQ() != 0){
        fclose(Crediario);
        Crediario = fopen("Listacred.txt", "r+");
        fseek(Crediario, 0, SEEK_SET);
        printf("\nInsira o CPF: ");
        scanf("%s", &chave_CPF);
        do{
            fread(&Pessoas, tamALOC, 1, Crediario);
            if(strcmp(Pessoas.cpf, chave_CPF) == 0){
                encontra = 1;
                printf("\nNome: %s\n", Pessoas.nome);
                printf("CPF: %s\n", Pessoas.cpf);
                printf("Contato: %s\n", Pessoas.contato);
                printf("Divida: %.2f\n", Pessoas.divida);
                if(Pessoas.divida > 0){
                    printf("Parcelas: %d\n", Pessoas.num_parcelas);
                    printf("Valor da Parcela: %.2f\n\n", Pessoas.val_parc);
                }
            }
        }while(feof(Crediario) == 0 && encontra == 0);
        if(encontra == 0){
            printf("\n\tCLIENTE NAO CADASTRADO\n");
            system("pause");
        }
        else{
            if(Pessoas.divida > 0){
                do{
                printf("Insira quantas parcelas deseja pagar:\n"
                "Min: 1\nMax: %d\n", Pessoas.num_parcelas);
                scanf("%d", &pag_parc);
            }while(pag_parc <= 0  || pag_parc > Pessoas.num_parcelas);
                Pessoas.num_parcelas = Pessoas.num_parcelas - pag_parc;
                Pessoas.divida = Pessoas.divida - (pag_parc*Pessoas.val_parc);
                Pessoas.val_parc = Pessoas.divida / (Pessoas.num_parcelas);
                fseek(Crediario, -tamALOC, SEEK_CUR);
                fwrite(&Pessoas, tamALOC, 1, Crediario);
                printf("\t\nPagamento efetuado, RECEBA!\n\n\t");
                system("pause");
            }
            else{
                printf("\n\tCLIENTE NAO POSSUI DIVIDA\n\n");
                system("pause");
            }
        }
        fclose(Crediario);
        Crediario = fopen("Listacred.txt", "a+");
    }
    else{
        printf("Lista vazia");
    }
    return 0;
}

void DesativarCred(){
    system("cls");
    printf("\n\tDESATIVAR CREDIARIO:\n");
    String50 chave_CPF;
    int encontra;
    char Opt;
    if(tamARQ() != 0){
        fclose(Crediario);
        Crediario = fopen("Listacred.txt", "r+");
        fseek(Crediario, 0, SEEK_SET);
        printf("\nInsira o CPF: ");
        scanf("%s", &chave_CPF);
        do{
            fread(&Pessoas, tamALOC, 1, Crediario);
            if(strcmp(Pessoas.cpf, chave_CPF) == 0){
                encontra = 1;
                printf("\nNome: %s\n", Pessoas.nome);
                printf("CPF: %s\n", Pessoas.cpf);
                printf("Contato: %s\n", Pessoas.contato);
                printf("Divida: %.2f\n", Pessoas.divida);
                if(Pessoas.divida > 0){
                    printf("Parcelas: %d\n", Pessoas.num_parcelas);
                    printf("Valor da Parcela: %.2f\n\n", Pessoas.val_parc);
                }
            }
        }while(feof(Crediario) == 0 && encontra == 0);
        if(encontra == 0){
            printf("\n\tCLIENTE NAO CADASTRADO\n");
            system("pause");
        }
        else{
            do{
                printf("\nConfirma que quer desativar o registro? S/N:\n\n");
                scanf(" %c", &Opt);
                Opt = toupper(Opt);
            }while(Opt != 'N' && Opt != 'S');
            if(Opt == 'S'){
                if(Pessoas.divida > 0){
                    printf("\n\tEI RAPAZ! PARA DESATIVAR SEU CREDIARIO E NECESSARIO QUITAR SUA DIVIDA!\n\n");
                    system("pause");
                }
                else{
                    Pessoas.divida = -1;
                    fseek(Crediario, -tamALOC, SEEK_CUR);
                    fwrite(&Pessoas, tamALOC, 1, Crediario);
                    printf("\n\tCREDIARIO DESATIVADO. DEVOLVA :(\n\n");
                    system("pause");    
                }
            } 
            if(Opt == 'N'){
                printf("\n\tDESATIVAMENTO CANCELADO\n\n");
                system("pause");
            }
        }
        fclose(Crediario);
        Crediario = fopen("Listacred.txt", "a+");
    }
    else{
        printf("Lista vazia");
    }
    return 0;
}

void ReativarCred(){
    system("cls");
    printf("\n\tREATIVAR CREDIARIO:\n");
    String50 chave_CPF;
    int encontra;
    char Opt;
    if(tamARQ() != 0){
        fclose(Crediario);
        Crediario = fopen("Listacred.txt", "r+");
        fseek(Crediario, 0, SEEK_SET);
        printf("\nInsira o CPF: ");
        scanf("%s", &chave_CPF);
        do{
            fread(&Pessoas, tamALOC, 1, Crediario);
            if(strcmp(Pessoas.cpf, chave_CPF) == 0){
                encontra = 1;
                printf("\nNome: %s\n", Pessoas.nome);
                printf("CPF: %s\n", Pessoas.cpf);
                printf("Contato: %s\n", Pessoas.contato);
                printf("Divida: %.2f\n", Pessoas.divida);
                if(Pessoas.divida > 0){
                    printf("Parcelas: %d\n", Pessoas.num_parcelas);
                    printf("Valor da Parcela: %.2f\n\n", Pessoas.val_parc);
                }
            }
        }while(feof(Crediario) == 0 && encontra == 0);
        if(encontra == 0){
            printf("\n\tCLIENTE NAO CADASTRADO\n");
            system("pause");
        }
        else{
            do{
                printf("\nConfirma que quer reativar o registro? S/N:\n\n");
                scanf(" %c", &Opt);
                Opt = toupper(Opt);
            }while(Opt != 'N' && Opt != 'S');
            if(Opt == 'S'){
                Pessoas.divida = 0;
                fseek(Crediario, -tamALOC, SEEK_CUR);
                fwrite(&Pessoas, tamALOC, 1, Crediario);
                printf("\n\tCREDIARIO REATIVADO. RECEBA!! :D\n\n");
                system("pause");    
            } 
            if(Opt == 'N'){
                printf("\n\tREATIVAMENTO CANCELADO\n\n");
                system("pause");
            }
        }
        fclose(Crediario);
        Crediario = fopen("Listacred.txt", "a+");
    }
    else{
        printf("Lista vazia");
    }
    return 0;
}

int main(){
    Crediario = fopen("Listacred.txt", "a+");
    int menu;
    do{
        system("cls");
        printf("\tLUVA DE PREDERO STORE\n\n");
        printf("\tMenu Principal: \n\n");
        printf("1 - Criar Cadastro\n"
        "2 - Comprar\n"
        "3 - Consultar Divida\n"
        "4 - Pagar\n"
        "5 - Desativar Crediario\n"
        "6 - Listar Todos os Crediarios\n"
        "7 - Reativar Crediario\n"
        "0 - Sair\n\n");
        scanf("%d", &menu);
        switch(menu){
            case 1:
                Incluir();
                break;
            case 2:
                Comprar();
                break;
            case 3:
                Consultar();
                break;
            case 4:
                Pagar();
                break;
            case 5:
                DesativarCred();
                break;
            case 6:
                Listar();
                break;
            case 7:
                ReativarCred();
                break;
        }
    }while(menu != 0);
}