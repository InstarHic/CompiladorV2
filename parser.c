/******************************************************************
            Este es el archivo correspondiente al PARSER
*******************************************************************/

#define EXTERNA extern
// Macros para usar con la fcion first
#define CONSTANTE 0
#define DECLARACION 1
#define DECLARACION_PARAMETRO 2
#define DECLARACION_VARIABLE 3
#define DECLARACIONES 4
#define DECLARADOR_INIT 5
#define DEFINICION_FUNCION 6
#define ESPECIFICADOR_DECLARACION 7
#define ESPECIFICADOR_TIPO 8
#define EXPRESION 9
#define EXPRESION_SIMPLE 10
#define FACTOR 11
#define LISTA_DECLARACIONES 12
#define LISTA_DECLARACIONES_PARAM 13
#define LISTA_EXPRESIONES 14
#define LISTA_INICIALIZADORES 15
#define LISTA_PROPOSICIONES 16
#define LLAMADA_FUNCION 17
#define PROPOSICION 18
#define PROPOSICION_COMPUESTA 19
#define PROPOSICION_E_S 20
#define PROPOSICION_EXPRESION 21
#define PROPOSICION_ITERACION 22
#define PROPOSICION_RETORNO 23
#define PROPOSICION_SELECCION 24
#define TERMINO 25
#define UNIDAD_TRADUCCION 26
#define VARIABLE 27
#define LISTA_DECLARACIONES_INIT 28
//


#include <stdio.h>
#include <stdlib.h> //Agregada para evitar el warning: "incompatible implicit declaration of build-in function"
#include <string.h> //Agregada para evitar el warning: "incompatible implicit declaration of build-in function" relacionado a strcpy y strcat
#include "codigos.h"
#include "var_globales.h"
#include "ts.h"
#include "set.h"


/*********** prototipos *************/

void unidad_traduccion(set folset);
void declaraciones(set folset);
void especificador_tipo(set folset);
void especificador_declaracion(set folset);
void definicion_funcion(set folset);
void declaracion_variable(set folset);
void lista_declaraciones_param(set folset);
void declaracion_parametro(set folset);
void declarador_init(set folset);
void lista_declacion_init(set folset);
void constante(set folset);
void lista_inicializadores(set folset);
void lista_proposiciones(set folset);
void lista_declaraciones(set folset);
void declaracion(set folset);
void proposicion(set folset);
void proposicion_expresion(set folset);
void proposicion_compuesta(set folset);
void proposicion_seleccion(set folset);
void proposicion_iteracion(set folset);
void proposicion_e_s(set folset);
void proposicion_retorno(set folset);
void variable(set folset);
void expresion(set folset);
void expresion_asignacion(set folset);
void expresion_relacional(set folset);
void expresion_simple(set folset);
void relacion(set folset);
void termino(set folset);
void factor(set folset);
void llamada_funcion(set folset);
void lista_expresiones(set folset);



void scanner ();

set first(int);
void test(set,set,int);

/********** variables globales ************/



token *sbol;
set folsetLlamada;

extern FILE *yyin;


void scanner()
{
    int i;

    for (; (i=yylex())!= NADA && sbol->codigo == SEGUIR;);
    if (i == NADA) sbol->codigo = CEOF;
    /* yylex retorna 0 si llego a fin de archivo */

    liberar = linea;
    linea = (char *) malloc (strlen(linea) + strlen (token1.lexema) + 3);
    strcpy(linea, liberar);
    strcat(linea, token1.lexema);
    free ( (void *) liberar);

    /* El alumno debera dar una solucion al problema que las constantes
      enteras y flotantes son entregadas como strings al parser */
}


int main( int argc,char *argv[])
{



    linea = (char *) malloc (2);
    strcat(linea, "");
    //INIT------------------------------
    nro_linea=0;
    existeReturn=0;
    folsetLlamada=cons(NADA,NADA);
    //INIT------------------------------
    if (argc != 3) // El programa se debe llamar con 3 argumentos
    {
        error_handler(6);
        error_handler(COD_IMP_ERRORES);
        exit(1);
    }
    else
    {
        if ((yyin = fopen(argv[2], "r" )) == NULL) // si no se puede abrir el codigo fuente
        {
            error_handler(7);
            error_handler(COD_IMP_ERRORES);
            exit(1);
        }
    }

    if (strcmp(argv[1],"-c")*(strcmp(argv[1], "-g")))
    {
        // Entra por esta rama si la multiplicacion de las comparaciones da como resultado un numero distinto de cero, indicando que el segundo argumento no es valido. Esto ya que la función strcmp retorna cero cuando las cadenas pasadas como parametro son iguales.
        error_handler(83);
        error_handler(COD_IMP_ERRORES);
        exit(1);
    }
    else
    {
        //Entra por esta rama cuando la multiplicación retorna cero, indicando que el parametro es "-c" o "-g".
        if (strcmp(argv[1],"-g")!=0)
        {
            //Entra por esta rama si el parametro ingresado es "-c"

            sbol=&token1 ;/* la variable token */
            inic_tablas();

            pushTB(); //Crea el nivel 0

            scanner();
            folsetLlamada=une(folsetLlamada,cons(NADA,CEOF));
            unidad_traduccion(folsetLlamada);
            if (sbol->codigo != CEOF)
            {
                error_handler(84);
                error_handler(COD_IMP_ERRORES);   //no es una decl valida
            }

			//Controlamos las condiciones que debe cumplir el Main
			int posEnTablaMain;
			posEnTablaMain = en_tabla("main");
			if (posEnTablaMain == NIL)  // O sea que no existe...
			{
				error_handler(15);
				error_handler(COD_IMP_ERRORES);
			}
			else // existe :P
			{
				if (ts[posEnTablaMain].ets->clase != CLASFUNC) //controlamos si es una funcion
				{
					error_handler(43);
					error_handler(COD_IMP_ERRORES);
				}
				else
				{
					if (ts[posEnTablaMain].ets->ptr_tipo != en_tabla("void") ) // se compara con cero para determinar si su retorno es void
					{
						error_handler(35);
						error_handler(COD_IMP_ERRORES);
					}
					else
					{
						if(ts[posEnTablaMain].ets->desc.part_var.sub.cant_par!=0) // controlamos que el main no tenga parametros
						{
							error_handler(36);
							error_handler(COD_IMP_ERRORES);
						}
						else
						{
							// El main cumple todas las condiciones evaluadas
							// Si no hay errores, mostrar Compilacion Exitosa
						}

					}
				}
			}
        }
        else
        {
            // Entra por esta rama si el parametro que se le da a ucc es "-g"
        }
    }

}


/********* funciones del parser ***********/

void unidad_traduccion(set folset)
{
    test(first(UNIDAD_TRADUCCION),folset,50);
    while (sbol->codigo == CVOID || sbol->codigo == CCHAR ||
            sbol->codigo == CINT || sbol->codigo == CFLOAT)
        {
            folsetLlamada=une(folset,first(DECLARACIONES));
            declaraciones(folsetLlamada);
        }
}

void declaraciones(set folset)
{

    folsetLlamada= une(folset,une(cons(NADA,CIDENT),first(ESPECIFICADOR_DECLARACION)));
    especificador_tipo(folsetLlamada);
    if (sbol->codigo == CIDENT)
    {
        flagExisteIDDeclaracion=1;
        // ------------------------------
        // Insertamos el ID en la TS
        strcpy(ultimoID,sbol->lexema);
        strcpy(inf_id->nbre,ultimoID);
        inf_id->desc.nivel=getTopeTB();
        insertarTS();
        //--------------------------------
        scanner();
    }
    else
    {
        flagExisteIDDeclaracion=0;
        error_handler(16);
    }
    especificador_declaracion(folset);
}

void especificador_tipo(set folset)
{
    test(first(ESPECIFICADOR_TIPO),folset,51);
    switch (sbol->codigo)
    {
    case CVOID:
    {
        ultimoTipo=en_tabla("void");
        scanner();
        break;
    }
    case CCHAR:
    {
        ultimoTipo=en_tabla("char");
        scanner();
        break;
    }
    case CINT:
    {
        ultimoTipo=en_tabla("int");
        scanner();
        break;
    }
    case CFLOAT:
    {
        ultimoTipo=en_tabla("float");
        scanner();
        break;
    }
    default:
        error_handler(17);
    }
    test(folset,cons(NADA,NADA),52);
}

void especificador_declaracion(set folset)
{
    test(first(ESPECIFICADOR_DECLARACION),folset,54);
    switch (sbol->codigo)
    {
    case CPAR_ABR:
    {
        if(flagExisteIDDeclaracion==1)
        {
            // Actualizamos los datos de la funcion en la TS-----------------------------------
            ts[en_tabla(ultimoID)].ets->ptr_tipo=ultimoTipo;
            ts[en_tabla(ultimoID)].ets->clase=CLASFUNC;
            ts[en_tabla(ultimoID)].ets->desc.part_var.sub.cant_par=0; // se incrementa luego, con cada parametro que damos de alta
            ts[en_tabla(ultimoID)].ets->desc.part_var.sub.ptr_inf_res=NULL;
            strcpy(ultimoIdentificadorFuncion,ultimoID);
            pushTB();
            //---------------------------------------------------------------------
        }
        else
        {
            //entra si no se puso el id de la fcion
        }
        definicion_funcion(folset);
        break;
    }
    case CASIGNAC:
    case CCOR_ABR:
    case CCOMA:
    case CPYCOMA:
    {
        if(flagExisteIDDeclaracion==1)
        {
            if(ultimoTipo==en_tabla("void")) // las vbles no pueden ser de tipo void
            {
                error_handler(49);
                ts[en_tabla(ultimoID)].ets->ptr_tipo=en_tabla("TIPOERROR");
            }
            else
            {
                ts[en_tabla(ultimoID)].ets->ptr_tipo=ultimoTipo;
            }
            ts[en_tabla(ultimoID)].ets->clase=CLASVAR;

        }
        else
        {

        }
        declaracion_variable(folset);
        break;

    }

    default:
        error_handler(18);
    }

}

void definicion_funcion(set folset)
{
    test(first(DEFINICION_FUNCION),une(folset,une(first(LISTA_DECLARACIONES_PARAM),une(first(PROPOSICION_COMPUESTA),cons(CPAR_CIE,NADA)))),53);

    if (sbol->codigo == CPAR_ABR) scanner();
    else error_handler(19);

    if (sbol->codigo == CVOID || sbol->codigo == CCHAR ||
            sbol->codigo == CINT || sbol->codigo == CFLOAT)
        {
        folsetLlamada= une(folset,une(cons(CPAR_CIE,NADA),first(PROPOSICION_COMPUESTA)));
        lista_declaraciones_param(folsetLlamada);
        }

    if (sbol->codigo == CPAR_CIE) scanner();
    else error_handler(20);

    proposicion_compuesta(folset);

    // Controlo si la funcion requiere o no retorno.
    // Si no lo requiere y existe la proposicion de retorno para la definicion de dicha funcion, mostramos error.
    // Si lo requiere y no existe la proposicion de retorno para la definicion de dicha funcion, mostramos error.
    // Sino Ok.

    int posUltimoIdentificadorFuncion = en_tabla(ultimoIdentificadorFuncion);
    if (posUltimoIdentificadorFuncion != NIL)
    {
		if (ts[posUltimoIdentificadorFuncion].ets->ptr_tipo != en_tabla("void")) //distinto de void
		{
			if (!existeReturn)
			{
				///Mirar
				printf("Linea %d -\n", nro_linea);
				printf("\t Error 37: Falta return\n");
				//error_handler(37);
				//error_handler(COD_IMP_ERRORES);
			}
		}
		else
		{
			if (existeReturn)
			{
				///Mirar
				printf("Linea %d -\n", nro_linea);
				printf("\t Error 45: La funcion no deberia tener retorno, ya que fue declarada con retorno void\n");
				//error_handler(45);
				//error_handler(COD_IMP_ERRORES);
			}

		}
	}
	else
	{
	}
	existeReturn=0;

}

void lista_declaraciones_param(set folset)
{

    folsetLlamada= une(folset,une(cons(CCOMA,NADA),first(DECLARACION_PARAMETRO)));
    declaracion_parametro(folsetLlamada);

    while (sbol->codigo ==CCOMA)
    {
        scanner();
        folsetLlamada= une(folset,une(cons(CCOMA,NADA),first(DECLARACION_PARAMETRO)));
        declaracion_parametro(folsetLlamada);
    }
}

void declaracion_parametro(set folset)
{
    int fueAmp=0; // usado para indicar el tipo de pasaje (1: referencia, 0: valor)

    folsetLlamada= une(folset,cons(CAMPER|CCOR_ABR|CCOR_CIE,CIDENT));
    especificador_tipo(folsetLlamada);

    if (sbol->codigo == CAMPER)
    {
        fueAmp=1;
        scanner();
    }

    if (sbol->codigo == CIDENT)
    {
        flagExisteIDParametro=1;
        // Insertamos el parametro en la TS---------------------------------------------
        strcpy(ultimoID,sbol->lexema);
        inf_id->clase=CLASPAR;
        strcpy(inf_id->nbre,ultimoID);
        if(ultimoTipo==en_tabla("void")) // controlamos que el tipo del par no sea void
        {
            error_handler(80);
            inf_id->ptr_tipo=en_tabla("TIPOERROR");
        }
        else
        {
            inf_id->ptr_tipo=ultimoTipo;
        }
        inf_id->desc.nivel=getTopeTB();
        if(fueAmp)
        {
            inf_id->desc.part_var.tipo_pje=PASREFERENCIA; // pasaje por referencia
        }
        else
        {
            inf_id->desc.part_var.tipo_pje=PASVALOR; // pasaje por valor
        }
        insertarTS();
        if(flagExisteIDDeclaracion==1)
        {
            ts[en_tabla(ultimoIdentificadorFuncion)].ets->desc.part_var.sub.cant_par++; //aumentamos la cant de pars de la fcion
        }
        else
        {
            //entra si el id de la fcion no se puso
        }

        //----------------------------------------------------------------------------
        scanner();
    }
    else
    {
        error_handler(16);
        flagExisteIDParametro=0;
    }

    if (sbol->codigo == CCOR_ABR)
    {
        if(flagExisteIDParametro==1)
        {
            if(fueAmp) //el arreglo no puede ser de pasaje por referencia
            {
                error_handler(81);
                ts[en_tabla(ultimoID)].ets->ptr_tipo=en_tabla("TIPOARREGLO");
                ts[en_tabla(ultimoID)].ets->desc.part_var.arr.ptero_tipo_base=en_tabla("TIPOERROR");
            }
            else
            {
                ts[en_tabla(ultimoID)].ets->ptr_tipo=en_tabla("TIPOARREGLO");
                if(ultimoTipo==en_tabla("void"))
                {
                    ts[en_tabla(ultimoID)].ets->desc.part_var.arr.ptero_tipo_base=en_tabla("TIPOERROR");
                }
                else
                {
                    ts[en_tabla(ultimoID)].ets->desc.part_var.arr.ptero_tipo_base=ultimoTipo;
                }
            }
        }
        else
        {
            // entra si falta el id de parametro
        }
        scanner();

        if (sbol->codigo == CCOR_CIE) scanner();
        else error_handler(21);
    }
    // Actualizamos la lista encadenada de parametros
    if(flagExisteIDDeclaracion==1 && flagExisteIDParametro==1)
    {
        tipo_inf_res *ptr_lista=ts[en_tabla(ultimoIdentificadorFuncion)].ets->desc.part_var.sub.ptr_inf_res;
        if(ptr_lista==NULL) //en caso que la lista este vacia
        {
            ts[en_tabla(ultimoIdentificadorFuncion)].ets->desc.part_var.sub.ptr_inf_res=(tipo_inf_res*)malloc(sizeof(tipo_inf_res));
            ts[en_tabla(ultimoIdentificadorFuncion)].ets->desc.part_var.sub.ptr_inf_res->ptero_tipo=ts[en_tabla(ultimoID)].ets->ptr_tipo;
            ts[en_tabla(ultimoIdentificadorFuncion)].ets->desc.part_var.sub.ptr_inf_res->tipo_pje=ts[en_tabla(ultimoID)].ets->desc.part_var.tipo_pje;
            ts[en_tabla(ultimoIdentificadorFuncion)].ets->desc.part_var.sub.ptr_inf_res->ptr_sig=NULL;
        }
        else
        {
            // usamos ptr_lista para insertar el nuevo elemento
            while(ptr_lista->ptr_sig!=NULL)
            {
                ptr_lista=ptr_lista->ptr_sig;
            }
            ptr_lista->ptr_sig=(tipo_inf_res*)malloc(sizeof(tipo_inf_res));
            ptr_lista->ptr_sig->ptero_tipo=ts[en_tabla(ultimoID)].ets->ptr_tipo;
            ptr_lista->ptr_sig->tipo_pje=ts[en_tabla(ultimoID)].ets->desc.part_var.tipo_pje;
            ptr_lista->ptr_sig->ptr_sig=NULL;
        }
    }
    else
    {
        //entra si el id de la fcion o de un par no esta
    }
    fueAmp=0;

    test(folset,cons(NADA,NADA),55);
}

void lista_declaraciones_init(set folset)
{
    test(first(LISTA_DECLARACIONES_INIT),une(folset,une(first(DECLARADOR_INIT),cons(CCOMA,CIDENT))),57);

    if (sbol->codigo == CIDENT)
    {
        flagExisteIDDeclaracion=1;
        //Insertamos la vble en la TS----------------------------------
        strcpy(ultimoID,sbol->lexema);
        strcpy(inf_id->nbre,ultimoID);
        if(ultimoTipo==en_tabla("void")) // las vbles no pueden ser void
        {
            error_handler(49);
            inf_id->ptr_tipo=en_tabla("TIPOERROR");
        }
        else
        {
            inf_id->ptr_tipo=ultimoTipo;
        }
        inf_id->clase=CLASVAR;
        inf_id->desc.nivel=getTopeTB();
        insertarTS();
        //-------------------------------------------------------------
        scanner();
    }
    else
    {
        error_handler(16);
        flagExisteIDDeclaracion=0;
    }

    folsetLlamada= une(folset,une(first(DECLARADOR_INIT),cons(CCOMA,CIDENT)));
    declarador_init(folsetLlamada);

    while (sbol->codigo == CCOMA)
    {
        scanner();

        if (sbol->codigo == CIDENT)
        {
            // Insertamos la vble----------------------------------------
            strcpy(ultimoID,sbol->lexema);
            strcpy(inf_id->nbre,ultimoID);
            if(ultimoTipo==en_tabla("void")) // las vbles no pueden ser void
            {
                error_handler(49);
                inf_id->ptr_tipo=en_tabla("TIPOERROR");
            }
            else
            {
                inf_id->ptr_tipo=ultimoTipo;
            }
            inf_id->clase=CLASVAR;
            inf_id->desc.nivel=getTopeTB();
            insertarTS();
            //---------------------------------------------------------
            scanner();
        }
        else error_handler(16);

        folsetLlamada= une(folset,une(first(DECLARADOR_INIT),cons(CCOMA,CIDENT)));
        declarador_init(folsetLlamada);
    }

}


void declaracion_variable(set folset)
{

    folsetLlamada= une(folset,une(cons(CCOMA|CPYCOMA,NADA),first(LISTA_DECLARACIONES_INIT)));
    declarador_init(folsetLlamada);

    if (sbol->codigo == CCOMA)
    {
        scanner();

        folsetLlamada= une(folset,cons(CPYCOMA,NADA));
        lista_declaraciones_init(folsetLlamada);
    }

    if (sbol->codigo == CPYCOMA) scanner();
    else error_handler(22);

    test(folset,cons(NADA,NADA),54); /// se controla aca en lugar de al final de especificador_declaracion

}


void declarador_init(set folset)
{

    test(first(DECLARADOR_INIT),une(folset,cons(CCOR_CIE|CLLA_ABR|CLLA_CIE,NADA)),58);
    /// VER TEST!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    switch (sbol->codigo)
    {
    case CASIGNAC:
    {
        scanner();
        constante(folset);
        break;
    }
    case CCOR_CIE:
    case CLLA_ABR:
    case CLLA_CIE:  ///VER!!!!!!!!!!!!!!!!!!!!!!!
    case CCOR_ABR:
    {
        scanner();
        if(flagExisteIDDeclaracion==1)
        {
            if(ultimoTipo==en_tabla("void"))//el arreglo no puede ser void. si lo es, actualizamos el tipo base a error
            {
                error_handler(46);
                ts[en_nivel_actual(ultimoID)].ets->desc.part_var.arr.ptero_tipo_base=en_tabla("TIPOERROR");
            }
            else
            {
                ts[en_nivel_actual(ultimoID)].ets->desc.part_var.arr.ptero_tipo_base=ultimoTipo;
            }
            ts[en_nivel_actual(ultimoID)].ets->ptr_tipo=en_tabla("TIPOARREGLO");
            if (sbol->codigo == CCONS_ENT)
            {
                ts[en_nivel_actual(ultimoID)].ets->desc.part_var.arr.cant_elem=atoi(sbol->lexema);
                folsetLlamada= une(folset,une(cons(CCOR_CIE|CASIGNAC|CLLA_ABR|CLLA_CIE,NADA),first(LISTA_INICIALIZADORES)));
                constante(folsetLlamada);
            }
        }
        else
        {
            //entra si no se puso el ident de la vble
        }
        if (sbol->codigo == CCOR_CIE) scanner();
        else error_handler(21);

        if (sbol->codigo == CASIGNAC)
        {
            scanner();

            if (sbol->codigo == CLLA_ABR) scanner();
            else error_handler(23);

            folsetLlamada= une(folset,cons(CLLA_CIE,NADA));
            lista_inicializadores(folsetLlamada);

            if (sbol->codigo == CLLA_CIE) scanner();
            else error_handler(24);

        }

        break;
    }
    }
    test(folset,cons(NADA,NADA),59);
}

void lista_inicializadores(set folset)
{

    folsetLlamada= une(folset,une(cons(CCOMA,NADA),first(CONSTANTE)));
    constante(folsetLlamada);

    while (sbol->codigo == CCOMA)
    {
        scanner();
        folsetLlamada= une(folset,une(cons(CCOMA,NADA),first(CONSTANTE)));
        constante(folsetLlamada);
    }

}


void proposicion_compuesta(set folset)
{
    test(first(PROPOSICION_COMPUESTA),une(folset,une(first(LISTA_DECLARACIONES),une(first(LISTA_PROPOSICIONES),cons(CLLA_CIE,NADA)))),60);

    if (sbol->codigo == CLLA_ABR)
    {
        scanner();
    }
    else error_handler(23);

    if (sbol->codigo == CVOID || sbol->codigo == CCHAR ||
            sbol->codigo == CINT || sbol->codigo == CFLOAT)

        {
            folsetLlamada= une(folset,une(cons(CLLA_CIE,NADA),first(LISTA_PROPOSICIONES)));
            lista_declaraciones(folsetLlamada);
        }


    if (sbol->codigo == CLLA_ABR || sbol->codigo == CMAS ||
            sbol->codigo == CMENOS || sbol->codigo == CIDENT ||
            sbol->codigo == CPAR_ABR || sbol->codigo == CNEG ||
            sbol->codigo == CCONS_ENT || sbol->codigo == CCONS_FLO ||
            sbol->codigo == CCONS_CAR || sbol->codigo == CCONS_STR ||
            sbol->codigo == CIF || sbol->codigo == CWHILE ||
            sbol->codigo == CIN || sbol->codigo == COUT ||
            sbol->codigo == CPYCOMA || sbol->codigo == CRETURN)

        {
            folsetLlamada= une(folset,cons(CLLA_CIE,NADA));
            lista_proposiciones(folsetLlamada);
        }


    if (sbol->codigo == CLLA_CIE)
    {
        pop_nivel();

        scanner();
    }
    else error_handler(24);

    test(folset,cons(NADA,NADA),61);
}
void lista_declaraciones(set folset)
{

    folsetLlamada= une(folset,first(DECLARACION));
    declaracion(folsetLlamada);

    while (sbol->codigo == CVOID || sbol->codigo == CCHAR ||
            sbol->codigo == CINT || sbol->codigo == CFLOAT)

        {
            folsetLlamada= une(folset,first(DECLARACION));
            declaracion(folsetLlamada);
        }


}

void declaracion(set folset)
{

    folsetLlamada= une(folset,une(cons(CPYCOMA,NADA),first(LISTA_DECLARACIONES_INIT)));
    especificador_tipo(folsetLlamada);

    folsetLlamada=une(folset,cons(CPYCOMA,NADA));
    lista_declaraciones_init(folsetLlamada);

    if (sbol->codigo == CPYCOMA) scanner();
    else error_handler(22);

    test(folset,cons(NADA,NADA),62);

}

void lista_proposiciones(set folset)
{

    folsetLlamada= une(folset,first(PROPOSICION));
    proposicion(folsetLlamada);

    while (sbol->codigo == CLLA_ABR || sbol->codigo == CMAS ||
            sbol->codigo == CMENOS || sbol->codigo == CIDENT ||
            sbol->codigo == CPAR_ABR || sbol->codigo == CNEG ||
            sbol->codigo == CCONS_ENT || sbol->codigo == CCONS_FLO ||
            sbol->codigo == CCONS_CAR || sbol->codigo == CCONS_STR ||
            sbol->codigo == CIF || sbol->codigo == CWHILE ||
            sbol->codigo == CIN || sbol->codigo == COUT ||
            sbol->codigo == CPYCOMA || sbol->codigo == CRETURN)


            {
                folsetLlamada= une(folset,first(PROPOSICION));
                proposicion(folsetLlamada);
            }


}

void proposicion(set folset)
{
    test(first(PROPOSICION),folset,63);
    switch (sbol->codigo)
    {
    case CLLA_ABR:
    {
        pushTB(); // en definicion de fcion no se llama a esto, y por lo tanto los bloques se dan de alta correctamente
        proposicion_compuesta(folset);
        break;
    }
    case CWHILE:
        proposicion_iteracion(folset);
        break;
    case CIF:
        proposicion_seleccion(folset);
        break;
    case CIN:
    case COUT:
        proposicion_e_s(folset);
        break;
    case CMAS:
    case CMENOS:
    case CIDENT:
    case CPAR_ABR:
    case CNEG:
    case CCONS_ENT:
    case CCONS_FLO:
    case CCONS_CAR:
    case CCONS_STR:
    case CPYCOMA:
        proposicion_expresion(folset);
        break;
    case CRETURN:
        proposicion_retorno(folset);
        break;
    default:
        error_handler(25);
    }
}

void proposicion_iteracion(set folset)
{
    //test(); No va porque solo tiene una invocacion, y es condicional

    if (sbol->codigo == CWHILE) scanner();
    else error_handler(26);

    if (sbol->codigo == CPAR_ABR)
    {
        scanner();

    }
    else error_handler(19);

    folsetLlamada= une(folset,une(cons(CPAR_CIE,NADA),first(PROPOSICION)));
    expresion(folsetLlamada);

    if (sbol->codigo == CPAR_CIE) scanner();
    else error_handler(20);

    proposicion(folset);

}


void proposicion_seleccion(set folset)
{
    //test(); No va porque solo tiene una invocacion, y es condicional

    if (sbol->codigo == CIF) scanner();
    else error_handler(27);

    if (sbol->codigo == CPAR_ABR)
    {
        scanner();

    }
    else error_handler(19);

    folsetLlamada= une(folset,une(cons(CPAR_CIE|CELSE,NADA),first(PROPOSICION)));
    expresion(folsetLlamada);

    if (sbol->codigo == CPAR_CIE) scanner();
    else error_handler(20);

    folsetLlamada= une(folset,une(cons(CELSE,NADA),first(PROPOSICION)));
    proposicion(folsetLlamada);

    if (sbol->codigo == CELSE)
    {
        scanner();

        proposicion(folset);
    }

}

void proposicion_e_s(set folset)
{
    //test(); No va porque solo tiene una invocacion, y es condicional

    switch(sbol->codigo)
    {
    case CIN:
    {
        flagUltimoIDError=0;
        scanner();

        if (sbol->codigo == CSHR) scanner();
        else error_handler(28);

        strcpy(ultimoID,sbol->lexema);
        if (en_tabla(sbol->lexema) == NIL)
        {
            flagUltimoIDError=1;
            error_handler(33);// identificador no declarado
            // agregar manej para fcion no declarada, etc
            inf_id->clase=CLASVAR; //SI EL ID NO ESTA EN LA TS, LO DAMOS DE ALTA COMO VBLE
            strcpy(inf_id->nbre,ultimoID);
            inf_id->ptr_tipo=en_tabla("TIPOERROR");
            inf_id->desc.nivel=getTopeTB();
            insertarTS();

        }
        folsetLlamada= une(folset,une(cons(CSHR|CPYCOMA,NADA),first(VARIABLE)));
        variable(folsetLlamada);

        while (sbol->codigo == CSHR)
        {

            scanner();
            flagUltimoIDError=0;
            strcpy(ultimoID,sbol->lexema);
            if (en_tabla(sbol->lexema) == NIL)
            {
                flagUltimoIDError=1;
                error_handler(33);// identificador no declarado
                // agregar manej para fcion no declarada, etc
                inf_id->clase=CLASVAR; //SI EL ID NO ESTA EN LA TS, LO DAMOS DE ALTA COMO VBLE
                strcpy(inf_id->nbre,ultimoID);
                inf_id->ptr_tipo=en_tabla("TIPOERROR");
                inf_id->desc.nivel=getTopeTB();
                insertarTS();

            }
            folsetLlamada= une(folset,une(cons(CSHR|CPYCOMA,NADA),first(VARIABLE)));
            variable(folsetLlamada);
        }
        if (sbol->codigo == CPYCOMA) scanner();
        else error_handler(22);
        break;
    }
    case COUT:
    {
        scanner();
        if (sbol->codigo == CSHL) scanner();
        else error_handler(29);
        folsetLlamada= une(folset,une(cons(CSHL|CPYCOMA,NADA),first(EXPRESION)));
        expresion(folsetLlamada);
        while (sbol->codigo == CSHL)
        {
            scanner();
            folsetLlamada= une(folset,une(cons(CSHL|CPYCOMA,NADA),first(EXPRESION)));
            expresion(folsetLlamada);
        }
        if (sbol->codigo == CPYCOMA) scanner();
        else error_handler(22);
        break;
    }
    default:
        error_handler(25);
    }
    test(folset,cons(NADA,NADA),64);
}


void proposicion_retorno(set folset)
{
    //test(); No va porque solo tiene una invocacion, y es condicional

    scanner();
    folsetLlamada= une(folset,cons(CPYCOMA,NADA));
    expresion(folsetLlamada);
    if (sbol->codigo == CPYCOMA) scanner();
    else error_handler(22);

    existeReturn=1;
    /// Aqui colocaremos el codigo para controlar el tipo de retorno. i think.

    test(folset,cons(NADA,NADA),65);
}


void proposicion_expresion(set folset)
{
    //test(); No va porque solo tiene una invocacion, y es condicional
    if (sbol->codigo == CMAS || sbol->codigo == CMENOS ||
            sbol->codigo == CIDENT ||
            sbol->codigo == CPAR_ABR || sbol->codigo == CNEG ||
            sbol->codigo == CCONS_ENT || sbol->codigo == CCONS_FLO ||
            sbol->codigo == CCONS_CAR || sbol->codigo == CCONS_STR)

        {
            folsetLlamada= une(folset,cons(CPYCOMA,NADA));
            expresion(folsetLlamada);
        }


    if (sbol->codigo == CPYCOMA) scanner();
    else error_handler(22);

    test(folset,cons(NADA,NADA),66);
}


void expresion(set folset)
{
    folsetLlamada= une(folset,une(cons(CASIGNAC,CDISTINTO|CIGUAL|CMENOR|CMEIG|CMAYOR|CMAIG),first(EXPRESION)));
    expresion_simple(folsetLlamada);

    switch (sbol->codigo)
    {
    case CASIGNAC:
    {
        scanner();
        expresion(folset);
        break;
    }
    case CDISTINTO:
    case CIGUAL:
    case CMENOR:
    case CMEIG:
    case CMAYOR:
    case CMAIG:
    {
        scanner();
        expresion(folset);
        break;
    }
    }
}


void expresion_simple(set folset)
{
    test(first(EXPRESION_SIMPLE),une(folset,cons(NADA,COR)),67);

    if (sbol->codigo == CMAS || sbol->codigo == CMENOS) scanner();

    folsetLlamada= une(folset,une(cons(NADA,CMAS|CMENOS|COR),first(TERMINO)));
    termino(folsetLlamada);

    while (sbol->codigo == CMAS || sbol->codigo == CMENOS || sbol->codigo == COR || in(sbol->codigo,first(TERMINO)))
    {
        scanner();
        folsetLlamada= une(folset,une(cons(NADA,CMAS|CMENOS|COR),first(TERMINO)));
        termino(folsetLlamada);
    }

}

void termino(set folset)
{

    folsetLlamada= une(folset,une(cons(NADA,CMULT|CDIV|CAND),first(FACTOR)));
    factor(folsetLlamada);

    while (sbol->codigo == CMULT || sbol->codigo == CDIV || sbol->codigo == CAND || in(sbol->codigo,first(FACTOR)))
    {
        scanner();
        folsetLlamada= une(folset,une(cons(NADA,CMULT|CDIV|CAND),first(FACTOR)));
        factor(folsetLlamada);
    }

}

void factor(set folset)
{
    test(first(FACTOR),folset,68);

    switch (sbol->codigo)
    {
    case CIDENT:
    {
        flagUltimoIDError=0; // se usa para distinguir si el error se produjo en el ultimo id, o en uno anterior
        strcpy(ultimoID,sbol->lexema);
        if (en_tabla(sbol->lexema) == NIL)
        {
            flagUltimoIDError=1;
            error_handler(33);// identificador no declarado
            // agregar manej para fcion no declarada, etc
            inf_id->clase=CLASVAR; //SI EL ID NO ESTA EN LA TS, LO DAMOS DE ALTA COMO VBLE
            strcpy(inf_id->nbre,ultimoID);
            inf_id->ptr_tipo=en_tabla("TIPOERROR");
            inf_id->desc.nivel=getTopeTB();
            insertarTS();
            variable(folset);
        }
        else
        {
            if (ts[en_tabla(sbol->lexema)].ets->clase == CLASVAR)
            {
                variable(folset);
            }
            else
            {
                if(ts[en_tabla(sbol->lexema)].ets->clase == CLASFUNC)
                {
                    llamada_funcion(folset);
                }
                else
                {
                    if(ts[en_tabla(sbol->lexema)].ets->clase == CLASPAR)
                    {

                        if(sbol->codigo==CIDENT)
                        {
                            strcpy(ultimoID,sbol->lexema);
                            scanner();
                            if(ts[en_tabla(ultimoID)].ets->ptr_tipo==en_tabla("TIPOARREGLO"))
                            {
                                if(sbol->codigo==CCOR_ABR)
                                {
                                    scanner();
                                    folsetLlamada= une(folset,cons(CCOR_CIE,NADA));
                                    expresion(folsetLlamada);
                                    if(sbol->codigo==CCOR_CIE)
                                    {
                                        scanner();
                                    }
                                    else
                                    {
                                        error_handler(21);
                                    }
                                }

                                else
                                {
                                    if(flagLlamadaFcion!=1)
                                    {
                                        error_handler(44);
                                    }

                                }

                        }
                        }
                        else
                        {
                            error_handler(16);
                        }
                        //se trata de un parametro formal.
                        ///hacer chequeo de tipos para parametro formal
                    }
                    else
                    {
                        //Error: se esperaba un factor
                        error_handler(31);
                    }

                }
            }
        }
        break;

    }
    case CCONS_ENT:
    case CCONS_FLO:
    case CCONS_CAR:
        constante(folset);
        break;
    case CCONS_STR:
        scanner();
        break;/* errores de recuperacion de errores */

    case CPAR_ABR:
    {
        scanner();
        folsetLlamada= une(folset,cons(CPAR_CIE,NADA));
        expresion(folsetLlamada);
        if (sbol->codigo == CPAR_CIE) scanner();
        else error_handler(20);
        break;
    }
    case CNEG:
    {
        scanner();
        expresion(folset);
        break;
    }
    default:
        error_handler(31);
    }
    test(folset,cons(NADA,NADA),69);

}

void variable(set folset)
{
    test(first(VARIABLE),une(folset,une(first(EXPRESION),cons(CCOR_ABR|CCOR_CIE,NADA))),70);
    if(sbol->codigo==CIDENT)
    {
        scanner();
    }
    else
    {
        error_handler(8);
    }

    if (sbol->codigo == CCOR_ABR)
    {
        if(ts[en_tabla(ultimoID)].ets->ptr_tipo!=en_tabla("TIPOARREGLO")) // el ultimo id no es un arreglo
        {
            if(ts[en_tabla(ultimoID)].ets->ptr_tipo==en_tabla("TIPOERROR")) // es una llamada a una vble no declarada
            {
                if(flagUltimoIDError) // cambia el tipo del id de vble a arreglo, y su tipo base a error
                {
                    ts[en_tabla(ultimoID)].ets->ptr_tipo=en_tabla("TIPOARREGLO");
                    ts[en_tabla(ultimoID)].ets->desc.part_var.arr.ptero_tipo_base=en_tabla("TIPOERROR");
                    error_handler(48);
                }
                else
                {
                    if(en_nivel_actual(ultimoID)!=NIL) //el id está en este nivel, pero declarado como vble
                    {
                        error_handler(32);
                    }
                    else // en id esta en un nivel anterior, y por eso se lo da de alta como un arr de tipo error
                    {

                        inf_id->clase=CLASVAR;
                        strcpy(inf_id->nbre,ultimoID);
                        inf_id->ptr_tipo=en_tabla("TIPOARREGLO");
                        inf_id->desc.part_var.arr.ptero_tipo_base=en_tabla("TIPOERROR");
                        inf_id->desc.nivel=getTopeTB();
                        insertarTS();
                        error_handler(48);
                    }
                }
            }
            else
            {
                if(en_nivel_actual(ultimoID)!=NIL) // ya esta declarado como vble en este nivel, por lo tanto no puede usarse como arreglo
                {
                    error_handler(32);
                }
                else
                {
                    // Insertamos el id en la TS-----------------------------------------
                    inf_id->clase=CLASVAR;
                    strcpy(inf_id->nbre,ultimoID);
                    inf_id->ptr_tipo=en_tabla("TIPOARREGLO");
                    inf_id->desc.part_var.arr.ptero_tipo_base=en_tabla("TIPOERROR");
                    inf_id->desc.nivel=getTopeTB();
                    insertarTS();
                    //----------------------------------------------------------------
                    error_handler(48);
                }
            }

        }
        else
        {
            /// Uso correcto. Acá van chequeos de tipo y eso...
        }
        scanner(); //consumimos el corchete que abre
        folsetLlamada= une(folset,cons(CCOR_CIE,NADA));
        expresion(folsetLlamada);
        if (sbol->codigo == CCOR_CIE) scanner();
        else error_handler(21);
    }
    else
    {
        if(flagUltimoIDError) // no viene un corchete que abre, entonces se trata del uso de una vble no declarada
        {
            error_handler(47);
        }
        else
        {
            if(ts[en_tabla(ultimoID)].ets->ptr_tipo==en_tabla("TIPOARREGLO") && flagLlamadaFcion!=1) // si el arr se usa como parametro, pueden ponerse los corchetes
            {
                error_handler(40);
            }
            else
            {
                //uso correcto de una vble
            }
        }
    }
    flagUltimoIDError=0;
    test(folset,cons(NADA,NADA),71);
}

void llamada_funcion(set folset)
{
    test(first(LLAMADA_FUNCION),une(folset,une(first(LISTA_EXPRESIONES),cons(CPAR_ABR|CPAR_CIE,NADA))),56);
    cantParReales=0; //Inicializamos la cant de parametros
    char funcionActual[TAM_LEXEMA]; // usada para referenciar la funcion a la que pertenecen los parametros que se estan contando
    if(sbol->codigo==CIDENT)
    {
        strcpy(funcionActual,ultimoID);
        scanner();
    }
    else
    {
        error_handler(16);
    }




    if (sbol->codigo == CPAR_ABR) scanner();
    else error_handler(19);

    if (sbol->codigo == CMAS || sbol->codigo == CMENOS ||
            sbol->codigo == CIDENT ||
            sbol->codigo == CPAR_ABR || sbol->codigo == CNEG ||
            sbol->codigo == CCONS_ENT || sbol->codigo == CCONS_FLO ||
            sbol->codigo == CCONS_CAR || sbol->codigo == CCONS_STR)

        {
        flagLlamadaFcion=1;
        folsetLlamada= une(folset,cons(CPAR_CIE,NADA));
        lista_expresiones(folsetLlamada);
        flagLlamadaFcion=0;
        }

    if (sbol->codigo == CPAR_CIE)
    {
        if(flagExisteIDDeclaracion)
        {
            if(cantParReales!=ts[en_tabla(funcionActual)].ets->desc.part_var.sub.cant_par) //controlamos que la cant de pars actuales concuerde con la de formales
            {
                error_handler(82);
            }
        }

        scanner();
    }
    else error_handler(20);
    test(folset,cons(NADA,NADA),72);
}

void lista_expresiones(set folset)
{

    folsetLlamada= une(folset,une(cons(CCOMA,NADA),first(EXPRESION)));
    expresion(folsetLlamada);
    cantParReales++; //por cada parametro aumentamos el contador

    while (sbol->codigo == CCOMA )
    {

        scanner();
        folsetLlamada= une(folset,une(cons(CCOMA,NADA),first(EXPRESION)));
        expresion(folsetLlamada);
        cantParReales++;
    }

}

void constante(set folset)
{
    test(first(CONSTANTE),folset,73);
    switch (sbol->codigo)
    {
    case CCONS_ENT:
        scanner();
        break;
    case CCONS_FLO:
        scanner();
        break;
    case CCONS_CAR:
        scanner();
        break;
    default:
        error_handler(38);
    }
    test(folset,cons(NADA,NADA),74);

}


set first(int metodo)
{
    switch(metodo)
    {
        case CONSTANTE: return cons(NADA,CCONS_ENT|CCONS_FLO|CCONS_CAR);
        case DECLARACION: return cons(CVOID|CCHAR|CINT|CFLOAT,NADA);
        case DECLARACIONES: return cons(CVOID|CCHAR|CINT|CFLOAT,NADA);
        case DECLARACION_PARAMETRO:  return cons(CVOID|CCHAR|CINT|CFLOAT,NADA);
        case DECLARACION_VARIABLE: return cons(CASIGNAC|CCOR_ABR|CCOMA|CPYCOMA,NADA);
        ///case DECLARADOR_INIT: return cons(CASIGNAC|CCOR_ABR,NADA);
        case DECLARADOR_INIT: return cons(CASIGNAC|CCOR_ABR|CCOMA|CPYCOMA,NADA);
        case DEFINICION_FUNCION: return cons(CPAR_ABR,NADA);
        case ESPECIFICADOR_DECLARACION: return cons(CPAR_ABR|CASIGNAC|CCOR_ABR|CCOMA|CPYCOMA,NADA);
        case ESPECIFICADOR_TIPO: return cons(CVOID|CCHAR|CINT|CFLOAT,NADA);
        case EXPRESION: return cons(CPAR_ABR,CMAS|CMENOS|CNEG|CIDENT|CCONS_CAR|CCONS_ENT|CCONS_FLO|CCONS_STR);
        case EXPRESION_SIMPLE: return cons(CPAR_ABR,CMAS|CMENOS|CNEG|CIDENT|CCONS_CAR|CCONS_ENT|CCONS_FLO|CCONS_STR);
        case FACTOR: return cons(CPAR_ABR,CNEG|CIDENT|CCONS_CAR|CCONS_ENT|CCONS_FLO|CCONS_STR);
        case LISTA_DECLARACIONES: return cons(CVOID|CCHAR|CINT|CFLOAT,NADA);
        case LISTA_DECLARACIONES_PARAM: return cons(CVOID|CCHAR|CINT|CFLOAT,NADA);
        case LISTA_EXPRESIONES: return cons(CPAR_ABR,CMAS|CMENOS|CNEG|CIDENT|CCONS_CAR|CCONS_ENT|CCONS_FLO|CCONS_STR);
        case LISTA_INICIALIZADORES: return cons(NADA,CCONS_CAR|CCONS_ENT|CCONS_FLO);
        case LISTA_PROPOSICIONES: return cons(CWHILE|CIF|CIN|COUT|CPYCOMA|CPAR_ABR|CLLA_ABR,CCONS_CAR|CCONS_ENT|CCONS_FLO|CCONS_STR|CIDENT|CMAS|CMENOS|CNEG|CRETURN);
        case LLAMADA_FUNCION: return cons(NADA,CIDENT);
        case PROPOSICION: return cons(CWHILE|CIF|CIN|COUT|CPYCOMA|CPAR_ABR|CLLA_ABR,CCONS_CAR|CCONS_ENT|CCONS_FLO|CCONS_STR|CIDENT|CMAS|CMENOS|CNEG|CRETURN);
        case PROPOSICION_COMPUESTA: return cons(CLLA_ABR,NADA);
        case PROPOSICION_EXPRESION: return cons(CPAR_ABR|CPYCOMA,CMAS|CMENOS|CNEG|CIDENT|CCONS_CAR|CCONS_ENT|CCONS_FLO|CCONS_STR);
        case PROPOSICION_E_S: return cons(CIN|COUT,NADA);
        case PROPOSICION_ITERACION: return cons(CWHILE,NADA);
        case PROPOSICION_RETORNO: return cons(CRETURN,NADA);
        case PROPOSICION_SELECCION: return cons(CIF,NADA);
        case TERMINO: return cons(CPAR_ABR,CNEG|CCONS_CAR|CCONS_ENT|CCONS_FLO|CCONS_STR|CIDENT);
        case UNIDAD_TRADUCCION: return cons(CVOID|CCHAR|CINT|CFLOAT,NADA);
        case VARIABLE: return cons(NADA,CIDENT);
        case LISTA_DECLARACIONES_INIT: return cons(NADA,CIDENT);
    }
}


void test(set cjto1,set cjto2,int n)
{
    if(!in(sbol->codigo,cjto1))
    {

        error_handler(n);
        cjto1= une(cjto1,cjto2);
        while(!in(sbol->codigo,cjto1))
        {
            scanner();
        }
    }
}












