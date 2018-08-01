#include <stdio.h>
#include <conio.h>
#include <ctype.h>
#define INICIO 0
#define AUTOMATICO 1
#define PROTECCION 2
#define MANUAL_DET 3
#define MANUAL 4
typedef struct{
    char s0:1;      //sensor 0
    char s1:1;      //sensor 1
    char s2:1;      //sensor 2
    char s3:1;      //sensor 3
    char bot:1;     //pulsador manual
    char cambio:1;  //Uso cuando cambio de estado y debo imprimir algo una unica vez
}sensores_t;

//PROTOTIPOS
void interprete(char, sensores_t*);
char inicio(sensores_t*);
char automatico(sensores_t*);
char proteccion(sensores_t*);
char manual_detencion(sensores_t*);
char manual(sensores_t*);
void motor(char);

/***CUERPO PRINCIPAL***/
int main()
{
    sensores_t sens;    //declaro estructura de datos
    sens.bot=0;         //valores iniciales
    sens.s0=1;
    sens.s1=1;
    sens.s2=1;
    sens.s3=1;
    sens.cambio=0;
    char opc, estado=0; //variables del main

    printf("*************CONTROL DE LLENADO************\n\n");
    printf("CONTROLES:\n");
    printf("Sensor 0 (BORDE TANQUE SUPERIOR)\t\t<Q:1> <W=0>\n");
    printf("Sensor 1 (FONDO TANQUE SUPERIOR)\t\t<E:1> <R=0>\n");
    printf("Sensor 2 (BORDE TANQUE INFERIOR)\t\t<T:1> <Y=0>\n");
    printf("Sensor 3 (FONDO TANQUE INFERIOR)\t\t<U:1> <I=0>\n");
    printf("Pulsador manual \t\t\t\t<O:1> <P=0>\n");
    printf("\nMOTOR OFF");

    /*****BUCLE DE FUNCIONAMIENTO*****/
    while(1)
    {
        if(kbhit())
        {
            opc=getch();        //leo la tecla presionada
            interprete(opc, &sens);     //cambio valores de sensores segun corresponda
        }
        switch(estado)      //direccionamiento de funciones de estado
        {
             case INICIO:
                estado=inicio(&sens);
                break;
            case AUTOMATICO:
                estado=automatico(&sens);
                break;
            case PROTECCION:
                estado=proteccion(&sens);
                break;
            case MANUAL_DET:
                estado=manual_detencion(&sens);
                break;
            case MANUAL:
                estado=manual(&sens);
                break;
            default:
                estado=inicio(&sens);
                break;
        }
    }
    return 0;
}

/***********FUNCIONES**********/
void interprete(char opc,sensores_t* sensores)
{
    if((isupper(opc))==0) opc=toupper(opc); //convierto si es necesario a mayusculas
    switch(opc)
    {
    case 'Q':
        if(sensores->s1) sensores->s0=1;    //bloqueo condicion fisica PROHIBIDA
        printf("\nTanque superior LLENO");
        break;
    case 'W':
        sensores->s0=0;
        break;
    case 'E':
        sensores->s1=1;
        printf("\nAgua en tanque superior");
        break;
    case 'R':
        sensores->s1=0;
        sensores->s0=0;     //condición fisica tanque vacio en caso de que s0=1
        printf("\nTanque superior VACIO");
        break;
    case 'T':
        if(sensores->s3) sensores->s2=1;    //bloqueo condicion fisica PROHIBIDA
        printf("\nTanque reserva LLENO");
        break;
    case 'Y':
        sensores->s2=0;
        break;
    case 'U':
        sensores->s3=1;
        printf("\nAgua en tanque reserva");
        break;
    case 'I':
        sensores->s3=0;
        sensores->s2=0;     //condición fisica tanque vacio en caso de que s2=1
        printf("\nTanque reserva VACIO");
        break;
    case 'O':
        sensores->bot=1;
        break;
    case 'P':
        sensores->bot=0;
        break;
    default:
        break;
    }
}

char inicio(sensores_t* sensores)
{
    if(sensores->cambio)    //imprime una unica vez hasta que cambie de estado
    {
        motor(0);
        sensores->cambio=0;
    }
    if(sensores->bot)
    {
        sensores->cambio=1;
        return MANUAL;
    }
    if(!(sensores->s1)&&(sensores->s3))
    {
        sensores->cambio=1;
        return AUTOMATICO;
    }
    return INICIO;
}

char automatico(sensores_t* sensores)
{
    if(sensores->cambio)    //imprime una unica vez hasta que cambie de estado
    {
        motor(1);
        sensores->cambio=0;
    }
    if(!(sensores->s3))
    {
        sensores->cambio=1;
        return PROTECCION;
    }
    if(sensores->s0)
    {
        sensores->cambio=1;
        return INICIO;
    }
    return AUTOMATICO;
}

char proteccion(sensores_t* sensores)
{
    if(sensores->cambio)    //imprime una unica vez hasta que cambie de estado
    {
        motor(4);
        sensores->cambio=0;
    }
    if((sensores->s3)&&(sensores->bot))
    {
        sensores->cambio=1;
        return MANUAL_DET;
    }
    if(sensores->s2)
    {
        sensores->cambio=1;
        return AUTOMATICO;
    }
    return PROTECCION;
}

char manual_detencion(sensores_t* sensores)
{
    if(sensores->cambio)    //imprime una unica vez hasta que cambie de estado
    {
        if(!(sensores->s3)) motor(3);
        sensores->cambio=0;
    }
    if(sensores->bot)
    {
        sensores->cambio=1;
        return PROTECCION;
    }
    return MANUAL_DET;
}

char manual(sensores_t* sensores)
{
    if(sensores->cambio)    //imprime una unica vez hasta que cambie de estado
    {
        if(!(sensores->s3)) motor(2);
        sensores->cambio=0;
    }
    if(!(sensores->bot))
    {
        sensores->cambio=1;
        return INICIO;
    }
    return MANUAL;
}

void motor(char on)
{
    if(on==1) printf("\nMOTOR ON (automatico)");
    if(on==2) printf("\nMOTOR ON (manual)");
    if(on==3) printf("\nMOTOR ON (manual en proteccion)");
    if(on==4) printf("\nMOTOR OFF (proteccion)");
    else printf("\nMOTOR OFF");
}

