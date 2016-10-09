#include "RaymarchScene.h"


RaymarchScene::RaymarchScene(Camera* camera): mCamera(camera)
{
	BuildQuadData();
}

void RaymarchScene::Render() const
{
	mRaymarchShader.Publish(
		mCamera->GetViewMatrix(),
		mCamera->GetProjectionMatrix(),
		vec3(1.f), vec3(1.f)
	);

	glBindVertexArray(QuadVAO);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	{
		glBindBuffer(GL_ARRAY_BUFFER, QuadVBO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, static_cast<void*>(nullptr));
		glBindBuffer(GL_ARRAY_BUFFER, QuadUVVBO);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, static_cast<void*>(nullptr));
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
}

void RaymarchScene::BuildQuadData()
{
	if (QuadVBO)return;

	glGenVertexArrays(1, &QuadVAO);
	glBindVertexArray(QuadVAO);

	glGenBuffers(1, &QuadVBO);
	glBindBuffer(GL_ARRAY_BUFFER, QuadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*18, QuadData, GL_STATIC_DRAW);

	glGenBuffers(1, &QuadUVVBO);
	glBindBuffer(GL_ARRAY_BUFFER, QuadUVVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 8, QuadDataUV, GL_STATIC_DRAW);
}

const GLfloat RaymarchScene::QuadData[] = {
	-1.0f, -1.0f, 0.0f,
	1.0f, -1.0f, 0.0f,
	-1.0f,  1.0f, 0.0f,
	-1.0f,  1.0f, 0.0f,
	1.0f, -1.0f, 0.0f,
	1.0f,  1.0f, 0.0f,
};

const GLfloat RaymarchScene::QuadDataUV[] = {
	1,0,
	1,1,
	0,1,
	0,0,
	
};

GLuint RaymarchScene::QuadVBO;
GLuint RaymarchScene::QuadUVVBO;
GLuint RaymarchScene::QuadVAO;
