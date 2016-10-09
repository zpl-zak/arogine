// /*
//  Copyright 2016 AROGAN Group
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//  http://www.apache.org/licenses/LICENSE-2.0
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.
// */
#pragma once

#include "defs.hpp"

class BaseShader
{
public:
	BaseShader();
	virtual ~BaseShader(){}

	auto GetProgram() const -> GLuint;
	virtual void Publish(mat4 v, mat4 p, vec3 lit, vec3 lic) const;
	
protected:
	GLuint mProgram;
	GLuint mmID;
	GLuint mvID;
	GLuint mpID;
	GLuint lnID;
	GLuint lcID;
};
