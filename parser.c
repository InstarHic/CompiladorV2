/******************************************************************
            Este es el archivo correspondiente al PARSER
*******************************************************************/

#define EXTERNA extern

#include <stdio.h>
#include <stdlib.h> //Agregada para evitar el warning: "incompatible implicit declaration of build-in function"
#include <string.h> //Agregada para evitar el warning: "incompatible implicit declaration of build-in function" relacionado a strcpy y strcat
#include "codigos.h"
#include "var_globales.h"
#include "ts.h"

/*********** prototipos *************/

void unidad_traduccion();
void declaraciones();
void especificador_tipo();
void especificador_declaracion();
void definicion_funcion();
void declaracion_variable();
void lista_declaraciones_param();
void declaracion_parametro();
void declarador_init();
void lista_declacion_init();
void constante();
void lista_inicializadores();
void lista_proposiciones();
void lista_declaraciones();
void declaracion();
void proposicion();
void proposicion_expresion();
void proposicion_compuesta();
void proposicion_seleccion();
void proposicion_iteracion();
void proposicion_e_s();
void proposicion_retorno();
void variable();
void expresion();
void expresion_asignacion();
void expresion_relacional();
void expresion_simple();
void relacion();
void termino();
void factor();
void llamada_funcion();
void lista_expresiones();


void scanner ();

/********** variables globales ************/



token *sbol;


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
        error_handler(53);
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

            unidad_traduccion();
            if (sbol->codigo != CEOF)
            {
                error_handler(41);
                error_handler(COD_IMP_ERRORES);   //no es una decl valida
            }
            //else
            {
                //Controlamos las condiciones que debe cumplir el Main
                int posEnTablaMain;
                posEnTablaMain = en_nivel_actual("main");
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
        }
        else
        {
            // Entra por esta rama si el parametro que se le da a ucc es "-g"
        }
    }

}


/********* funciones del parser ***********/

void unidad_traduccion()
{

    while (sbol->codigo == CVOID || sbol->codigo == CCHAR ||
            sbol->codigo == CINT || sbol->codigo == CFLOAT)
        declaraciones();
}

void declaraciones()
{

    especificador_tipo();
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
    especificador_declaracion();
}

void especificador_tipo()
{

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
}

void especificador_declaracion()
{

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
        definicion_funcion();
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
        declaracion_variable();
        break;

    }

    default:
        error_handler(18);
    }

}

void definicion_funcion()
{

    if (sbol->codigo == CPAR_ABR) scanner();
    else error_handler(19);

    if (sbol->codigo == CVOID || sbol->codigo == CCHAR ||
            sbol->codigo == CINT || sbol->codigo == CFLOAT)
        lista_declaraciones_param();

    if (sbol->codigo == CPAR_CIE) scanner();
    else error_handler(20);

    proposicion_compuesta();

}

void lista_declaraciones_param()
{

    declaracion_parametro();

    while (sbol->codigo ==CCOMA)
    {
        scanner();

        declaracion_parametro();
    }
}

void declaracion_parametro()
{
    int fueAmp=0; // usado para indicar el tipo de pasaje (1: referencia, 0: valor)

    especificador_tipo();

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
            error_handler(50);
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
                error_handler(51);
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
}

void lista_declaraciones_init()
{

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

    declarador_init();

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

        declarador_init();
    }

}


void declaracion_variable()
{

    declarador_init();

    if (sbol->codigo == CCOMA)
    {
        scanner();

        lista_declaraciones_init();
    }

    if (sbol->codigo == CPYCOMA) scanner();
    else error_handler(22);

}


void declarador_init()
{

    switch (sbol->codigo)
    {
    case CASIGNAC:
    {
        scanner();
        constante();
        break;
    }
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
                constante();
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

            lista_inicializadores();

            if (sbol->codigo == CLLA_CIE) scanner();
            else error_handler(24);

        }

        break;
    }
    }
}

void lista_inicializadores()
{

    constante();

    while (sbol->codigo == CCOMA)
    {
        scanner();

        constante();
    }

}


void proposicion_compuesta()
{

    if (sbol->codigo == CLLA_ABR)
    {
        scanner();
    }
    else error_handler(23);

    if (sbol->codigo == CVOID || sbol->codigo == CCHAR ||
            sbol->codigo == CINT || sbol->codigo == CFLOAT)

        lista_declaraciones();

    if (sbol->codigo == CLLA_ABR || sbol->codigo == CMAS ||
            sbol->codigo == CMENOS || sbol->codigo == CIDENT ||
            sbol->codigo == CPAR_ABR || sbol->codigo == CNEG ||
            sbol->codigo == CCONS_ENT || sbol->codigo == CCONS_FLO ||
            sbol->codigo == CCONS_CAR || sbol->codigo == CCONS_STR ||
            sbol->codigo == CIF || sbol->codigo == CWHILE ||
            sbol->codigo == CIN || sbol->codigo == COUT ||
            sbol->codigo == CPYCOMA || sbol->codigo == CRETURN)

        lista_proposiciones();

    if (sbol->codigo == CLLA_CIE)
    {
        // Controlo que en caso de que la llave cierre el cuerpo de una funcion, si dicha funcion necesita retorno lo tenga

        pop_nivel();
        int posUltimoIdentificadorFuncion = en_nivel_actual(ultimoIdentificadorFuncion);
        if (posUltimoIdentificadorFuncion >= 0)
        {
            if (ts[posUltimoIdentificadorFuncion].ets->ptr_tipo != en_tabla("void")) //distinto de void
            {
                if (!existeReturn)
                {
                    error_handler(37);
                    //error_handler(COD_IMP_ERRORES);
                }
            }
            else
            {
                if (existeReturn)
                {
                    error_handler(45);
                    //error_handler(COD_IMP_ERRORES);
                }

            }
            existeReturn=0;

        }

        scanner();
    }
    else error_handler(24);

}
void lista_declaraciones()
{

    declaracion();

    while (sbol->codigo == CVOID || sbol->codigo == CCHAR ||
            sbol->codigo == CINT || sbol->codigo == CFLOAT)

        declaracion();

}

void declaracion()
{

    especificador_tipo();

    lista_declaraciones_init();

    if (sbol->codigo == CPYCOMA) scanner();
    else error_handler(22);

}

void lista_proposiciones()
{

    proposicion();

    while (sbol->codigo == CLLA_ABR || sbol->codigo == CMAS ||
            sbol->codigo == CMENOS || sbol->codigo == CIDENT ||
            sbol->codigo == CPAR_ABR || sbol->codigo == CNEG ||
            sbol->codigo == CCONS_ENT || sbol->codigo == CCONS_FLO ||
            sbol->codigo == CCONS_CAR || sbol->codigo == CCONS_STR ||
            sbol->codigo == CIF || sbol->codigo == CWHILE ||
            sbol->codigo == CIN || sbol->codigo == COUT ||
            sbol->codigo == CPYCOMA || sbol->codigo == CRETURN)

        proposicion();

}

void proposicion()
{

    switch (sbol->codigo)
    {
    case CLLA_ABR:
    {
        pushTB(); // en definicion de fcion no se llama a esto, y por lo tanto los bloques se dan de alta correctamente
        proposicion_compuesta();
        break;
    }
    case CWHILE:
        proposicion_iteracion();
        break;
    case CIF:
        proposicion_seleccion();
        break;
    case CIN:
    case COUT:
        proposicion_e_s();
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
        proposicion_expresion();
        break;
    case CRETURN:
        proposicion_retorno();
        break;
    default:
        error_handler(25);
    }
}

void proposicion_iteracion()
{

    if (sbol->codigo == CWHILE) scanner();
    else error_handler(26);

    if (sbol->codigo == CPAR_ABR)
    {
        scanner();

    }
    else error_handler(19);

    expresion();

    if (sbol->codigo == CPAR_CIE) scanner();
    else error_handler(20);

    proposicion();

}


void proposicion_seleccion()
{

    if (sbol->codigo == CIF) scanner();
    else error_handler(27);

    if (sbol->codigo == CPAR_ABR)
    {
        scanner();

    }
    else error_handler(19);

    expresion();

    if (sbol->codigo == CPAR_CIE) scanner();
    else error_handler(20);

    proposicion();

    if (sbol->codigo == CELSE)
    {
        scanner();

        proposicion();
    }

}

void proposicion_e_s()
{

    switch(sbol->codigo)
    {
    case CIN:
    {
        flagUltimoIDError=0;
        scanner();

        if (sbol->codigo == CSHR) scanner();
        else error_handler(28);
        /// ¿Aqui no se deberia actualizar el valor de ultimoId?
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
        variable();
        /// Como variable() no pide el proximo token, token sigue teniendo el identificador de la variable causando un error.

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
            variable();
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
        expresion();
        while (sbol->codigo == CSHL)
        {
            scanner();
            expresion();
        }
        if (sbol->codigo == CPYCOMA) scanner();
        else error_handler(22);
        break;
    }
    default:
        error_handler(25);
    }
}


void proposicion_retorno()
{

    scanner();
    expresion();
    if (sbol->codigo == CPYCOMA) scanner();
    else error_handler(22);

    existeReturn=1;
    /// Aqui colocaremos el codigo para controlar el tipo de retorno. i think.

}


void proposicion_expresion()
{

    if (sbol->codigo == CMAS || sbol->codigo == CMENOS ||
            sbol->codigo == CIDENT ||
            sbol->codigo == CPAR_ABR || sbol->codigo == CNEG ||
            sbol->codigo == CCONS_ENT || sbol->codigo == CCONS_FLO ||
            sbol->codigo == CCONS_CAR || sbol->codigo == CCONS_STR)

        expresion();

    if (sbol->codigo == CPYCOMA) scanner();
    else error_handler(22);
}


void expresion()
{

    expresion_simple();

    switch (sbol->codigo)
    {
    case CASIGNAC:
    {
        scanner();
        expresion();
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
        expresion();
        break;
    }
    }
}


void expresion_simple()
{

    if (sbol->codigo == CMAS || sbol->codigo == CMENOS) scanner();

    termino();

    while (sbol->codigo == CMAS || sbol->codigo == CMENOS || sbol->codigo == COR)
    {
        scanner();
        termino();
    }

}

void termino()
{

    factor();

    while (sbol->codigo == CMULT || sbol->codigo == CDIV || sbol->codigo == CAND)
    {
        scanner();
        factor();
    }

}

void factor()
{

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
            variable();
        }
        else
        {
            if (ts[en_tabla(sbol->lexema)].ets->clase == CLASVAR)
            {
                variable();
            }
            else
            {
                if(ts[en_tabla(sbol->lexema)].ets->clase == CLASFUNC)
                {
                    llamada_funcion();
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
                                    expresion();
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

        /*
            if(sbol->codigo!=CIDENT)
            {
                error_handler(16);
            }
            else
            {   // hacemos scanner() para poder saber si se trata de una funcion o de una vble
                strcpy(ultimoID,sbol->lexema);
                scanner();
            }
            if(sbol->codigo==CPAR_ABR)
            {
                if(en_tabla(ultimoID)==NIL)
                {
                    error_handler(42);
                }
                llamada_funcion();
            }
            else
            {
                if(en_tabla(ultimoID)==NIL)
                {
                    error_handler(16);
                }
                variable();
            }

            break;
            */
    }
    case CCONS_ENT:
    case CCONS_FLO:
    case CCONS_CAR:
        constante();
        break;
    case CCONS_STR:
        scanner();
        break;
    case CPAR_ABR:
    {
        scanner();
        expresion();
        if (sbol->codigo == CPAR_CIE) scanner();
        else error_handler(20);
        break;
    }
    case CNEG:
    {
        scanner();
        expresion();
        break;
    }
    default:
        error_handler(31);
    }

}

void variable()
{

    /*

        if(en_tabla(ultimoID)==NIL)
        {
            // Insertamos la vble no declarada en la TS------------------------------


            //---------------------------------------------------------
        }
    */
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
        expresion();
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
}

void llamada_funcion()
{

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

        {flagLlamadaFcion=1;
        lista_expresiones();
        flagLlamadaFcion=0;
        }

    if (sbol->codigo == CPAR_CIE)
    {
        if(flagExisteIDDeclaracion)
        {
            if(cantParReales!=ts[en_tabla(funcionActual)].ets->desc.part_var.sub.cant_par) //controlamos que la cant de pars actuales concuerde con la de formales
            {
                error_handler(52);
            }
        }

        scanner();
    }
    else error_handler(20);

}

void lista_expresiones()
{

    expresion();
    cantParReales++; //por cada parametro aumentamos el contador

    while (sbol->codigo == CCOMA)
    {
        scanner();

        expresion();
        cantParReales++;
    }

}

void constante()
{

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

}














