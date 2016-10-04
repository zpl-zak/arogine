#ifndef SHADER_HPP
#define SHADER_HPP

#define SHADER_VS(x) "shader/"##x##.vs
#define SHADER_FS(x) "shader/"##x##.fs

GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path);

#endif
