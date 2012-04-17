#include <iostream.h>
#include <stdio.h>
#include <conio.h>
#include <graphics.h>
#include <string.h>
#include <math.h>

enum tecla { ANTERIOR=45/*(-)*/, SEGUINTE=43 /*(+)*/, ENTER=13,ESCAPE=27,BACKSPACE=8 ,TAB=9};
enum luz { NADA, CIMA_DIREITA, CIMA_ESQUERDA, BAIXO_DIREITA, BAIXO_ESQUERDA};
enum bool {FALSO, VERDADEIRO};
enum input_type {BOTAO,CAIXA};
enum val {INTEIROS, LETRAS, ALFANUMERICOS, DECIMAIS,ASCIICHAR };
/*
	INTEIROS-> 0 a 9	LETRAS->'a' a 'z', 'A' a 'Z'
	ALFANUMERICOS-> INTEIROS + LETRAS
	DECIMAIS-> INTEIROS + '.'
	ASCIICHAR-> ALFANUMERICOS + '_' + ' ' + '|' + '&'
*/
enum valin {TOPO, CENTRO, BASE};
enum halin {ESQUERDA, MEIO};


//ponto cartesiano
class ponto
{
   public:
   //CAMPOS
   int x, y;
   //CONSTRUTORES
   ponto (int PX=0, int PY=0)   //construtor especifico & default
   { redimensiona(PX,PY);}
   //OPERADORES
   ponto operator + (ponto SOMANDO)
   {
	ponto Temp;
	Temp.x = x + SOMANDO.x;
	Temp.y = y + SOMANDO.y;
	return(Temp);
   }
   ponto operator += (ponto ACRESCENTANDO)
   {
	x = x + ACRESCENTANDO.x;
	y = y + ACRESCENTANDO.y;
	return (*this);
   }
   ponto operator -= (ponto DIMINUINDO)
   {
	x = x - DIMINUINDO.x;
	y = y - DIMINUINDO.y;
	return (*this);
   }
   ponto operator - (ponto SUBTRAINDO)
   {
	ponto Temp;
	Temp.x = x - SUBTRAINDO.x;
	Temp.y = y - SUBTRAINDO.y;
	return(Temp);
   }
   ponto operator = (ponto IGUALANDO)
   {
	x = IGUALANDO.x;
	y = IGUALANDO.y;
	return (*this);
   }
   //UTILIDADES
   void redimensiona(int PX, int PY)
   {
	x = PX; y = PY;
   }
   void redimensiona (ponto PT)
   {
	x = PT.x ; y = PT.y ;
   }
   void desenha_me(int COR=getcolor())
   {
	if (COR!=getcolor())
		setcolor(COR);
	line(x,y,x,y);
   }
};


ponto calcula_ponto_desenho(valin V_AL, halin H_AL, ponto ORIGEM, int ALT_CXPR, int LAR_CXPR, int ALT_CX, int LAR_CX);
inline int altura_menu(int NCONTROLOS, int ALTURAC, int INTERVALO);
double troca_char_int (char CHR);
float valstr (char NUMERO[]);
int modulo (float NUMERO);
float partedecimal (float NUMERO);
int arredonda ( double NUMERO);
int nalgarismos(int NUMERO);
void escreve_texto(ponto ORIGEM, char * TEXTO, int COR=WHITE, int TAMANHO=1);
char * strval (float NUMERO);
//MENUS
int menuINICIAR();
int menuLOGINADM();
int menuADM();
void produtosADMNISTRADOR(int Nprodutos);
void moedasADMNISTRADOR(int Nmoedas);


class linha
{
	/*objectivos puramente gr ficos!!!*/
	public:
	ponto p1, p2;
	linha(int X1=0, int Y1=0,int X2=0,int Y2=0 ): p1(X1,Y1), p2(X2,Y2)
	{}
	linha(ponto P1, ponto P2): p1(P1.x,P1.y),p2(P2.x,P2.y)
	{}
	void redimensiona(linha L1)
	{
		p1 = L1.p1; p2 = L1.p2;
	}
	void redimensiona(ponto P1, ponto P2)
	{
		p1 = P1; p2 = P2;
	}
	void desenha_me(int ESPESSURA, int COR=getcolor())
	{
		int Contador;
		if (COR!=getcolor())
			setcolor(COR);
		if (p1.y==p2.y) //linha horizontal
		{
			for (Contador=0; Contador<=ESPESSURA; Contador++)
				line(p1.x,p1.y+Contador,p2.x,p2.y+Contador);
		}
		if (p1.x==p2.x) //linha vertical
		{
			for (Contador=0; Contador<=ESPESSURA; Contador++)
				line(p1.x+Contador,p1.y,p2.x+Contador,p2.y);

		}
	}
};

//quadrilatero - figura de 4 lados
class quadrilatero
{
   public:
   //CAMPOS
   ponto pt1, //ponto superior esquerdo->origem
    pt2, //ponto superior direito
    pt3, //ponto inferior esquerdo
	pt4, //ponto inferior direito
    centro; //guarda o valor do centro do quadrilatero
   //CONSTRUTORES
   /* Nestes construtores, os pontos ou as coordenadas sao dadas na seguinte ordem:
   ponto superior esquerdo { ponto _ponton_ } ou { int _xn_, int _yn_ }
   ponto superior direito   - idem
   ponto inferior esquerdo  - idem
   ponto inferior direito   - idem
   */
   quadrilatero(ponto P1, ponto P2, ponto P3, ponto P4):pt1(P1.x,P1.y),pt2(P2.x,P2.y),pt3(P3.x,P3.y),pt4(P4.x,P4.y)    //construtor especifico 1 (c/pontos)
   {
	   calcula_centro();
   }
   quadrilatero (ponto P1, int ALTURA, int LARGURA ): pt1(P1.x,P1.y),pt2(P1.x + LARGURA, P1.y),pt3(P1.x , pt2.y + ALTURA),pt4(pt2.x , pt3.y) //construtor especifico 3 (c/ ponto superior esquerdo e altura e largura)
   {
	calcula_centro();
   }
   quadrilatero (int X1=0, int Y1=0, int ALTURA=10, int LARGURA=10 ): pt1(X1,Y1),pt2(X1 + LARGURA, Y1),pt3(X1 , pt2.y + ALTURA),pt4(pt2.x , pt3.y) //construtor especifico 3 (c/ ponto superior esquerdo e altura e largura)
   {
	calcula_centro();
   }
   //UTILIDADES
   void calcula_centro()	//calcula o centro de 1 quadrilatero regular
   {
	   int LarguraA = pt2.x - pt1.x, LarguraB = pt4.x - pt3.x;
	   int AlturaA = pt3.y-pt1.y, AlturaB = pt4.y-pt2.y;
	   if ((LarguraA==LarguraB) && (AlturaA==AlturaB )) //condicoes para quadrilatero regular (quadrado/rectangulo)
	   centro.redimensiona( LarguraA/2+pt1.x, AlturaA/2+pt1.y);
	   else
	   centro.redimensiona(-1,-1);
   }
   void desenha_me(int COR = getcolor())  //desenha no ecra o quadrado
   {
	   if (COR!=getcolor())
		setcolor(COR);
	   line(pt1.x,pt1.y,pt2.x,pt2.y);line(pt3.x,pt3.y,pt4.x,pt4.y);
	   line(pt1.x,pt1.y,pt3.x,pt3.y);line(pt2.x,pt2.y,pt4.x,pt4.y);
   }
   virtual void redimensiona(ponto P1 , ponto P2 , ponto P3 , ponto P4)
   {
	pt1 = P1; pt2 = P2; pt3 = P3 ; pt4 = P4;
	calcula_centro();
   }
   virtual void redimensiona(int X1, int Y1,int X2, int Y2,int X3, int Y3,int X4, int Y4)	//construtor especifico 2 (c/ coordenadas , 1 a 1)
   {
	pt1.redimensiona( X1 , Y1 ); pt2.redimensiona( X2 , Y2 );
	pt3.redimensiona( X3 , Y3 ); pt4.redimensiona( X4 , Y4 );
	calcula_centro();
   }
   virtual void redimensiona(ponto P1 , int ALTURA, int LARGURA)
   {
	pt1 = P1;
	pt2.redimensiona( P1.x + LARGURA, P1.y);
	pt3.redimensiona( P1.x , pt2.y + ALTURA);
	pt4.redimensiona( pt2.x , pt3.y);
	calcula_centro();
   }
};

//grelha - matriz de quadrilateros regulares
class grelha
{
	public:
	//CAMPOS
	int numero_linhas, numero_colunas, numero_quadrados;
	quadrilatero * malha;
	//CONSTRUTOR TABULEIRO
	grelha ( ponto ORIGEM , int ALT_QUAD, int LAR_QUAD, int N_LINHAS, int N_COLUNAS)
	{
		int Contador;
		numero_linhas = N_LINHAS; numero_colunas = N_COLUNAS;
		quadrilatero * Ponteiro;
		numero_quadrados = N_LINHAS * N_COLUNAS;
		Ponteiro = new quadrilatero [ numero_quadrados ];
		int Linha = 0 , Coluna = 0; ponto Def(0,0);
		for (Contador = 0; Contador < numero_quadrados ; Contador++)
		{
/*caso seja o 1§ quadrilatero da grelha (CONTADOR==0), o PONTO vai coincidir com a ORIGEM DA GRELHA
SENAO
caso seja o 1§ quadrilatero de uma Linha (desde q nao seja a 1¦), o PONTO vai coincidir com
o PONTO INFERIOR ESQUERDO do 1§ quadrilatero da Linha anterior
SENAO
o PONTO vai coincidir com o PONTO SUPERIOR DIREITO do quadrilatero anterior*/
			if (Contador == 0 /* Linha==0 && Coluna==0 */) Def.redimensiona(ORIGEM);
			if (Linha>0 && Coluna==0) Def.redimensiona(Ponteiro[ N_COLUNAS * (Linha-1) ].pt3);
			if (Coluna>0) Def.redimensiona(Ponteiro [Contador-1].pt2);
/*fazer redimensionamento a cada quadricula*/
			Ponteiro[Contador].redimensiona( Def , ALT_QUAD , LAR_QUAD );
/*leis de incremento*/
			if (Coluna+1 ==N_COLUNAS ) { Coluna = 0 ; Linha++; }
			else Coluna++;
		}
	malha = Ponteiro;
	}
	grelha ( int X1, int Y1 , int ALT_QUAD, int LAR_QUAD, int N_LINHAS, int N_COLUNAS)
	{
		int Contador;
		numero_linhas = N_LINHAS; numero_colunas = N_COLUNAS;
		quadrilatero * Ponteiro;
		numero_quadrados = N_LINHAS * N_COLUNAS;
		Ponteiro = new quadrilatero [ numero_quadrados ];
		int Linha = 0 , Coluna = 0; ponto Def(X1,Y1);
		for (Contador = 0; Contador < numero_quadrados ; Contador++)
		{
/*caso seja o 1§ quadrilatero da grelha (CONTADOR==0), o PONTO vai coincidir com a ORIGEM DA GRELHA
SENAO
caso seja o 1§ quadrilatero de uma Linha (desde q nao seja a 1¦), o PONTO vai coincidir com
o PONTO INFERIOR ESQUERDO do 1§ quadrilatero da Linha anterior
SENAO
o PONTO vai coincidir com o PONTO SUPERIOR DIREITO do quadrilatero anterior*/
			if (Linha>0 && Coluna==0) Def.redimensiona(Ponteiro[ N_COLUNAS * (Linha-1) ].pt3);
			if (Coluna>0) Def.redimensiona(Ponteiro [Contador-1].pt2);
/*fazer redimensionamento a cada quadricula*/
			Ponteiro[Contador].redimensiona( Def , ALT_QUAD , LAR_QUAD );
/*leis de incremento*/
			if (Coluna+1 ==N_COLUNAS ) { Coluna = 0 ; Linha++; }
			else Coluna++;
		}
	malha = Ponteiro;
	}
	//UTILIDADES
	virtual void desenha_me(int COR = getcolor(), int PR_LINHA=0, int ULT_LINHA=(-1))
	{
		int Contador;
		if (COR !=getcolor())
			setcolor(COR);
		if (ULT_LINHA==(-1))
			ULT_LINHA=numero_linhas-1;
		int PrQuadr = devolve_indice(PR_LINHA,0);
		int UltQuadr = devolve_indice(ULT_LINHA,numero_colunas-1 );
		for (Contador=PrQuadr; Contador<=UltQuadr ; Contador++)
			malha[Contador].desenha_me(COR);
	}
	//devolve o indice do elemento de uma grelha a partir da coluna e da linha
	inline int devolve_indice ( int LINHA, int COLUNA )
	{
		return (numero_colunas * LINHA + COLUNA);
	}
	inline int devolve_linha(int INDICE)
	{
		return (INDICE/numero_colunas);
	}
	inline int devolve_coluna(int INDICE)
	{
		return (INDICE - numero_colunas*devolve_linha(INDICE));
	}
};

class grelha_nreg: public grelha
{
	public:
	int * dimcolunas;  //guarda a largura das colunas
	int * dimlinhas;	//guarda a altura das linhas
	grelha_nreg ( ponto ORIGEM , int ALT_QUAD, int LAR_QUAD, int N_LINHAS, int N_COLUNAS):	grelha ( ORIGEM , ALT_QUAD, LAR_QUAD, N_LINHAS, N_COLUNAS)
	{
		dimcolunas = new int[N_COLUNAS];
		dimlinhas = new int[N_LINHAS];
		int Contador;
		for (Contador=0; Contador<N_COLUNAS; Contador++)
			dimcolunas[Contador]=LAR_QUAD;
		for (Contador=0; Contador<N_LINHAS; Contador++)
			dimlinhas[Contador]=ALT_QUAD;
	}
	grelha_nreg ( int X1,int Y1 , int ALT_QUAD, int LAR_QUAD, int N_LINHAS, int N_COLUNAS):grelha ( X1,Y1 , ALT_QUAD, LAR_QUAD, N_LINHAS, N_COLUNAS)
	{
		dimcolunas = new int[N_COLUNAS];
		dimlinhas = new int[N_LINHAS];
		int Contador;
		for (Contador=0; Contador<N_COLUNAS; Contador++)
			dimcolunas[Contador]=LAR_QUAD;
		for (Contador=0; Contador<N_LINHAS; Contador++)
			dimlinhas[Contador]=ALT_QUAD;
	}

	void redimensiona_coluna(int NCOLUNA, int NVLARGURA)
	/*altera a largura de uma coluna e ajusta as outras colunas*/
	{
		dimcolunas[NCOLUNA] = NVLARGURA;
		//redimensiona propria coluna
		int Contador, Contador2;
		//percorre a coluna de cima para bx
		for (Contador=NCOLUNA; Contador<numero_quadrados;Contador+=numero_colunas )
		{
			malha[Contador].redimensiona( malha[Contador].pt1, dimlinhas[devolve_linha(Contador)] , NVLARGURA );
		}
		//ajusta as colunas seguintes
		//percorre as colunas da esq para a dir
		for (Contador=NCOLUNA+1	; Contador<numero_colunas; Contador++)
		{
			//percorre as colunas de cima para bx
			for (Contador2=Contador; Contador2<numero_quadrados; Contador2+=numero_colunas)
			{
				malha[Contador2].redimensiona( malha[Contador2-1].pt2, dimlinhas[devolve_linha(Contador2)], dimcolunas[devolve_coluna(Contador2)]);
			}
		}
	}
	void redimensiona_origem(ponto NVORIGEM)
	{
		ponto VelhaOrigem = malha[0].pt1;
		ponto Dif = NVORIGEM-VelhaOrigem ;
		int Contador;
		for (Contador=0;Contador<numero_quadrados;Contador++)
		{
			malha[Contador].pt1+=Dif;
			malha[Contador].pt2+=Dif;
			malha[Contador].pt3+=Dif;
			malha[Contador].pt4+=Dif;
		}
	}
	int altura()
	{
	/*calcula a altura da grelha*/
		int Contador, Tot=0;
		for (Contador=0; Contador<numero_linhas; Contador++)
			Tot+=dimlinhas[Contador];
		return (Tot);
	}
	int largura()
	{
	/*calula a largura da grelha*/
		int Contador, Tot=0;
		for (Contador=0; Contador<numero_colunas; Contador++)
			Tot+=dimcolunas[Contador];
		return (Tot);
	}
};


class quadrilatero_relevo : public quadrilatero
{
	public:
	int espessura;
	luz direccao_luz;
	linha hs, hi, ve, vd;
/*  linhas horizontais - superior -> (p1,p2)
			 - inferior -> (p3,p4)
	linhas verticais   - esquerda -> (p1,p3)
			 - direita  -> (p2,p4)
*/
	//CONSTRUTOR
	quadrilatero_relevo(ponto P1, ponto P2, ponto P3, ponto P4, int ESPESSURA=2, luz DIRECCAO_LUZ=BAIXO_ESQUERDA):quadrilatero(P1,P2,P3,P4), hs(P1,P2), hi(P3,P4), ve(P1,P3), vd(P2,P4)
	{
		espessura = ESPESSURA;  direccao_luz = DIRECCAO_LUZ ;
	}
	quadrilatero_relevo(ponto ORIGEM, int ALTURA, int LARGURA, int ESPESSURA=2, luz DIRECCAO_LUZ=CIMA_ESQUERDA):quadrilatero(ORIGEM, ALTURA, LARGURA)
	{
		calcula_linhas();
		espessura = ESPESSURA; direccao_luz = DIRECCAO_LUZ   ;
	}
	quadrilatero_relevo(int X1=0, int Y1=0, int ALTURA=10, int LARGURA=10, int ESPESSURA=2, luz DIRECCAO_LUZ=CIMA_ESQUERDA):quadrilatero(X1,Y1, ALTURA, LARGURA)
	{
		calcula_linhas();
		espessura = ESPESSURA; direccao_luz = DIRECCAO_LUZ   ;
	}
	void desenha_me(int COR_BORDA, int COR_INTERNA, bool PINTA_INTERIOR=VERDADEIRO)
	{
		int ContY;
		if (getcolor()!=COR_INTERNA)
			setcolor(COR_INTERNA);
		/*varrimento horizontal para pintar quadrilatero*/
		if (PINTA_INTERIOR)
			for (ContY=pt1.y; ContY<=pt3.y ; ContY++ )
			{
				line(pt1.x,ContY,pt2.x,ContY);
			}
		switch (direccao_luz)
		{
		case NADA:
			hi.desenha_me(0,COR_BORDA);
			ve.desenha_me(0,COR_BORDA);
			vd.desenha_me(0,COR_BORDA);
			hs.desenha_me(0,COR_BORDA);
		break;
		//1§ vem as linhas com a cor borda, depois a sombra - [pormenor t‚cnico]
		case BAIXO_DIREITA:
			hi.desenha_me(0,COR_BORDA);
			vd.desenha_me(0,COR_BORDA);
			ve.desenha_me(espessura,DARKGRAY);
			hs.desenha_me(espessura,DARKGRAY);
		break;
		case BAIXO_ESQUERDA:
			hi.desenha_me(0,COR_BORDA);
			ve.desenha_me(0,COR_BORDA);
			vd.desenha_me(espessura,DARKGRAY);
			hs.desenha_me(espessura,DARKGRAY);
		break;
		case CIMA_DIREITA:
			hs.desenha_me(0,COR_BORDA);
			vd.desenha_me(0,COR_BORDA);
			hi.desenha_me(espessura,DARKGRAY);
			ve.desenha_me(espessura,DARKGRAY);
		break;
		case CIMA_ESQUERDA:
			hs.desenha_me(0,COR_BORDA);
			ve.desenha_me(0,COR_BORDA);
			hi.desenha_me(espessura,DARKGRAY);
			vd.desenha_me(espessura,DARKGRAY);
		break;
		}
	}
	virtual void redimensiona(ponto P1 , ponto P2 , ponto P3 , ponto P4)
	{
		pt1 = P1; pt2 = P2; pt3 = P3 ; pt4 = P4;
		calcula_centro();
		calcula_linhas();
	}
	virtual void redimensiona(int X1, int Y1,int X2, int Y2,int X3, int Y3,int X4, int Y4)	//construtor especifico 2 (c/ coordenadas , 1 a 1)
	{
		pt1.redimensiona( X1 , Y1 ); pt2.redimensiona( X2 , Y2 );
		pt3.redimensiona( X3 , Y3 ); pt4.redimensiona( X4 , Y4 );
		calcula_centro();
		calcula_linhas();
	}
	virtual void redimensiona(ponto P1 , int ALTURA, int LARGURA)
	{
		pt1 = P1;
		pt2.redimensiona( P1.x + LARGURA, P1.y);
		pt3.redimensiona( P1.x , pt2.y + ALTURA);
		pt4.redimensiona( pt2.x , pt3.y);
		calcula_centro();
		calcula_linhas();
	}
	void calcula_linhas()
	{
		hs.redimensiona(pt1,pt2); hi.redimensiona(pt3,pt4);
		ve.redimensiona(pt1,pt3); vd.redimensiona(pt2,pt4);
	}
	quadrilatero_relevo operator = (quadrilatero IGUALANDO)
	{
	   redimensiona(IGUALANDO.pt1,IGUALANDO.pt2,IGUALANDO.pt3,IGUALANDO.pt4);
	   return (*this);
	}
};

class botao_opcao
{
	public:
	char * texto;
	int cor_botao, cor_bordabotao;
	int cor_botaoS, cor_bordabotaoS;
	int cor_texto, cor_textoS, tamanho;
	quadrilatero_relevo grafico;
	botao_opcao(char TEXTO[]="BOTAO", int X1=0, int Y1=0, int ALTURA=20, int LARGURA=50, luz DIRECCAO_LUZ=BAIXO_ESQUERDA, int ESPESSURA=2 , int COR_BOTAO=GREEN, int COR_BORDABOTAO=WHITE, int COR_BOTAOS=BLUE, int  COR_BORDABOTAOS=RED , int COR_TEXTO=WHITE , int COR_TEXTOS=RED, int TAMANHO=1): grafico(X1,Y1, ALTURA, LARGURA, ESPESSURA, DIRECCAO_LUZ)
	{
		texto = new char[strlen(TEXTO)];
		strcpy (texto,TEXTO);
		cor_botao = COR_BOTAO;
		cor_bordabotao = COR_BORDABOTAO;
		cor_botaoS = COR_BOTAOS;
		cor_bordabotaoS = COR_BORDABOTAOS;
		cor_texto = COR_TEXTO;
		cor_textoS = COR_TEXTOS;
	}
	void desenha_me(bool SELECCIONADO=FALSO)
	{
		int CorBotao, CorBorda, CorTexto;
		if (SELECCIONADO)
		{
			CorBotao = cor_botaoS;
			CorBorda = cor_bordabotaoS;
			CorTexto = cor_textoS;
		}
		else
		{
			CorBotao = cor_botao;
			CorBorda = cor_bordabotao;
			CorTexto = cor_texto;
		}
		grafico.desenha_me(CorBorda,CorBotao);
		int Altura = grafico.pt3.y - grafico.pt1.y;
		int Largura = grafico.pt2.x - grafico.pt1.x;
		ponto N;
		N = calcula_ponto_desenho(CENTRO, MEIO, grafico.pt1, Altura, Largura , textheight(texto) , textwidth(texto));
		escreve_texto(N, texto, CorTexto);
	}
};

class caixa_texto
{
	public:
	char * texto;
	char * conteudo;
	int max_caracteres,	tamanho, letra;
	int cor_caixa;
	int cor_bordacaixa, cor_bordacaixaS;
	int cor_legenda, cor_legendaS;
	int cor_texto;
	ponto posicao_escrita;
	bool passwordchar; //verdadeiro para escrever * em vez dos caracteres reais
	val valores; //tipo de caracteres q reconhece
	quadrilatero_relevo grafico;
	caixa_texto(char TEXTO[]="CAIXA", int X1=0, int Y1=0, int ALTURA=20, int LARGURA=50, luz DIRECCAO_LUZ=NADA, int ESPESSURA=0 , int MAX_CARACTERES=0, bool PASS=FALSO, int COR_CAIXA=DARKGRAY, int COR_BORDACAIXA=WHITE, int  COR_BORDACAIXAS=RED , int COR_LEGENDA=WHITE, int COR_LEGENDAS = RED , int COR_TEXTO=WHITE , int TAMANHO=1): grafico(X1,Y1, ALTURA, LARGURA, ESPESSURA, DIRECCAO_LUZ)
	{

		int Altura = grafico.pt3.y - grafico.pt1.y, Largura = grafico.pt2.x - grafico.pt1.x;

		if (!MAX_CARACTERES) //por default, ‚ 0
			MAX_CARACTERES = Largura / textwidth("M") ;
		max_caracteres = MAX_CARACTERES ;

		conteudo = new char [max_caracteres];
		conteudo[0]='\0';
		texto = new char [strlen(TEXTO)];
		strcpy(texto,TEXTO);

			letra = 0;
		valores = LETRAS;

		passwordchar = PASS;
		cor_caixa = COR_CAIXA;
		cor_bordacaixa = COR_BORDACAIXA; cor_bordacaixaS = COR_BORDACAIXAS;
		cor_legenda = COR_LEGENDA; cor_legendaS = COR_LEGENDAS;
		cor_texto = COR_TEXTO;

		posicao_escrita = calcula_ponto_desenho(CENTRO, ESQUERDA,grafico.pt1 , Altura, Largura, textheight("M"), textwidth("M"));
	}
	void apaga_conteudo()
	{
		do
		{
			escreve(BACKSPACE);
		}
		while (conteudo[0]!='\0');
	}
	char * conteudo_util()
	/*devolve um array, terminado em \0 , com o conteudo util, deixando
	de parte os caracteres q sobram devido ao max caracteres das caixas
	de texto*/
	{
		for (int Cntdor=0; *(conteudo+Cntdor)!='\0' ; Cntdor++);
		Cntdor++;
		char * Prov; Prov = new char[Cntdor];
		for (int Cntdor2=0; Cntdor2<Cntdor-1;Cntdor2++ )
			Prov[Cntdor2] = conteudo[Cntdor2];
		Prov[Cntdor-1] = '\0';
		return (Prov);
	}
	void conteudo_default(char CONTEUDO[]) //nao s utiliza com passwords nem para apgar o conteudo
	{
		conteudo = CONTEUDO;
		escreve_texto(posicao_escrita,CONTEUDO);
		letra = strlen(CONTEUDO);
		posicao_escrita.x += textwidth(CONTEUDO);
	}
	void redefine_cores(int NAT, int BOR_NAT, int BOR_SEL)
	{
		cor_caixa = NAT;
		cor_bordacaixa = BOR_NAT; cor_bordacaixaS = BOR_SEL;
	}
	void redefine_propriedades(char * NOVOTEXTO, val VALORES=LETRAS, bool PASS=FALSO)
	{
		texto = NOVOTEXTO;
		passwordchar = PASS;
		valores = VALORES;
		int Altura = grafico.pt3.y - grafico.pt1.y, Largura = grafico.pt2.x - grafico.pt1.x;
		posicao_escrita = calcula_ponto_desenho(CENTRO, ESQUERDA,grafico.pt1 , Altura, Largura, textheight("M"), textwidth("M"));
	}
	void redimensiona(ponto ORIGEM, int ALTURA, int LARGURA)
	{
		grafico.redimensiona(ORIGEM,ALTURA,LARGURA);
		int Altura = grafico.pt3.y - grafico.pt1.y, Largura = grafico.pt2.x - grafico.pt1.x;
		posicao_escrita = calcula_ponto_desenho(CENTRO, ESQUERDA, grafico.pt1 , Altura, Largura, textheight("M"), textwidth("M"));
		max_caracteres = Largura/textwidth("M");
	}
	bool is_val(int CODIGO, val TIPO_VALOR)
	/*devolve verdadeiro se um caracter for do tipo passado no parametro*/
	{
		switch(TIPO_VALOR)
		{
			case (INTEIROS):
				if (CODIGO>=48 && CODIGO<=57)
					return (VERDADEIRO);
				else
					return (FALSO);
			case (LETRAS):
				if ((CODIGO>=65 && CODIGO<=90) || (CODIGO>=97 && CODIGO<=122))
					return (VERDADEIRO);
				else
					return (FALSO);
			case (ALFANUMERICOS):
				if (is_val(CODIGO,INTEIROS) || is_val(CODIGO,LETRAS))
					return (VERDADEIRO);
				else
					return (FALSO);
			case (DECIMAIS):
				if (is_val(CODIGO, INTEIROS))
					return (VERDADEIRO);
				if	(CODIGO==46)
				{
					//tem de ver s j  h  ponto no conteudo; s¢ admite 1 ponto!
					for (int x=0; conteudo[x]!='\0'; x++)
						if (conteudo[x]==46)
							return (FALSO);
					return (VERDADEIRO);
				}
				return (FALSO);
			case (ASCIICHAR):
				if (is_val(CODIGO, ALFANUMERICOS) || (CODIGO==95) || (CODIGO==32) || (CODIGO==124) || (CODIGO==38)) // '_' ;; ' ' ;; '|' ;; '&'
					return (VERDADEIRO);
				else
					return (FALSO);
		}
		return(FALSO);
	}
	bool permite(int CODIGO)
	{
		if (CODIGO==BACKSPACE)
			return (VERDADEIRO); //Backspace serve para apagar
		return (is_val(CODIGO, valores));
	}
	void escreve(int CODIGO)
	{
		if (permite(CODIGO))
			if (CODIGO!=BACKSPACE && letra<max_caracteres)
			{
				char T[2]={CODIGO,'\0'};
				if (passwordchar)
					escreve_texto(posicao_escrita ,"*", cor_texto);
				else
					escreve_texto(posicao_escrita ,T, cor_texto);
				posicao_escrita.x += textwidth(T);
				conteudo[letra] = T[0];
				letra++;
				conteudo[letra]='\0';
			}
		if (CODIGO==BACKSPACE && letra>0)
		{
			char T[2];
			T[0]=219; //‚ isto-> Û <-
			T[1]='\0';
			//retrocede posicao_escrita
			posicao_escrita-=textwidth(T);
			//desenha o ultimo caracter do conteudo com cor_caixa
			escreve_texto(posicao_escrita, T, cor_caixa);
			//retira o ultimo caracter do conteudo
			letra--;
			conteudo[letra]='\0';
		}
	}
	void desenha_me(bool SELECCIONADO=FALSO, bool INTERIOR=FALSO)
	{
		int CorCaixa, CorBorda, CorTexto;
		if (SELECCIONADO)
		{
			CorBorda = cor_bordacaixaS;
			CorTexto = cor_legendaS;
		}
		else
		{
			CorBorda = cor_bordacaixa;
			CorTexto = cor_legenda;
		}
		ponto escrlegenda = grafico.pt1;
		escrlegenda.y -= textheight ("A") +2;
		escreve_texto(escrlegenda,texto, CorTexto);
		grafico.desenha_me(CorBorda,cor_caixa,INTERIOR);
	}
};

class menu
{
	public:
	int opcao_seleccionada;
	char * titulo;
	int nbotoes;
	botao_opcao * listabotoes;
	int ncaixas;
	caixa_texto * listacaixas;
	int ncontrolos;
	input_type  * listacontrolos;

	menu (char TITULO[],int NBOTOES, int NCAIXAS)
	{
		opcao_seleccionada=0;
		if (NBOTOES)
			listabotoes = new botao_opcao[NBOTOES];
		nbotoes = NBOTOES;
		if (NCAIXAS)
			listacaixas = new caixa_texto[NCAIXAS];
		ncaixas = NCAIXAS;
		listacontrolos = new input_type[NBOTOES+NCAIXAS];
		ncontrolos = NBOTOES + NCAIXAS ;
		titulo = TITULO;
	}
	int ncontrolo(input_type TIPO, int OPCAO_SELECCIONADA)
	/*devolve o numero do controlo da lista
	especifica do seu tipo */
	{
		for (int x=0,c=0; x<=OPCAO_SELECCIONADA; x++)
			if (listacontrolos[x]==TIPO	)
				c++;
		return(c-1);
	}
	int onkeydown (tecla CODIGO)
	/*Mudan‡a de foco de controlo -> 2
	Escrita em caixa de texto -> 1
	Escolha de um botao ("clicar") -> 0
	NADA -> -1
	*/
	{
		if (CODIGO==ANTERIOR || CODIGO==SEGUINTE || CODIGO==TAB) //muda foco
		{
			muda_opcao_seleccionada(CODIGO);
			return (2);
		}
		if (CODIGO==ENTER)
		{
			if (listacontrolos[opcao_seleccionada]==BOTAO) //sai menu
				return(0);
			if (listacontrolos[opcao_seleccionada]==CAIXA) //muda foco
			{
				muda_opcao_seleccionada(SEGUINTE);
				return(2);
			}
		}
		//outra tecla qualquer; escrita em caixa de texto
		if (listacontrolos[opcao_seleccionada]==CAIXA)
		{
			listacaixas[ncontrolo(CAIXA,opcao_seleccionada)].escreve(CODIGO);
			return(1);
		}
		return(-1);
	}
	void desenha_opcao_seleccionada(bool FOCO=VERDADEIRO)
	{
		int N = ncontrolo(listacontrolos[opcao_seleccionada],opcao_seleccionada);
		switch( listacontrolos[opcao_seleccionada] )
		{
			case CAIXA:
				listacaixas[N].desenha_me(FOCO);
			break;
			case BOTAO:
				listabotoes[N].desenha_me(FOCO);
			break;
		}
	}
	void muda_opcao_seleccionada(tecla CODIGO)
	{
		desenha_opcao_seleccionada(FALSO);
		if (CODIGO==ANTERIOR)
		{
			if (opcao_seleccionada==0)
				opcao_seleccionada = ncontrolos-1;
			else
				opcao_seleccionada--;
		}
		if (CODIGO==SEGUINTE || CODIGO==TAB)
		{
			if (opcao_seleccionada==ncontrolos-1)
				opcao_seleccionada = 0;
			else
				opcao_seleccionada++;
		}
		desenha_opcao_seleccionada();
	}
	void define_ordem_controlos(int NCONTROLO, input_type TIPO_CONTROLO)
	{
		listacontrolos[NCONTROLO] = TIPO_CONTROLO;
	}
	void redimensiona_controlos(ponto ORIGEM,int ALTURA,int LARGURA,int INTERVALO)
	{
		int x, c=0, b=0;
		for (x=0; x<ncontrolos; x++)
		{
			if (listacontrolos[x]==CAIXA)
			{
				listacaixas[c].redimensiona(ORIGEM,ALTURA,LARGURA);
				c++;
			}
			if (listacontrolos[x]==BOTAO)
			{
				listabotoes[b].grafico.redimensiona(ORIGEM,ALTURA,LARGURA);
				b++;
			}
			ORIGEM.y += ALTURA+INTERVALO;
		}
	}

	void desenha_me()
	{
		int c=0,b=0, x;
		//escreve o titulo
		ponto p(0,0); //origem do fundo

		settextstyle(DEFAULT_FONT,HORIZ_DIR,2);
		ponto xy = calcula_ponto_desenho (TOPO, MEIO, p, getmaxy(), getmaxx(), textheight(titulo),textwidth(titulo));
		escreve_texto(xy,titulo,RED,2);
		settextstyle(DEFAULT_FONT,HORIZ_DIR,1);

		for (x=0;x<ncontrolos;x++)
		{
			if (listacontrolos[x]==CAIXA)
			{	listacaixas[c].desenha_me(FALSO,VERDADEIRO);
				c++;
			}
			if (listacontrolos[x]==BOTAO)
			{	listabotoes[b].desenha_me();
				b++;
			}
		}
		desenha_opcao_seleccionada();
	}
};

class produto
{
	public:
	char * nome;
	float preco;
	int quant;
	produto(char NOME[]="PRODUTO", float PRECO=0.50, int QUANT=200)
	{
		nome = NOME;
		preco = PRECO;
		quant = QUANT;
	}
};

class moeda
{
	public:
	float valor;
	int quant;
	moeda(float VALOR=0.05, int QUANT=200)
	{
		valor = VALOR; quant = QUANT;
	}
};

//CONVENCOES PARA MENUS
#define C_ALTCB 30	//altura dos botoes e das caixas de texto
#define C_LARCB 180	//largura dos botoes e das caixas de texto
#define C_FOLGA 25 //espa‡o entre controlos(botoes/caixas) de texto

produto listaprodutos[7];
moeda listamoedas[6];

void main()
{
	clrscr();
	int gdriver = DETECT, gmode, errorcode;
	initgraph(&gdriver, &gmode, "");

	setbkcolor(BLACK);

	int Npr=7, Nmds=6;

	listaprodutos[0].nome = "caf‚";
	listaprodutos[0].preco = 0.35;
	listaprodutos[0].quant = 20;
	listaprodutos[1].nome = "descafeinado";
	listaprodutos[1].preco = 0.35;
	listaprodutos[1].quant = 20;
	listaprodutos[2].nome = "galao";
	listaprodutos[2].preco = 0.45;
	listaprodutos[2].quant = 20;
	listaprodutos[3].nome = "chocolate quente";
	listaprodutos[3].preco = 0.50;
	listaprodutos[3].quant = 20;
	listaprodutos[4].nome = "cappuccino";
	listaprodutos[4].preco = 0.50;
	listaprodutos[4].quant = 20;
	listaprodutos[5].nome = "chocolate com leite";
	listaprodutos[5].preco = 0.50;
	listaprodutos[5].quant = 20;
	listaprodutos[6].nome = " gua";
	listaprodutos[6].preco = 0.40;
	listaprodutos[6].quant = 20;

	listamoedas[0].valor = 0.05;
	listamoedas[1].valor = 0.1;
	listamoedas[2].valor = 0.2;
	listamoedas[3].valor = 0.5;
	listamoedas[4].valor = 1;
	listamoedas[5].valor = 2;


	int OPCAO1, OPCAO2, OPCAO3;
	do
	{
		cleardevice();
		OPCAO1 = menuINICIAR();
		if (OPCAO1==1) //adm
		{
			do
			{
				cleardevice();
				OPCAO2 = menuLOGINADM();
				// 0 - voltar atrax 1- entra -1 - falha
				if (OPCAO2==1)
				{
					do
					{
						cleardevice();
						OPCAO3 = menuADM();
						//0-sair 1-produtos 2-moedas
						if (OPCAO3==1)
						{
							cleardevice();
							produtosADMNISTRADOR(Npr);
						}
						if (OPCAO3==2)
						{
							cleardevice();
							moedasADMNISTRADOR(Nmds);
						}
					}
					while(OPCAO3);
				}
			}
			while(OPCAO2);
		}
		if (OPCAO1==2)     //utilizador
		{
			/**/
		}
	}
	while(OPCAO1);
	closegraph();
}

int menuINICIAR()
{
	//MENU ENTRADA MAQUINA VENDING
	ponto p1(0,0);//origem do fundo
	//centra controlos do menu no centro do fundo
	ponto xy = calcula_ponto_desenho(CENTRO, MEIO, p1, getmaxy(), getmaxx(), altura_menu(3,C_ALTCB,C_FOLGA),C_LARCB);

	menu menuINI("VENDING MACHINE",3,0);
	menuINI.define_ordem_controlos(0,BOTAO);
		menuINI.listabotoes[0].texto = "Modo Admnistrador";
	menuINI.define_ordem_controlos(1,BOTAO);
		menuINI.listabotoes[1].texto = "Modo Utilizador";
	menuINI.define_ordem_controlos(2,BOTAO);
		menuINI.listabotoes[2].texto = "Sair";
	menuINI.redimensiona_controlos(xy,C_ALTCB,C_LARCB,C_FOLGA);
	menuINI.desenha_me();
	int tecla_menuINI, CONTINUA;
	do
	{
		tecla_menuINI = getch();
		CONTINUA =menuINI.onkeydown(tecla_menuINI);
	}
	while(CONTINUA);

	if (menuINI.opcao_seleccionada==2) //sair
		return(0);
	else
		return(menuINI.opcao_seleccionada+1); // admnitrador(1), utilizador(2)
}

int menuLOGINADM()
{
	//MENU ENTRADA MODO ADM
	ponto p1(0,0);//origem do fundo
	//centra controlos do menu no centro do fundo
	ponto xy = calcula_ponto_desenho(CENTRO, MEIO, p1, getmaxy(), getmaxx(), altura_menu(4,C_ALTCB,C_FOLGA),C_LARCB);

	menu menuLADM("LOGIN ADMNISTRADOR",2,2);
	menuLADM.define_ordem_controlos(0,CAIXA);
		menuLADM.listacaixas[0].redefine_propriedades("Login",ASCIICHAR);
	menuLADM.define_ordem_controlos(1,CAIXA);
		menuLADM.listacaixas[1].redefine_propriedades("Password",ASCIICHAR,VERDADEIRO);
	menuLADM.define_ordem_controlos(2,BOTAO);
		menuLADM.listabotoes[0].texto = "Entrar";
	menuLADM.define_ordem_controlos(3,BOTAO);
		menuLADM.listabotoes[1].texto = "Voltar atr s";
	menuLADM.redimensiona_controlos(xy,C_ALTCB,C_LARCB,C_FOLGA);
	menuLADM.desenha_me();
	menuLADM.listacaixas[0].conteudo_default("root\0");
	int tecla_menuLADM=1, CONTINUA;
	do
	{
		if (!tecla_menuLADM)//sem este if, as teclas das setas retornam valores!
		{
			getch();
		}
		tecla_menuLADM=getch();
		CONTINUA = menuLADM.onkeydown(tecla_menuLADM);
	}
	while(CONTINUA);

	if (menuLADM.opcao_seleccionada==3) //desistir
		return (0);
	if (menuLADM.opcao_seleccionada==2) //entrar
	{
		if (!strcmp(menuLADM.listacaixas[0].conteudo,"root") && !strcmp(menuLADM.listacaixas[1].conteudo,"toor"))
			return (1);
		else
			return (-1);
	}
}

int menuADM()
{
	//MENU ADMNISTRADOR
	ponto p1(0,0);//origem do fundo
	//centra controlos do menu no centro do fundo
	ponto xy = calcula_ponto_desenho(CENTRO, MEIO, p1, getmaxy(), getmaxx(), altura_menu(3,C_ALTCB,C_FOLGA),C_LARCB);

	menu menuADM("ADMNISTRADOR",3,0);
	menuADM.define_ordem_controlos(0,BOTAO);
		menuADM.listabotoes[0].texto = "Produtos";
	menuADM.define_ordem_controlos(1,BOTAO);
		menuADM.listabotoes[1].texto = "Moedas";
	menuADM.define_ordem_controlos(2,BOTAO);
		menuADM.listabotoes[2].texto = "Sair de Sessao";
	menuADM.redimensiona_controlos(xy,C_ALTCB,C_LARCB,C_FOLGA);
	menuADM.desenha_me();
	int tecla_menuADM, CONTINUA;
	do
	{
		tecla_menuADM=getch();
		CONTINUA =menuADM.onkeydown(tecla_menuADM);
	}
	while(CONTINUA);

	if (menuADM.opcao_seleccionada==2) //sair
		return(0);
	else
		return(menuADM.opcao_seleccionada+1); // produtos(1), moedas(2)
}

void produtosADMNISTRADOR(int Nprodutos)
{
	//ADM - MANUNTENCAO PRODUTOS
	float * TempPrecos; //array temporario para os precos
	TempPrecos = new float[Nprodutos];
	int * TempQuant; //array temporario para as quantidades
	TempQuant = new int [Nprodutos];

	//cria e desenha a grelha
	int Ncolunas=3, Cntdor, Cntdor2;
	ponto origemgrelha(0,0);
	grelha_nreg tabela( origemgrelha, 28, 150, 12, Ncolunas);
	tabela.redimensiona_coluna(0,200);
	tabela.redimensiona_coluna(1,93);
	tabela.redimensiona_coluna(2,93);
	ponto grelhacentrada = calcula_ponto_desenho(CENTRO, MEIO, origemgrelha, getmaxy(),getmaxx(),tabela.altura(),tabela.largura());
	tabela.redimensiona_origem(grelhacentrada);
	tabela.desenha_me(WHITE,0,Nprodutos);
	//escreve_titulos
	ponto escreveaqui;
	escreveaqui = calcula_ponto_desenho(CENTRO,ESQUERDA, tabela.malha[0].pt1, tabela.malha[0].pt3.y-tabela.malha[0].pt1.y, tabela.malha[0].pt2.x-tabela.malha[0].pt1.x,textheight("Produto"),textwidth("Produto")  );
	escreve_texto(escreveaqui, "Produto");
	escreveaqui = calcula_ponto_desenho(CENTRO,ESQUERDA, tabela.malha[1].pt1, tabela.malha[1].pt3.y-tabela.malha[1].pt1.y, tabela.malha[1].pt2.x-tabela.malha[1].pt1.x,textheight("Pre‡os"),textwidth("Pre‡os")  );
	escreve_texto(escreveaqui, "Pre‡os");
	escreveaqui= calcula_ponto_desenho(CENTRO,ESQUERDA, tabela.malha[2].pt1, tabela.malha[2].pt3.y-tabela.malha[2].pt1.y, tabela.malha[2].pt2.x-tabela.malha[2].pt1.x,textheight("Quantidades"),textwidth("Quantidades"));
	escreve_texto(escreveaqui, "Quantidades");

	//escreve nome dos produtos (est tico)
	for ( Cntdor=0; Cntdor< Nprodutos;Cntdor++)
	{
		escreveaqui = calcula_ponto_desenho(CENTRO,ESQUERDA,tabela.malha[Ncolunas*(Cntdor+1)].pt1, tabela.malha[Ncolunas*(Cntdor+1)].pt3.y-tabela.malha[Ncolunas*(Cntdor+1)].pt1.y, tabela.malha[Ncolunas*(Cntdor+1)].pt2.x-tabela.malha[Ncolunas*(Cntdor+1)].pt1.x,textheight(listaprodutos[Cntdor].nome),textwidth(listaprodutos[Cntdor].nome));
		escreve_texto(escreveaqui,listaprodutos[Cntdor].nome);
	}
	//menu com 2 botoes e n caixas de texto = nprodutos * 2(coluna precos e quant.)
	menu admprodutos("PRODUTOS",2,Nprodutos*2);

	for (Cntdor=0; Cntdor<Nprodutos*2; Cntdor++)
		admprodutos.define_ordem_controlos(Cntdor,CAIXA);
	admprodutos.define_ordem_controlos(Nprodutos*2,BOTAO);
	admprodutos.define_ordem_controlos(Nprodutos*2+1,BOTAO);

	//para cada caixa d  o conteudo e escreve-o
	//cada caixa de texto encaixa numa c‚lula da grelha
	//coluna precos
	for (Cntdor=4, Cntdor2=0; Cntdor2<Nprodutos; Cntdor+=Ncolunas, Cntdor2++)
	{
		admprodutos.listacaixas[Cntdor2*2].grafico = tabela.malha[Cntdor];
		admprodutos.listacaixas[Cntdor2*2].redefine_propriedades("",DECIMAIS);
	}
	//coluna quant.
	for (Cntdor=5, Cntdor2=0;Cntdor2<Nprodutos ; Cntdor+=Ncolunas, Cntdor2++)
	{
		admprodutos.listacaixas[Cntdor2*2+1].grafico = tabela.malha[Cntdor];
		admprodutos.listacaixas[Cntdor2*2+1].redefine_propriedades("",INTEIROS);
		admprodutos.listacaixas[Cntdor2*2+1].max_caracteres = 3;
	}
	admprodutos.listabotoes[0].grafico.redimensiona( tabela.malha[33].pt1, C_ALTCB,C_LARCB);
	admprodutos.listabotoes[0].texto = "Voltar e actualizar";
	admprodutos.listabotoes[1].grafico.redimensiona( tabela.malha[34].pt1, C_ALTCB,C_LARCB);
	admprodutos.listabotoes[1].texto = "Cancelar";
	admprodutos.desenha_me();
	//coloca os precos no ecra e numa tabela provisoria
	for ( Cntdor=0 ; Cntdor<Nprodutos*2 ;Cntdor+=2 )
	{
		TempPrecos[Cntdor/2] = listaprodutos[Cntdor/2].preco;
		admprodutos.listacaixas[Cntdor].conteudo_default (strval(listaprodutos[Cntdor/2].preco));
		TempQuant[Cntdor/2] = listaprodutos[Cntdor/2].quant;
		admprodutos.listacaixas[Cntdor+1].conteudo_default (strval(listaprodutos[Cntdor/2].quant));
	}

	int tecla_admprodutos,CONTINUA;
	do
	{
		tecla_admprodutos=getch();
		CONTINUA =admprodutos.onkeydown(tecla_admprodutos);
		//alterar precos e quantidades
		if (CONTINUA==1) //ESCRITA EM CAIXA DE TEXTO
		{
			//admprodutos.ncontrolo(CAIXA,admprodutos.opcao_seleccionada) devolve o indice da caixa de texto q esta a ser manipulada
			// caixa de texto indice par - quant, impar - precos; Nprod = caixa texto indice / 2
			if (admprodutos.ncontrolo(CAIXA,admprodutos.opcao_seleccionada) % 2==0) //PRECOS
			{
				TempPrecos[admprodutos.ncontrolo(CAIXA,admprodutos.opcao_seleccionada)/2] = valstr(admprodutos.listacaixas[admprodutos.ncontrolo(CAIXA,admprodutos.opcao_seleccionada)].conteudo_util());
			}
			else //QUANTIDADES
			{
				//for‡a o utilizador a escrever 200 - a bem ou a mal...
				if (valstr(admprodutos.listacaixas[admprodutos.ncontrolo(CAIXA,admprodutos.opcao_seleccionada)].conteudo_util())>200)
				{
					admprodutos.listacaixas[admprodutos.ncontrolo(CAIXA,admprodutos.opcao_seleccionada)].apaga_conteudo();
					admprodutos.listacaixas[admprodutos.ncontrolo(CAIXA,admprodutos.opcao_seleccionada)].escreve('2');
					admprodutos.listacaixas[admprodutos.ncontrolo(CAIXA,admprodutos.opcao_seleccionada)].escreve('0');
					admprodutos.listacaixas[admprodutos.ncontrolo(CAIXA,admprodutos.opcao_seleccionada)].escreve('0');
				}
				TempQuant[admprodutos.ncontrolo(CAIXA,admprodutos.opcao_seleccionada)/2] = valstr(admprodutos.listacaixas[admprodutos.ncontrolo(CAIXA,admprodutos.opcao_seleccionada)].conteudo_util());
			}
		}
	}
	while(CONTINUA);

	if (!admprodutos.ncontrolo(BOTAO,admprodutos.opcao_seleccionada)) //botao Voltar e actualizar tem valor 0
	{
		//ACTUALIZA PRECOS E QUANTIDADES
		for (Cntdor=0; Cntdor<Nprodutos; Cntdor++) //precos
			listaprodutos[Cntdor].preco = TempPrecos[Cntdor];
		for (Cntdor=0; Cntdor<Nprodutos; Cntdor++) //quantidades
			listaprodutos[Cntdor].quant = TempQuant[Cntdor];
	}
}

void moedasADMNISTRADOR(int Nmoedas)
{
	//ADM - MANUNTENCAO MOEDAS
	int * TempQuant; //array temporario para as quantidades
	TempQuant = new int [Nmoedas];

	//cria e desenha a grelha
	int Ncolunas=2, Cntdor, Cntdor2;
	ponto origemgrelha(0,0);
	grelha_nreg tabela( origemgrelha, 28, 150, 12, Ncolunas);
	tabela.redimensiona_coluna(0,200);
	tabela.redimensiona_coluna(1,93);
	ponto grelhacentrada = calcula_ponto_desenho(CENTRO, MEIO, origemgrelha, getmaxy(),getmaxx(),tabela.altura(),tabela.largura());
	tabela.redimensiona_origem(grelhacentrada);
	tabela.desenha_me(WHITE,0,Nmoedas);
	//escreve_titulos
	ponto escreveaqui;
	escreveaqui = calcula_ponto_desenho(CENTRO,ESQUERDA, tabela.malha[0].pt1, tabela.malha[0].pt3.y-tabela.malha[0].pt1.y, tabela.malha[0].pt2.x-tabela.malha[0].pt1.x,textheight("Moeda"),textwidth("Moeda")  );
	escreve_texto(escreveaqui, "Moeda");
	escreveaqui = calcula_ponto_desenho(CENTRO,ESQUERDA, tabela.malha[1].pt1, tabela.malha[1].pt3.y-tabela.malha[1].pt1.y, tabela.malha[1].pt2.x-tabela.malha[1].pt1.x,textheight("Quantidades"),textwidth("Quantidades")  );
	escreve_texto(escreveaqui, "Quantidades");


	//escreve valores das moedas (est tico)
	for ( Cntdor=0; Cntdor< Nmoedas; Cntdor++)
	{
		escreveaqui = calcula_ponto_desenho(CENTRO,ESQUERDA,tabela.malha[Ncolunas*(Cntdor+1)].pt1, tabela.malha[Ncolunas*(Cntdor+1)].pt3.y-tabela.malha[Ncolunas*(Cntdor+1)].pt1.y, tabela.malha[Ncolunas*(Cntdor+1)].pt2.x-tabela.malha[Ncolunas*(Cntdor+1)].pt1.x,textheight(strval(listamoedas[Cntdor].valor)),textwidth(strval(listamoedas[Cntdor].valor)));
		escreve_texto(escreveaqui,strval(listamoedas[Cntdor].valor));
	}
	//menu com 2 botoes e n caixas de texto = nmoedas (coluna quantidades)
	menu admmoedas("MOEDAS",2,Nmoedas);

	for (Cntdor=0; Cntdor<Nmoedas; Cntdor++)
		admmoedas.define_ordem_controlos(Cntdor,CAIXA);
	admmoedas.define_ordem_controlos(Nmoedas,BOTAO);
	admmoedas.define_ordem_controlos(Nmoedas+1,BOTAO);

	//coluna quantidades
	for (Cntdor=3, Cntdor2=0; Cntdor2<Nmoedas; Cntdor+=Ncolunas, Cntdor2++)
	{
		admmoedas.listacaixas[Cntdor2].grafico = tabela.malha[Cntdor];
		admmoedas.listacaixas[Cntdor2].redefine_propriedades("",INTEIROS);
	}

	admmoedas.listabotoes[0].grafico.redimensiona( tabela.malha[22].pt1, C_ALTCB,C_LARCB);
	admmoedas.listabotoes[0].texto = "Voltar e actualizar";
	admmoedas.listabotoes[1].grafico.redimensiona( tabela.malha[23].pt1, C_ALTCB,C_LARCB);
	admmoedas.listabotoes[1].texto = "Cancelar";
	admmoedas.desenha_me();

	//coloca as quantidades no ecra e numa tabela provisoria
	for ( Cntdor=0 ; Cntdor<Nmoedas ;Cntdor++ )
	{
		TempQuant[Cntdor] = listamoedas[Cntdor].quant;
		admmoedas.listacaixas[Cntdor].conteudo_default (strval(listamoedas[Cntdor].quant));
	}

	int tecla_admmoedas,CONTINUA;
	do
	{
		tecla_admmoedas=getch();
		CONTINUA =admmoedas.onkeydown(tecla_admmoedas);
		//alterar precos e quantidades
		if (CONTINUA==1) //ESCRITA EM CAIXA DE TEXTO; s¢ h  coluna QUANTIDADES
		{
			//for‡a o utilizador a escrever 200 - a bem ou a mal...
			if (valstr(admmoedas.listacaixas[admmoedas.ncontrolo(CAIXA,admmoedas.opcao_seleccionada)].conteudo_util())>200)
			{
				admmoedas.listacaixas[admmoedas.ncontrolo(CAIXA,admmoedas.opcao_seleccionada)].apaga_conteudo();
				admmoedas.listacaixas[admmoedas.ncontrolo(CAIXA,admmoedas.opcao_seleccionada)].escreve('2');
				admmoedas.listacaixas[admmoedas.ncontrolo(CAIXA,admmoedas.opcao_seleccionada)].escreve('0');
				admmoedas.listacaixas[admmoedas.ncontrolo(CAIXA,admmoedas.opcao_seleccionada)].escreve('0');
			}
			TempQuant[admmoedas.ncontrolo(CAIXA,admmoedas.opcao_seleccionada)] = valstr(admmoedas.listacaixas[admmoedas.ncontrolo(CAIXA,admmoedas.opcao_seleccionada)].conteudo_util());
		}
	}
	while(CONTINUA);

	if (!admmoedas.ncontrolo(BOTAO,admmoedas.opcao_seleccionada)) //botao Voltar e actualizar tem valor 0
	{
		//ACTUALIZA QUANTIDADES
		for (Cntdor=0; Cntdor<Nmoedas; Cntdor++) //quantidades
			listamoedas[Cntdor].quant = TempQuant[Cntdor];
	}
}

inline int altura_menu(int NCONTROLOS, int ALTURAC, int INTERVALO)
/*devolve a altura esperada dos controlos de um menu com os botoes dispostos
verticalmente */
{
	return (ALTURAC * NCONTROLOS + (NCONTROLOS-1) * INTERVALO);
}

float valstr (char NUMERO[])
/* o array que entrar nesta funcao tem q ter mais um indice q o
 comprimento efectivo. Assim, o numero 1234 estar  num array com 5 indices,
 onde o ultimo indice ‚ = '\0'  <=>  [NULO]*/
{
	int Comprimento, PosicaoPonto , CasasDecimais, Contador;
	float Numero = 0;
	Comprimento = strlen(NUMERO);
	//procurar ponto da divisao decimal
	for (PosicaoPonto=0; (NUMERO[PosicaoPonto]!='.' && PosicaoPonto<Comprimento ); PosicaoPonto++);
	//calcular numero de casas decimais:
	//… prova da macacada de escrever um ponto sem algarismos depois ( ex: '1903.' )
	//tb … prova da macacada de escrever um ponto sem algarismos antes ( ex: '.2333' )
	CasasDecimais = (Comprimento > (PosicaoPonto+1))? Comprimento - (PosicaoPonto+1):0;

	CasasDecimais *= (-1);

	for (Contador=Comprimento-1; Contador>=0;Contador--)
	{
		if (NUMERO[Contador]!='.')
		{
			Numero += (NUMERO[Contador]-48) * pow(10,CasasDecimais) ;
			CasasDecimais++;
		}
	}
	return(Numero);
}

void escreve_texto(ponto ORIGEM, char * TEXTO, int COR, int TAMANHO)
{
	if (getcolor()!=COR)
		setcolor(COR);
	settextstyle(DEFAULT_FONT,HORIZ_DIR,TAMANHO);
	outtextxy(ORIGEM.x,ORIGEM.y,TEXTO);
}


ponto calcula_ponto_desenho(valin V_AL, halin H_AL, ponto ORIGEM, int ALT_CXPR, int LAR_CXPR, int ALT_CX, int LAR_CX)
{
	int Epc = 5; //numero de pixeis de folga para a borda da Caixa Principal onde a Caixa ‚ posicionada
				//na pratica, ‚ o cellpadding
	int NovoX=0, NovoY=0; //incrementos relativamente … origem
	switch (V_AL)
	{
		case TOPO:
			NovoY = Epc;
		break;
		case BASE:
			NovoY = ORIGEM.y + ALT_CXPR - ALT_CX - Epc;
		break;
		case CENTRO:
			NovoY =	ALT_CXPR / 2 - ALT_CX / 2;
		break;
	}
	switch (H_AL)
	{
		case ESQUERDA:
			NovoX = Epc;
		break;
		case MEIO:
			NovoX =	LAR_CXPR / 2 - LAR_CX / 2;
		break;
	}
	ORIGEM.x+=NovoX; ORIGEM.y += NovoY;
	return (ORIGEM);

};

//CONVERTER UM NUMERO NUMA STRING

char * strval (float NUMERO)
/*converte um numero numa string. O numero devolvido ter  max. 2 casas decimais*/
{
	int pint, pdec, Ncd=2, Nint, Cntdor, dim; //o numero de casas decimais est  defenido para 2
	char * array;
	pint = modulo(NUMERO);
	pdec = partedecimal(NUMERO);
	Nint = nalgarismos(pint);
	if (pdec)
		dim = Ncd+Nint+2;
	else
		dim = Nint+1;
	array = new char[dim];
	dim--;
	array[dim]='\0';
	dim--;
	if (pdec)
	{
		//casas decimais
		for (Cntdor = 0;Cntdor<2 ; Cntdor++)
		{
			array[dim]= (pdec%10)+48;
			pdec/=10;
			dim--;
		}
		//ponto
		array[dim]='.';
		dim--;
	}
	for (Cntdor=0; Cntdor < Nint; Cntdor++)
	{
		array[dim]=(pint % 10)+48;
		pint/=10;
		dim--;
	}
	return (array);
}

int modulo(float NUMERO)
{
	int N = NUMERO;
	return (N);
}

float partedecimal(float NUMERO)
{
	return (arredonda( (NUMERO-modulo(NUMERO))*100 ));
}

int arredonda ( double NUMERO)
/* corta parte decimal
 casa das decimais igual ou superior a 5: arredonda para cima;
 senao, fica o valor absoluto do numero
 esta funcao ‚ utilizada no arredondamento da parte decimal uma vez q h  uma descrepancia
	ex: numero = 1.01
	modulo = 1
	partedecimal(S/ARRED) = 0.99999999999999999 -> era suposto devolver 1.
*/
{
	if (NUMERO<=0)
		return (0);
	int AbsN = NUMERO; double Dif = NUMERO - AbsN;
	return  (Dif>=0.5)?NUMERO+1:NUMERO;  ;
}

int nalgarismos(int NUMERO)
{
	int Cntador=0;
	do
	{
		NUMERO/=10;
		Cntador++;
	} while (NUMERO>0);
	return(Cntador);
}
