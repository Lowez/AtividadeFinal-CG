#include "Scene7.h"


CScene7::CScene7()
{

	pCamera = NULL;
	pTexto = NULL;
	pTextures = NULL;
	pModel3DS_1 = NULL;
	pModel3DS_2 = NULL;
	pModel3DS_3 = NULL;
	pShaderMgr = NULL;

	bIsWireframe = false;
	bIsCameraFPS = true;

	iFPS = 0;
	iFrames = 0;
	ulLastFPS = 0;
	szTitle[256] = 0;

	// Cria gerenciador de impressão de texto na tela
	pTexto = new CTexto();

	// Cria camera
	pCamera = new CCamera(-39.23f, 119.02f, -16.25f, 1.0f);

	// Cria o Timer
	pTimer = new CTimer();
	pTimer->Init();

	fTimerPosY = 0.0f;
	fRenderPosY = 0.0f;

	
	// Carrega todas as texturas
	pTextures = new CTexture();
	pTextures->CreateTextureAnisotropic(0, "../assets/skybox2-right2.jpg");
	pTextures->CreateTextureAnisotropic(1, "../assets/skybox2-front.jpg");
	pTextures->CreateTextureAnisotropic(2, "../assets/skybox2-bottom.jpg");
	pTextures->CreateTextureAnisotropic(3, "../assets/skybox2-top.jpg");
	pTextures->CreateTextureAnisotropic(4, "../assets/skybox2-right.jpg");
	pTextures->CreateTextureAnisotropic(5, "../assets/skybox2-left.jpg");

	pTextures->CreateTextureAnisotropic(6, "../Scene7/stair.jpg");
	pTextures->CreateTextureMipMap(7, "../Scene1/grass.bmp");

	pTextures->CreateTextureTGA(9, "../Scene1/tree.tga");
	pTextures->CreateTextureTGA(10, "../Scene1/tree2.tga");
	pTextures->CreateTextureTGA(11, "../Scene1/grass.png");

	pTextures->CreateTextureAnisotropic(12, "../Scene7/stair.jpg");
	pTextures->CreateTextureAnisotropic(13, "../Scene7/summonj.jpg");


	// Carrega Objetos da  Cena (casa)
	pModel3DS_1 = new CModel_3DS();
	pModel3DS_1->Load("../assets/dark-souls-bonfire/textures/mapa.3ds");

	// Carrega Objetos da  Cena (muro)
	pModel3DS_2 = new CModel_3DS();
	pModel3DS_2->Load("../Scene1/Castle/Castle.3DS");

	// Carrega Objetos da  Cena (gramado)
	pModel3DS_3 = new CModel_3DS();
	pModel3DS_3->Load("../Scene1/Plane001.3DS");
	

	fPosX = 0.0f;
	fPosY = 10.0f;
	fPosZ = 0.0f;
	fMovementFactor = 0.1f;

	// Cria esfera com coordenadas de textura
	this->sphere1 = gluNewQuadric();
	gluQuadricTexture(this->sphere1, TRUE);

	// Carrega os Shader Programs
	pShaderMgr = new CShaderManager();
	pShaderMgr->InitShaderManager("../Scene2/SpotLightShader.vert", "../Scene2/SpotLightShader.frag");
	bActiveShaderProgram = false;

	LightAmbient[0] = 0.0f;		LightAmbient[1] = 0.0f;		LightAmbient[2] = 0.0f;	LightAmbient[3] = 1.0f;
	LightDiffuse[0] = 1.0f;		LightDiffuse[1] = 1.0f;		LightDiffuse[2] = 1.0f;		LightDiffuse[3] = 1.0f;
	LightSpecular[0] = 1.0f;	LightSpecular[1] = 1.0f;	LightSpecular[2] = 1.0f;	LightSpecular[3] = 1.0f;
	LightPosition[0] = 0.0f;	LightPosition[1] = 20.0f;	LightPosition[2] = 0.0f;	LightPosition[3] = 1.0f;
	LightDirection[0] = 0.0f;	LightDirection[1] = -1.0f;	LightDirection[2] = 0.0f;

	MatAmbient[0] = 0.3f;	MatAmbient[1] = 0.3f;	MatAmbient[2] = 0.3f;	MatAmbient[3] = 1.0f;
	MatDiffuse[0] = 0.5f;	MatDiffuse[1] = 0.5f;	MatDiffuse[2] = 0.5f;	MatDiffuse[3] = 1.0f;
	MatSpecular[0] = 0.6f;	MatSpecular[1] = 0.6f;	MatSpecular[2] = 0.6f;	MatSpecular[3] = 1.0f;
	MatShininess = 64.0f;



	borda = 10.0f;
	cutoff = 45.0f;
	// Armazena posição da variável "interna"
	uinterna = glGetUniformLocation(pShaderMgr->id(0), "interna");
	// Inicializa o valor de cosborda
	cosborda = cos((cutoff - borda) * GL_PI / 180.0f);

	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	// Criando a SpotLight
	glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, LightSpecular);
	glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, cutoff);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, borda);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, LightDirection);


	// Aplicando Material 1 aos objetos
	/*glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MatAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MatDiffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, MatSpecular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, MatShininess);*/

	enabledFog = false;
	enabledSpotlight = false;
}


CScene7::~CScene7(void)
{
	if (pTexto)
	{
		delete pTexto;
		pTexto = NULL;
	}

	if (pTextures)
	{
		delete pTextures;
		pTextures = NULL;
	}

	if (pCamera)
	{
		delete pCamera;
		pCamera = NULL;
	}

	if (pTimer)
	{
		delete pTimer;
		pTimer = NULL;
	}

	if (pShaderMgr)
	{
		delete pShaderMgr;
		pShaderMgr = NULL;
	}

	gluDeleteQuadric(this->sphere1);

	if (pModel3DS_1)
	{
		delete pModel3DS_1;
		pModel3DS_1 = NULL;
	}

	if (pModel3DS_2)
	{
		delete pModel3DS_2;
		pModel3DS_2 = NULL;
	}

	if (pModel3DS_3)
	{
		delete pModel3DS_3;
		pModel3DS_3 = NULL;
	}
}




int CScene7::DrawGLScene(void)	// Função que desenha a cena
{
	// Get FPS
	if (GetTickCount() - ulLastFPS >= 1000)	// When A Second Has Passed...
	{
		ulLastFPS = GetTickCount();				// Update Our Time Variable
		iFPS = iFrames;							// Save The FPS
		iFrames = 0;							// Reset The FPS Counter
	}
	iFrames++;									// FPS counter
	
	pTimer->Update();							// Atualiza o timer

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Limpa a tela e o Depth Buffer
	glLoadIdentity();									// Inicializa a Modelview Matrix Atual


	// Seta as posições da câmera
	pCamera->setView();


	// Desenha grid 
	//Draw3DSGrid(20.0f, 20.0f);

	// Modo FILL ou WIREFRAME (pressione barra de espaço)	
	if (bIsWireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);	

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//                               DESENHA OS OBJETOS DA CENA (INÍCIO)
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	glColor3f(1.0f, 1.0f, 1.0f);

	glEnable(GL_TEXTURE_2D);
	// Habilita Blending
	glEnable(GL_BLEND);
	// Configura função de Blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	/*
		SpotLight
	*/
	if (enabledSpotlight) {
		glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, LightDirection);
		glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);

		//// Habilita iluminação
		/*glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);*/


		//// Ativa ou não o Shader Program (TRUE = Spotlight per Pixel | FALSE = Spotlight per Vertex)
		if (bActiveShaderProgram == false)
		{
			pShaderMgr->bind(0);
			glUniform1f(uinterna, cosborda); 	// E envia o valor atual da borda para o shader
		}
	}

	/*
		Skybox
	*/
	CreateSkyBox(0.0f, 100.0f, 0.0f,
	1000.0f, 1000.0f, 1000.0f,
	pTextures);

	/*
		Neblina
	*/
	if (enabledFog) {

		glEnable(GL_FOG);

		// Define o tipo de névoa
		glFogi(GL_FOG_MODE, GL_LINEAR); // GL_EXP, GL_EXP2 também são opções

		// Define a cor da névoa (RGBA)
		GLfloat fogColor[4] = { 0.5f, 0.5f, 0.5f, 0.2f }; // Cor cinza
		glFogfv(GL_FOG_COLOR, fogColor);

		// Define a distância inicial e final da névoa
		glFogf(GL_FOG_START, 13.0f); // Distância inicial da névoa
		glFogf(GL_FOG_END, 70.0f);   // Distância final da névoa
		glFogi(GL_FOG_MODE, GL_LINEAR);
		glHint(GL_FOG_HINT, GL_NICEST);

		// Define a densidade da névoa (opcional)
		glFogf(GL_FOG_DENSITY, 0.1f);
	}

	/*
		3DS Mapa
	*/
	glPushMatrix();
	glTranslatef(-25.0f, 100.0f, 20.0f);
	glRotatef(90, 0.0f, 1.0f, 0.0f);

	glNormal3f(0.0f, 0.0f, 1.0f);
	pModel3DS_1->Draw();

	/*
		Modelagem glVertex
	*/

	// Escadas da Árvore
	DrawStairs(-50.0f, 3.0f, 20.0f, 0.0f, 1.0f, 0.0f, 90.0f, 20.0f, 0.5f, 1.0f);

	// Escadas do Poço
	DrawStairs(-14.0f, 5.0f, -35.0f, 0.0f, 1.0f, 0.0f, 12.0f, 10.0f, 1.0f, 2.0f);

	// Escada Multi Textura
	MultiTextureStair(-15.0f, 3.0f, 10.0f, 0.0f, 1.0f, 0.0f, 90.0f, 6);

	/*
		Blending: Gramas
	*/

	// Desenha Gramas
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.5);
	pTextures->ApplyTexture(11);

	DrawGrasses();

	glDisable(GL_ALPHA_TEST);

	glPopMatrix();

	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);

	// Desabilita o processamento de shaders dos objetos
	if (bActiveShaderProgram == true)
	{
		pShaderMgr->unbind();
	}


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//                               DESENHA OS OBJETOS DA CENA (FIM)
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	fTimerPosY = pTimer->GetTime() / 1000.0f;
	fRenderPosY += 0.2f;

	// Impressão de texto na tela...
	// Muda para modo de projeção ortogonal 2D
	// OBS: Desabilite Texturas e Iluminação antes de entrar nesse modo de projeção
	OrthoMode(0, 0, WIDTH, HEIGHT);


	glPushMatrix();
	glTranslatef(0.0f, HEIGHT - 100, 0.0f);	// Move 1 unidade para dentro da tela (eixo Z)

	// Cor da fonte
	glColor3f(1.0f, 1.0f, 0.0f);


	glRasterPos2f(10.0f, 0.0f);	// Posicionando o texto na tela
	if (!bIsWireframe) {
		pTexto->glPrint("[TAB]  Modo LINE"); // Imprime texto na tela
	}
	else {
		pTexto->glPrint("[TAB]  Modo FILL");
	}


	//// Camera LookAt
	glRasterPos2f(10.0f, 40.0f);
	pTexto->glPrint("Player LookAt  : %f, %f, %f", pCamera->Forward[0], pCamera->Forward[1], pCamera->Forward[2]);

	//// Posição do Player
	glRasterPos2f(10.0f, 60.0f);
	pTexto->glPrint("Player Position: %f, %f, %f", pCamera->Position[0], pCamera->Position[1], pCamera->Position[2]);

	//// Imprime o FPS da aplicação e o Timer
	glRasterPos2f(10.0f, 80.0f);
	pTexto->glPrint("Frames-per-Second: %d ---- Timer: %.1f segundos", iFPS, (pTimer->GetTime()/1000));


	glPopMatrix();

	// Muda para modo de projeção perspectiva 3D
	PerspectiveMode();

	return true;
}

void CScene7::DrawStairs(float pX, float pY, float pZ,
	float rX, float rY, float rZ, float angle, float sX, float sY, float sZ)
{
	glPushMatrix();
	glTranslatef(pX, pY, pZ);
	glRotatef(angle, rX, rY, rZ);
	
	DrawCube(sX, sY, sZ, 6);

	glPushMatrix();
	glTranslatef(0.0f, sY, -sZ);

	DrawCube(sX - 2.0f, sY, sZ, 6);

	glPushMatrix();
	glTranslatef(0.0f, sY, -sZ);

	DrawCube(sX - 3.0f, sY, sZ, 6);

	glPushMatrix();
	glTranslatef(0.0f, sY, -sZ);

	DrawCube(sX - 4.0f, sY, sZ, 6);


	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
}

void CScene7::DrawGrasses()
{
	vector<vector<float>> grassPos = {
		{-22.0f, 4.5f, -37.0f},
		{5.0f, 5.0f, -18.0f},
		{7.0f, 5.0f, -18.0f},
		{4.0f, 5.0f, -15.0f},
		{7.0f, 5.0f, -8.0f},
		{4.0f, 5.0f, -15.0f},

		{-50.0f, 4.5f, 25.0f},
		{-45.0f, 3.0f, 26.0f},
		{-47.0f, 4.5f, 28.0f},
	};

	for (const auto& coords : grassPos) {
		if (coords.size() < 3) {
			// Certifique-se de que o vetor de coordenadas tem pelo menos 3 valores (x, y, z)
			continue;
		}

		float x = coords[0];
		float y = coords[1];
		float z = coords[2];

		glPushMatrix();
		glTranslatef(x, y, z);
		glScalef(0.3f, 0.3f, 0.3f);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-10.0f, 0.0f, 0.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(10.0f, 0.0f, 0.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(10.0f, 15.0f, 0.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-10.0f, 15.0f, 0.0f);

		glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 10.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0f, 0.0f, -10.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0f, 15.0f, -10.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, 15.0f, 10.0f);
		glEnd();
		glPopMatrix();
	}
}

void CScene7::DrawCube(float sX, float sY, float sZ,
	int texID)
{

	// Seta a textura ativa
	if (texID >= 0) {
		pTextures->ApplyTexture(texID);

		// Configura a repetição da textura
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	glPushMatrix();
	//glTranslatef(pX, pY, pZ);
	//glRotatef(angle, rX, rY, rZ);
	glScalef(sX, sY, sZ);

	glBegin(GL_QUADS);

	glNormal3f(0.0f, 0.0f, 1.0f);
	// face frente
	// face frente
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, 0.5f);
	glTexCoord2f(2.0f, 0.0f); glVertex3f(1.5f, -0.5f, 0.5f);
	glTexCoord2f(2.0f, 2.0f); glVertex3f(1.5f, 0.5f, 0.5f);
	glTexCoord2f(0.0f, 2.0f); glVertex3f(-0.5f, 0.5f, 0.5f);

	// face trás
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.5f, -0.5f, -0.5f);
	glTexCoord2f(0.0f, 2.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
	glTexCoord2f(2.0f, 2.0f); glVertex3f(-0.5f, 0.5f, -0.5f);
	glTexCoord2f(2.0f, 0.0f); glVertex3f(1.5f, 0.5f, -0.5f);

	// face direita
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.5f, -0.5f, 0.5f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.5f, -0.5f, -0.5f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.5f, 0.5f, -0.5f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.5f, 0.5f, 0.5f);

	// face esquerda
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f, -0.5f, 0.5f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f, 0.5f, 0.5f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, 0.5f, -0.5f);

	// face baixo
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
	glTexCoord2f(0.0f, 2.0f); glVertex3f(1.5f, -0.5f, -0.5f);
	glTexCoord2f(2.0f, 2.0f); glVertex3f(1.5f, -0.5f, 0.5f);
	glTexCoord2f(2.0f, 0.0f); glVertex3f(-0.5f, -0.5f, 0.5f);

	// face cima
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, 0.5f, 0.5f);
	glTexCoord2f(0.0f, 2.0f); glVertex3f(1.5f, 0.5f, 0.5f);
	glTexCoord2f(2.0f, 2.0f); glVertex3f(1.5f, 0.5f, -0.5f);
	glTexCoord2f(2.0f, 0.0f); glVertex3f(-0.5f, 0.5f, -0.5f);


	glEnd();

	glPopMatrix();
}

void CScene7::MultiTextureStair(float pX, float pY, float pZ,
	float rX, float rY, float rZ, float angle,
	int texID)
{
	// Seta a textura ativa
	//if (texID >= 0) {
	//	pTextures->ApplyTexture(texID);

	//	// Configura a repetição da textura
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//}

	// Unidade de Textura 0
	glPushAttrib(GL_TEXTURE_BIT);
	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
	pTextures->ApplyTexture(12);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
	glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_REPLACE);

	// Unidade de Textura 1
	glActiveTexture(GL_TEXTURE1);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_TEXTURE_2D);
	pTextures->ApplyTexture(13);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
	glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);

	//pTextures->ApplyTexture(6);

	glPushMatrix();
	glTranslatef(pX, pY, pZ);
	glRotatef(angle, rX, rY, rZ);

	glBegin(GL_QUADS);

	glNormal3f(0.0f, 0.0f, 1.0f);
	// face frente
	glMultiTexCoord2f(GL_TEXTURE0, 0.0f, 0.0f);
	glVertex3f(-0.5f, -0.5f, 0.5f);

	glMultiTexCoord2f(GL_TEXTURE0, 2.0f, 0.0f);
	glVertex3f(15.0f, -0.5f, 0.5f);

	glMultiTexCoord2f(GL_TEXTURE0, 2.0f, 2.0f);
	glVertex3f(15.0f, 0.5f, 0.5f);

	glMultiTexCoord2f(GL_TEXTURE0, 0.0f, 2.0f);
	glVertex3f(-0.5f, 0.5f, 0.5f);

	// face trás
	glMultiTexCoord2f(GL_TEXTURE0, 0.0f, 0.0f);
	glVertex3f(15.0f, -0.5f, -4.5f);

	glMultiTexCoord2f(GL_TEXTURE0, 0.0f, 2.0f);
	glVertex3f(-0.5f, -0.5f, -4.5f);

	glMultiTexCoord2f(GL_TEXTURE0, 2.0f, 2.0f);
	glVertex3f(-0.5f, 0.5f, -4.5f);

	glMultiTexCoord2f(GL_TEXTURE0, 2.0f, 0.0f);
	glVertex3f(15.0f, 0.5f, -4.5f);

	// face direita
	glMultiTexCoord2f(GL_TEXTURE0, 0.0f, 0.0f);
	glVertex3f(15.0f, -0.5f, 0.5f);

	glMultiTexCoord2f(GL_TEXTURE0, 0.0f, 1.0f);
	glVertex3f(15.0f, -0.5f, -4.5f);

	glMultiTexCoord2f(GL_TEXTURE0, 1.0f, 1.0f);
	glVertex3f(15.0f, 0.5f, -4.5f);

	glMultiTexCoord2f(GL_TEXTURE0, 1.0f, 0.0f);
	glVertex3f(15.0f, 0.5f, 0.5f);


	// face esquerda
	glMultiTexCoord2f(GL_TEXTURE0, 0.0f, 0.0f);
	glVertex3f(-0.5f, -0.5f, -4.5f);

	glMultiTexCoord2f(GL_TEXTURE0, 1.0f, 0.0f);
	glVertex3f(-0.5f, -0.5f, 0.5f);

	glMultiTexCoord2f(GL_TEXTURE0, 1.0f, 1.0f);
	glVertex3f(-0.5f, 0.5f, 0.5f);

	glMultiTexCoord2f(GL_TEXTURE0, 0.0f, 1.0f);
	glVertex3f(-0.5f, 0.5f, -4.5f);

	// face baixo
	glMultiTexCoord2f(GL_TEXTURE0, 0.0f, 0.0f);
	glVertex3f(-0.5f, -0.5f, -4.5f);

	glMultiTexCoord2f(GL_TEXTURE0, 0.0f, 2.0f);
	glVertex3f(15.0f, -0.5f, -4.5f);

	glMultiTexCoord2f(GL_TEXTURE0, 2.0f, 2.0f);
	glVertex3f(15.0f, -0.5f, 0.5f);

	glMultiTexCoord2f(GL_TEXTURE0, 2.0f, 0.0f);
	glVertex3f(-0.5f, -0.5f, 0.5f);

	// face cima
	glMultiTexCoord2f(GL_TEXTURE0, 0.0f, 0.0f);
	glMultiTexCoord2f(GL_TEXTURE1, 0.0f, 0.0f);
	glVertex3f(-0.5f, 0.5f, 0.5f);

	glMultiTexCoord2f(GL_TEXTURE0, 0.0f, 2.0f);
	glMultiTexCoord2f(GL_TEXTURE1, 0.0f, -1.0f);
	glVertex3f(15.0f, 0.5f, 0.5f);

	glMultiTexCoord2f(GL_TEXTURE0, 2.0f, 2.0f);
	glMultiTexCoord2f(GL_TEXTURE1, -1.0f, -1.0f);
	glVertex3f(15.0f, 0.5f, -4.5f);

	glMultiTexCoord2f(GL_TEXTURE0, 2.0f, 0.0f);
	glMultiTexCoord2f(GL_TEXTURE1, -1.0f, 0.0f);
	glVertex3f(-0.5f, 0.5f, -4.5f);

	glEnd();
	glPopMatrix();

	glActiveTexture(GL_TEXTURE1);
	glDisable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glDisable(GL_TEXTURE_2D);
	glPopAttrib();
}


void CScene7::MouseMove(void) // Tratamento de movimento do mouse
{
	// Realiza os cálculos de rotação da visão do Player (através das coordenadas
	// X do mouse.
	POINT mousePos;
	int middleX = WIDTH >> 1;
	int middleY = HEIGHT >> 1;

	GetCursorPos(&mousePos);

	if ((mousePos.x == middleX) && (mousePos.y == middleY)) return;

	SetCursorPos(middleX, middleY);

	fDeltaX = (float)((middleX - mousePos.x)) / 10;
	fDeltaY = (float)((middleY - mousePos.y)) / 10;

	// Rotaciona apenas a câmera
	pCamera->rotateGlob(-fDeltaX, 0.0f, 1.0f, 0.0f);
	pCamera->rotateLoc(-fDeltaY, 1.0f, 0.0f, 0.0f);
}

void CScene7::KeyPressed(void) // Tratamento de teclas pressionadas
{

	// Verifica se a tecla 'W' foi pressionada e move o Player para frente
	if (GetKeyState('W') & 0x80)
	{
		pCamera->moveGlob(pCamera->Forward[0], pCamera->Forward[1], pCamera->Forward[2]);
	}
	// Verifica se a tecla 'S' foi pressionada e move o Player para tras
	else if (GetKeyState('S') & 0x80)
	{
		pCamera->moveGlob(-pCamera->Forward[0], -pCamera->Forward[1], -pCamera->Forward[2]);
	}
	// Verifica se a tecla 'A' foi pressionada e move o Player para esquerda
	else if (GetKeyState('A') & 0x80)
	{
		pCamera->moveGlob(-pCamera->Right[0], -pCamera->Right[1], -pCamera->Right[2]);
	}
	// Verifica se a tecla 'D' foi pressionada e move o Player para direira
	else if (GetKeyState('D') & 0x80)
	{
		pCamera->moveGlob(pCamera->Right[0], pCamera->Right[1], pCamera->Right[2]);
	}
	else if (GetKeyState('Q') & 0x80)
	{
		pCamera->moveGlob(0.0f, -pCamera->Up[1], 0.0f);
	}
	else if (GetKeyState('E') & 0x80)
	{
		pCamera->moveGlob(0.0f, pCamera->Up[1], 0.0f);
	}
	// Senão, interrompe movimento do Player
	else if (GetKeyState('F') & 0x80)
	{
		enabledFog = !enabledFog;

		if (enabledFog == false) {
			glDisable(GL_FOG);
		}
	}
	else if (GetKeyState('G') & 0x80)
	{
		enabledSpotlight = !enabledSpotlight;
		if (enabledSpotlight == false) {
			glDisable(GL_LIGHT0);
			glDisable(GL_LIGHTING);
		}
		else {
			glEnable(GL_LIGHTING);
			glEnable(GL_LIGHT0);
		}
	}
	else if (GetKeyState('R') & 0x80)
	{
		pCamera->Position[0] = -39.23f;
		pCamera->Position[1] = 119.02f;
		pCamera->Position[2] = -16.25f;
	}

	if (GetKeyState(VK_UP) & 0x80 && enabledSpotlight)
	{
		LightPosition[2] -= 0.8f;
	}
	if (GetKeyState(VK_DOWN) & 0x80 && enabledSpotlight)
	{
		LightPosition[2] += 0.8f;
	}
	if (GetKeyState(VK_LEFT) & 0x80 && enabledSpotlight)
	{
		LightPosition[0] -= 0.8f;
	}
	if (GetKeyState(VK_RIGHT) & 0x80 && enabledSpotlight)
	{
		LightPosition[0] += 0.8f;
	}
	if (GetKeyState(VK_PRIOR) & 0x80 && enabledSpotlight)
	{
		LightPosition[1] += 0.8f;
	}
	if (GetKeyState(VK_NEXT) & 0x80 && enabledSpotlight)
	{
		LightPosition[1] -= 0.8f;
	}
}

void CScene7::KeyDownPressed(WPARAM	wParam) // Tratamento de teclas pressionadas
{
	switch (wParam)
	{
	case VK_TAB:
		bIsWireframe = !bIsWireframe;
		break;

	case VK_SPACE:
	{
		pTimer->Init();
	}
		break;

	case VK_RETURN:

		break;


	}

}

//	Cria um grid horizontal ao longo dos eixos X e Z
void CScene7::Draw3DSGrid(float width, float length)
{

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(0.0f, 0.3f, 0.0f);
	glPushMatrix();
	for (float i = -width; i <= length; i += 1)
	{
		for (float j = -width; j <= length; j += 1)
		{
			// inicia o desenho das linhas
			glBegin(GL_QUADS);
			glNormal3f(0.0f, 1.0f, 0.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(i, 0.0f, j + 1);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(i + 1, 0.0f, j + 1);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(i + 1, 0.0f, j);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(i, 0.0f, j);
			glEnd();
		}
	}
	glPopMatrix();
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}


void CScene7::DrawAxis()
{
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);
	// Eixo X
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-1000.0f, 0.0f, 0.0f);
	glVertex3f(1000.0f, 0.0f, 0.0f);

	// Eixo Y
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 1000.0f, 0.0f);
	glVertex3f(0.0f, -1000.0f, 0.0f);

	// Eixo Z
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 1000.0f);
	glVertex3f(0.0f, 0.0f, -1000.0f);
	glEnd();
	glPopMatrix();
}


void CScene7::CreateSkyBox(float x, float y, float z,
	float width, float height, float length,
	CTexture* pTextures)
{
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glPushMatrix();

	// Centraliza o Skybox em torno da posição especificada(x, y, z)
	x = x - width / 2;
	y = y - height / 2;
	z = z - length / 2;


	// Aplica a textura que representa a parte da frente do skybox (BACK map)
	pTextures->ApplyTexture(0);

	// Desenha face BACK do cubo do skybox
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y, z);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y, z);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + height, z);
	glEnd();


	// Aplica a textura que representa a parte da frente do skybox (FRONT map)
	pTextures->ApplyTexture(1);

	// Desenha face FRONT do cubo do skybox
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z + length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z + length);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y, z + length);
	glEnd();


	// Aplica a textura que representa a parte da frente do skybox (DOWN map)
	pTextures->ApplyTexture(2);

	// Desenha face BOTTOM do cubo do skybox
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y, z + length);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y, z);
	glEnd();


	// Aplica a textura que representa a parte da frente do skybox (UP map)
	pTextures->ApplyTexture(3);

	// Desenha face TOP do cubo do skybox
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y + height, z + length);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y + height, z + length);
	glEnd();


	// Aplica a textura que representa a parte da frente do skybox (LEFT map)
	pTextures->ApplyTexture(4);

	// Desenha face LEFT do cubo do skybox
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + height, z + length);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y, z + length);
	glEnd();


	// Aplica a textura que representa a parte da frente do skybox (RIGHT map)
	pTextures->ApplyTexture(5);

	// Desenha face RIGHT do cubo do skybox
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y, z);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y, z + length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z);
	glEnd();

	glPopMatrix();
}

//bool CScene7::loadModel(const string& filePath) {
//	Assimp::Importer importer;
//	const aiScene* scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs);
//
//	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
//		// Erro ao carregar o modelo
//		return false;
//	}
//
//	return true;
//}