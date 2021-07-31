/*
 * Paulina Vara Figueroa
 * 2019
 * Solución sistemas lineales
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
float **matriz = NULL;//Matriz que se irá modificando para solucionar el sistema
float **untouched = NULL;/*Guarda la matriz original en caso de que el usuario
                            quiera solucionar el mismo sistema por otro método*/
float *solucion = NULL;//Para guardar los resultados de cada incógnita
float **componenteXO = NULL;//Para los componentes iniciales "k" en los métodos iterativos
int n;//Número de incógnitas y ecuaciones

//Funciones de impresión
void imprimeMat ()
{
    int i, j;
    for (i=0; i<n; i++)
    {
        for (j=0; j<n+1; j++)
            printf("%11.3f", matriz[i][j]);
        printf("\n");
    }
}
void imprimeSolucion ()
{
    int i;
    printf("\n");
    for (i=0; i<n; i++)
        printf("\tX%d = %f\n",i+1,solucion[i]);
    printf("\n");
}
void printX0 ()
{
    int i;
    for (i=0; i<n; i++)
    {
        printf("x%d = %f\n", i+1, componenteXO[i][1]);
    }
}
void mostrar_despejeOrdenado ()
{
    int i, j;
    for (i=0; i<n; i++)
    {
        printf("X%d = ",i+1);
        for (j=0; j<n; j++)
        {
            if(matriz[i][j] < 0)
                printf(" - ");
            else if(j != 0)
                printf(" + ");
            printf("%5.3f X%d",fabs(matriz[i][j]), j+1);
        }
        if(matriz[i][j] < 0)
            printf(" - ");
        else
            printf(" + ");
        printf("%5.3f\n",fabs(matriz[i][j]), j+1);
    }
}

//Funciones generar matriz
void solicitarIncog()
{
    n=-1;
    while(n<=0)
    {
        printf("\nIndique la cantidad de incognitas y ecuaciones:   ");
        scanf("%d",&n);
        system("cls");
        if(n<=0)
            printf("----------  Cantidad invalida de incognitas ----------\n\n");
    }
}
void datosLlenadoMatriz ()
{
    int i, j;
    matriz = (float **) malloc (n*sizeof(float *));//Crea el espacio para los renglones de la matriz
    untouched = (float **) malloc (n*sizeof(float *));//Crea una copia para guardar solo la matriz ingresada originalmente
    for (i=0; i < n; i++) 
    {
        matriz[i]=(float *) malloc ((n+1)*sizeof(float)); //Crea el espacio para las columnas de la matriz
        untouched[i]=(float *) malloc ((n+1)*sizeof(float));//Copia matriz original para no perderla
    }
    for (i=0; i<n; i++)//Llenado de matriz
    {
        printf("Ingrese los coeficientes de las variables de la ecuacion %d:\n", i+1);
        for (j=0; j<n; j++)
        {
            printf("Coeficiente de X%d:   ", j+1);
            scanf("%f", &matriz[i][j]);
            untouched[i][j] = matriz[i][j];
        }
        printf("Resultado de la ecuacion %d:   ", i+1);
        scanf("%f",&matriz[i][j]);
        untouched[i][j] = matriz[i][j];
    }
    system("cls");
    printf("Los datos capturados se encuentran a continuacion:\n");
    imprimeMat(matriz);
}
void liberarMatriz ()
{
    int i;
    for (i=0;i<n;i++)
        free (matriz[i]);
    free(matriz);
}

//Funciones de ayuda
void despeje ()
{
    int i, j;
    float save_sol;
    for(i=n-1;i>=0;i--)
        {
            j=n;
            save_sol = matriz[i][j];
            for(j=n;j-1>i;j--)
            {
                solucion[i] = save_sol - matriz[i][j-1]*solucion[j-1];
                save_sol = solucion[i];
            }
            solucion[i] = save_sol / matriz[i][i];
        }
}
int despejeIterativos () //realiza el despeje e indica si se despejó adecuadamente (1) o no (0)
{
    int i, j;
    float divisor_despeje;
    for (i=0; i<n && matriz[i][i] != 0; i++)
    {
        divisor_despeje = matriz[i][i];
        for (j=0; j<n; j++)
            matriz[i][j] = matriz[i][j] / (-divisor_despeje);
        matriz[i][j] = matriz[i][j] / divisor_despeje;
    }
    if(i < n)
    {
        return 0;
    }
    else
    {
        for (i=0; i<n; i++)
        {
            matriz[i][i] = 0;
        }
        return 1;
    }
}
int menuMetodos ()
{
    int opc=-1;
    while(opc<0 || opc>5)
    {
        printf("\nQue metodo desea utilizar para solucionar el sistema?\n");
        printf("1.  Metodo gaussiano\n");
        printf("2.  Metodo de pivoteo maximo de columna\n");
        printf("3.  Metodo de pivoteo escalado de columna\n");
        printf("4.  Metodo de Jacobi\n");
        printf("5.  Metodo Gauss-Seidel\n");
        printf("\n0.  Salir\n");
        printf("Opcion:    ");
        scanf("%d",&opc);
        if(opc<0 || opc>5)
            printf("\n---Por favor elija otra opcion---\n");
    }
}
int menuContOSalir ()
{
    int opc_cont_sal = -1;
    while (opc_cont_sal < 1 || opc_cont_sal > 3)
    {
        printf("\nDesea:\n1.   Resolver el mismo sistema con otro metodo\n2.   Resolver otro sistema de ecuaciones\n3.   Salir del programa\nOpcion:   ");
        scanf("%d",&opc_cont_sal);
        if(opc_cont_sal < 1 || opc_cont_sal > 3)
            printf("\n---Por favor vuelva a ingresar la opcion---\n");
    }
}
void solicitaXO ()//Para iterativos
{
    int i;
    printf("Ingrese los valores a considerar para XO:\n");
    componenteXO = (float **) malloc (n*sizeof(float *));
    for (i=0; i < n; i++) 
        componenteXO[i]=(float *) malloc (3*sizeof(float)); 
    printf("Indique los valores del vector inicial:\n");
    for (i=0; i<n; i++)
    {
        printf("Xo[%d]:    ", i+1);
        scanf("%f",&componenteXO[i][0]);
        componenteXO[i][2] = componenteXO[i][0];
    }
}
float max_AbsDiff ()//Obtiene ||x-Xo|| para calcular el error
{
    float maximo = 0.0;
    int i;
    for (i=0; i<n; i++)
    {
        if(fabs(componenteXO[i][1]-componenteXO[i][0])>maximo)
            maximo = fabs(componenteXO[i][1]-componenteXO[i][0]);
    }
    return maximo;
}

//Funciones de métodos de solución
void metodoGauss ()
{
    int i, j, k, p, foundP;
    float valTemp, valMulti, solucionUnica = 1, save_sol = 0;

    for(i=0; (i<n && solucionUnica); i++)
    {
        foundP=-1;
        for (p=i; p<n; p++)
        {
            if (matriz[p][i]!=0)
            {
                foundP = p;
                p=n;
            }
        }
        if(foundP==-1)
        {
            printf("No existe solucion unica\n");
            solucionUnica = 0;
        }
        else//Si es posible hallar solucion única:
        {
            if(foundP!=i)
            {
                for (j=0; j<=n; j++)
                {
                    valTemp = matriz [i][j];
                    matriz[i][j] = matriz[foundP][j];
                    matriz[foundP][j] = valTemp;
                }
            }
            for (j=i+1;j<n;j++)
            {
                valMulti = matriz[j][i] / matriz [i][i];
                for (k=0; k<n+1; k++)
                    matriz[j][k] = matriz[j][k] - valMulti*matriz[i][k];
            }
        }
        
    }
    if(matriz[n-1][n-1] == 0 && solucionUnica)
    {
        printf("No existe solucion unica\n");
        solucionUnica = 0;
    }
    else if(solucionUnica)
    {
        despeje();
        printf("\nEl metodo Gaussiano genera la siguiente matriz para solucionar el sistema:\n\n");
        imprimeMat();
        printf("\nLa solucion hallada para el sistema de ecuaciones es:\n");
        imprimeSolucion();
    }
}
void pivoteoMaximoCol ()
{
    int i, j, k, p, maxP;
    float valTemp, valMulti, solucionUnica = 1, save_sol = 0;

    for(i=0; (i<n && solucionUnica); i++)
    {
        maxP = i;
        for (p=i; p<n; p++)
        {
            if(fabs(matriz[p][i]) > fabs(matriz[maxP][i]))
                maxP = p;
        }
        if(matriz[maxP][i] == 0)//Primera validación de solución única
        {
            printf("No existe solucion unica\n");
            solucionUnica = 0;
        }
        else//Si es posible hallar solucion única:
        {
            if(maxP != i)
            {
                for (j=0; j<=n; j++)
                {
                    valTemp = matriz [i][j];
                    matriz[i][j] = matriz[maxP][j];
                    matriz[maxP][j] = valTemp;
                }
            }
            for (j=i+1;j<n;j++)
            {
                valMulti = matriz[j][i] / matriz [i][i];
                for (k=0; k<n+1; k++)
                    matriz[j][k] = matriz[j][k] - valMulti*matriz[i][k];
            }
        }
        
    }
    if(matriz[n-1][n-1] == 0 && solucionUnica)//Segunda validación de solución única
    {
        printf("No existe solucion unica\n");
        solucionUnica = 0;
    }
    else if(solucionUnica)
    {
        despeje();
        printf("\nEl metodo con pivoteo maximo de columna genera la siguiente matriz para solucionar el sistema:\n\n");
        imprimeMat();
        printf("\nLa solucion hallada para el sistema de ecuaciones es:\n");
        imprimeSolucion();
    }
}
void pivoteoEscaladoCol ()
{
    int i, j, k, p, pivo;
    float valTemp, valMulti, solucionUnica = 1, save_sol = 0;
    float *s = NULL;
    s = (float *) malloc (n*sizeof(float));

    for(i=0; (i<n && solucionUnica); i++)
    {
        for (j=i;j<n;j++)
            s[j] = 0;
        for (k=i;k<n;k++)
        {
            for (j=0; j<n; j++)
            {
                if(fabs(matriz[k][j]) > s[k])
                    s[k] = matriz[k][j];
            }
            if(s == 0)//Primera validación de solución única
            {
                printf("No existe solucion unica\n");
                solucionUnica = 0;
                k = n;
            }
        }
        pivo = i;
        if(solucionUnica)//Si solución única es posible (if)
        {
            for (p=i; p<n; p++)
            {
                if(fabs(matriz[p][i])/s[p] > fabs(matriz[pivo][i])/s[pivo])
                    pivo = p;
            }
            if(matriz[pivo][i] == 0)//Segunda validación de solución única
            {
                printf("No existe solucion unica\n");
                solucionUnica = 0;
            }
            else//Si es posible hallar solucion única (else)
            {
                if(pivo != i)
                {
                    for (j=0; j<=n; j++)
                    {
                        valTemp = matriz [i][j];
                        matriz[i][j] = matriz[pivo][j];
                        matriz[pivo][j] = valTemp;
                    }
                }
                for (j=i+1;j<n;j++)
                {
                    valMulti = matriz[j][i] / matriz [i][i];
                    for (k=0; k<n+1; k++)
                        matriz[j][k] = matriz[j][k] - valMulti*matriz[i][k];
                }
            }//else
        }//if
        
    }
    if(matriz[n-1][n-1] == 0 && solucionUnica)//Tercera validación de solución única
    {
        printf("No existe solucion unica\n");
        solucionUnica = 0;
    }
    else if(solucionUnica)
    {
        despeje();
        printf("\nEl metodo con pivoteo escalado de columna genera la siguiente matriz para solucionar el sistema:\n\n");
        imprimeMat();
        printf("\nLa solucion hallada para el sistema de ecuaciones es:\n");
        imprimeSolucion();
    }
}
void metodoJacobi ()
{
    int k, i, j, num_iteracion, flag_tol;
    float tol, error;
    if(despejeIterativos())//Si se logro despejar
    {
        solicitaXO();
        printf("Indique el valor de la tolerancia:   ");
        scanf("%f",&tol);
        printf("Indique el numero maximo de iteraciones:   ");
        scanf("%d",&num_iteracion);
        k = 1;
        flag_tol = 1;
        while (k<=num_iteracion && flag_tol)
        {
            for (i=0; i<n; i++)
            {
                componenteXO[i][1] = 0;
                for (j=0; j<n; j++)
                    componenteXO[i][1] += matriz[i][j]*componenteXO[j][0];
                componenteXO[i][1] += matriz[i][j];
            }
            error = max_AbsDiff();
            if(error < tol)
                flag_tol = 0;//Se alcanzó el resultado
            else
            {
                k++;
                for (i=0; i<n; i++)
                    componenteXO[i][0] = componenteXO[i][1];
            }
        }//while
        if(k > num_iteracion)
        {
            printf("\nObteniendo el despeje:\n\n");
            mostrar_despejeOrdenado();
            printf("\nEl metodo no logro obtener el resultado en %d iteraciones\n\n", num_iteracion);
        }
        else
        {
            printf("\nObteniendo el despeje:\n\n");
            mostrar_despejeOrdenado();
            printf("\nEl metodo de Jacobi encontro la siguiente solucion para el sistema despues de %d iteraciones\n\n",k);
            printX0();
        }
    }//Si se logro despejar
    else
    {
        printf("Imposible resolver por metodo de Jacobi, la matriz ingresada no permite realizar el despeje\n");
    }
}
void metodoGaussSeidel ()
{
    int k, i, j, num_iteracion, flag_tol, flag;
    float tol, error;
    flag = despejeIterativos();
    if(flag)//Si se logra despejar
    {
        solicitaXO();
        printf("Indique el valor de la tolerancia:   ");
        scanf("%f",&tol);
        printf("Indique el numero maximo de iteraciones:   ");
        scanf("%d",&num_iteracion);
        k = 1;
        flag_tol = 1;
        while (k<=num_iteracion && flag_tol)
        {
            for (i=0; i<n; i++)
            {
                componenteXO[i][1] = 0;
                for (j=0; j<n; j++)
                    componenteXO[i][1] += matriz[i][j]*componenteXO[j][2];
                componenteXO[i][1] += matriz[i][j];
                componenteXO[i][2] = componenteXO[i][1];
            }
            error = max_AbsDiff();
            if(error < tol)
                flag_tol = 0;//Se alcanzó el resultado
            else
            {
                k++;
                for(i=0; i<n; i++)
                    componenteXO[i][0] = componenteXO[i][2];
            }
        }//while
        if(k > num_iteracion)
        {
            printf("\nObteniendo el despeje:\n\n");
            mostrar_despejeOrdenado();
            printf("\nEl metodo no logro obtener el resultado en %d iteraciones\n\n", num_iteracion);
        }
        else
        {
            printf("\nObteniendo el despeje:\n\n");
            mostrar_despejeOrdenado();
            printf("\nEl metodo Gauss-Seidel encontro la siguiente solucion para el sistema despues de %d iteraciones\n\n",k);
            printX0();
        }
    }//Si se logra despejar
    else
    {
        printf("Imposible resolver por metodo Gauss-Seidel, la matriz ingresada no permite realizar el despeje\n");
    }
}
int main ()
{
    int i, j, opc=-1;
    printf("SOLUCION DE SISTEMAS LINEALES\n");
    solicitarIncog();//Solicita cantidad de incognitas
    datosLlenadoMatriz();
    solucion = (float *) malloc (n*sizeof(float));
    do
    {
        opc = menuMetodos();
        switch(opc)
        {
            case 0:
            printf("\nFin del programa\n");
            break;
            case 1:
            metodoGauss();
            switch (menuContOSalir())
            {
                case 1:
                    for (i=0;i<n;i++)
                    {
                        for(j=0;j<n+1;j++)
                            matriz[i][j] = untouched[i][j];
                    }
                    free (solucion);
                    solucion = (float *) malloc (n*sizeof(float));
                    system("cls");
                    printf("Considerando la matriz:\n\n");
                    imprimeMat();
                break;
                case 2:
                    liberarMatriz();
                    free (solucion);
                    solicitarIncog();//Solicita cantidad de incognitas
                    datosLlenadoMatriz();
                    solucion = (float *) malloc (n*sizeof(float));
                break;
                case 3:
                    printf("\nFin del programa\n");
                    opc = 0;
                break;
            }
            break;
            case 2:
            pivoteoMaximoCol();
            switch (menuContOSalir())
            {
                case 1:
                    for (i=0;i<n;i++)
                    {
                        for(j=0;j<n+1;j++)
                            matriz[i][j] = untouched[i][j];
                    }
                    free (solucion);
                    solucion = (float *) malloc (n*sizeof(float));
                    system("cls");
                    printf("Considerando la matriz:\n\n");
                    imprimeMat();
                break;
                case 2:
                    liberarMatriz();
                    free (solucion);
                    solicitarIncog();//Solicita cantidad de incognitas
                    datosLlenadoMatriz();
                    solucion = (float *) malloc (n*sizeof(float));
                break;
                case 3:
                    printf("\nFin del programa\n");
                    opc = 0;
                break;
            }
            break;
            case 3:
            pivoteoEscaladoCol();
            switch (menuContOSalir())
            {
                case 1:
                    for (i=0;i<n;i++)
                    {
                        for(j=0;j<n+1;j++)
                            matriz[i][j] = untouched[i][j];
                    }
                    free (solucion);
                    solucion = (float *) malloc (n*sizeof(float));
                    system("cls");
                    printf("Considerando la matriz:\n\n");
                    imprimeMat();
                break;
                case 2:
                    liberarMatriz();
                    free (solucion);
                    solicitarIncog();//Solicita cantidad de incognitas
                    datosLlenadoMatriz();
                    solucion = (float *) malloc (n*sizeof(float));
                break;
                case 3:
                    printf("\nFin del programa\n");
                    opc = 0;
                break;
            }
            break;
            case 4:
            metodoJacobi();
            for (i=0; i<2; i++)
                free(componenteXO[i]);
            free (componenteXO);
            switch (menuContOSalir())
            {
                case 1:
                    for (i=0;i<n;i++)
                    {
                        for(j=0;j<n+1;j++)
                            matriz[i][j] = untouched[i][j];
                    }
                    free (solucion);
                    solucion = (float *) malloc (n*sizeof(float));
                    system("cls");
                    printf("Considerando la matriz:\n\n");
                    imprimeMat();
                break;
                case 2:
                    liberarMatriz();
                    free (solucion);
                    solicitarIncog();//Solicita cantidad de incognitas
                    datosLlenadoMatriz();
                    solucion = (float *) malloc (n*sizeof(float));
                break;
                case 3:
                    printf("\nFin del programa\n");
                    opc = 0;
                break;
            }
            break;
            case 5:
            metodoGaussSeidel();
            for (i=0; i<3; i++)
                free(componenteXO[i]);
            free (componenteXO);
            switch (menuContOSalir())
            {
                case 1:
                    for (i=0;i<n;i++)
                    {
                        for(j=0;j<n+1;j++)
                            matriz[i][j] = untouched[i][j];
                    }
                    free (solucion);
                    solucion = (float *) malloc (n*sizeof(float));
                    system("cls");
                    printf("Considerando la matriz:\n\n");
                    imprimeMat();
                break;
                case 2:
                    liberarMatriz();
                    free (solucion);
                    solicitarIncog();//Solicita cantidad de incognitas
                    datosLlenadoMatriz();
                    solucion = (float *) malloc (n*sizeof(float));
                break;
                case 3:
                    printf("\nFin del programa\n");
                    opc = 0;
                break;
            }
            break;
        }
    } while (opc != 0);
    free(solucion);
    liberarMatriz();
    system ("pause");
    return 0;
}
