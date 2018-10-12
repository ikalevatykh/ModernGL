#include "GLMethods.hpp"

#include "Types.hpp"

void AddToLog(PyObject ** pstr, const char * function) {
	PyObject * name = PyUnicode_FromFormat("- %s\n", function);
	PyUnicode_Append(pstr, name);
	Py_DECREF(name);
}

#define INVALID_METHOD(method) (!(method))

void GLAPI FakeGetProgramStageiv(GLuint program, GLenum shadertype, GLenum pname, GLint * values) {
	values[0] = 0;
}

#if defined(_WIN32) || defined(_WIN64)

#define PREFIX ""

#include <Windows.h>

#ifdef MemoryBarrier
#undef MemoryBarrier
#endif

void * GLMethods::loadMethod(const char * method) {
	static HMODULE opengl32 = LoadLibrary("opengl32.dll");

	void * proc = (void *)GetProcAddress(opengl32, method);

	if (proc) {
		return proc;
	}

	proc = (void *)wglGetProcAddress(method);

	if (proc) {
		return proc;
	}

	// printf("%s NOT found!\n", method);
	return 0;
}

#elif defined(__APPLE__)

#define PREFIX "_"

#import <mach-o/dyld.h>
#import <stdlib.h>
#import <string.h>

void * GLMethods::loadMethod(const char * method) {
	NSSymbol symbol = 0;

	if (NSIsSymbolNameDefined(method)) {
		symbol = NSLookupAndBindSymbol(method);
	}

	return symbol ? NSAddressOfSymbol(symbol) : 0;
}

#else

#define PREFIX ""

#include <dlfcn.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

void * GLMethods::loadMethod(const char * method) {
	void * proc = (void *)dlsym(this->libgl, method);

	if (proc) {
		// printf("%s found!\n", method);
		return proc;
	}

	proc = (void *)this->GetProcAddress(method);

	if (proc) {
		// printf("%s found!\n", method);
		return proc;
	}

	// printf("%s NOT found!\n", method);
	return 0;
}

#endif

bool GLMethods::load() {
	this->ActiveShaderProgram = (PROC_glActiveShaderProgram)this->loadMethod(PREFIX "glActiveShaderProgram");
	this->ActiveTexture = (PROC_glActiveTexture)this->loadMethod(PREFIX "glActiveTexture");
	this->AttachShader = (PROC_glAttachShader)this->loadMethod(PREFIX "glAttachShader");
	this->BeginConditionalRender = (PROC_glBeginConditionalRender)this->loadMethod(PREFIX "glBeginConditionalRender");
	this->BeginQuery = (PROC_glBeginQuery)this->loadMethod(PREFIX "glBeginQuery");
	this->BeginQueryIndexed = (PROC_glBeginQueryIndexed)this->loadMethod(PREFIX "glBeginQueryIndexed");
	this->BeginTransformFeedback = (PROC_glBeginTransformFeedback)this->loadMethod(PREFIX "glBeginTransformFeedback");
	this->BindAttribLocation = (PROC_glBindAttribLocation)this->loadMethod(PREFIX "glBindAttribLocation");
	this->BindBuffer = (PROC_glBindBuffer)this->loadMethod(PREFIX "glBindBuffer");
	this->BindBufferBase = (PROC_glBindBufferBase)this->loadMethod(PREFIX "glBindBufferBase");
	this->BindBufferRange = (PROC_glBindBufferRange)this->loadMethod(PREFIX "glBindBufferRange");
	this->BindBuffersBase = (PROC_glBindBuffersBase)this->loadMethod(PREFIX "glBindBuffersBase");
	this->BindBuffersRange = (PROC_glBindBuffersRange)this->loadMethod(PREFIX "glBindBuffersRange");
	this->BindFragDataLocation = (PROC_glBindFragDataLocation)this->loadMethod(PREFIX "glBindFragDataLocation");
	this->BindFragDataLocationIndexed = (PROC_glBindFragDataLocationIndexed)this->loadMethod(PREFIX "glBindFragDataLocationIndexed");
	this->BindFramebuffer = (PROC_glBindFramebuffer)this->loadMethod(PREFIX "glBindFramebuffer");
	this->BindImageTexture = (PROC_glBindImageTexture)this->loadMethod(PREFIX "glBindImageTexture");
	this->BindImageTextures = (PROC_glBindImageTextures)this->loadMethod(PREFIX "glBindImageTextures");
	this->BindProgramPipeline = (PROC_glBindProgramPipeline)this->loadMethod(PREFIX "glBindProgramPipeline");
	this->BindRenderbuffer = (PROC_glBindRenderbuffer)this->loadMethod(PREFIX "glBindRenderbuffer");
	this->BindSampler = (PROC_glBindSampler)this->loadMethod(PREFIX "glBindSampler");
	this->BindSamplers = (PROC_glBindSamplers)this->loadMethod(PREFIX "glBindSamplers");
	this->BindTexture = (PROC_glBindTexture)this->loadMethod(PREFIX "glBindTexture");
	this->BindTextureUnit = (PROC_glBindTextureUnit)this->loadMethod(PREFIX "glBindTextureUnit");
	this->BindTextures = (PROC_glBindTextures)this->loadMethod(PREFIX "glBindTextures");
	this->BindTransformFeedback = (PROC_glBindTransformFeedback)this->loadMethod(PREFIX "glBindTransformFeedback");
	this->BindVertexArray = (PROC_glBindVertexArray)this->loadMethod(PREFIX "glBindVertexArray");
	this->BindVertexBuffer = (PROC_glBindVertexBuffer)this->loadMethod(PREFIX "glBindVertexBuffer");
	this->BindVertexBuffers = (PROC_glBindVertexBuffers)this->loadMethod(PREFIX "glBindVertexBuffers");
	this->BlendColor = (PROC_glBlendColor)this->loadMethod(PREFIX "glBlendColor");
	this->BlendEquation = (PROC_glBlendEquation)this->loadMethod(PREFIX "glBlendEquation");
	this->BlendEquationSeparate = (PROC_glBlendEquationSeparate)this->loadMethod(PREFIX "glBlendEquationSeparate");
	this->BlendEquationSeparatei = (PROC_glBlendEquationSeparatei)this->loadMethod(PREFIX "glBlendEquationSeparatei");
	this->BlendEquationi = (PROC_glBlendEquationi)this->loadMethod(PREFIX "glBlendEquationi");
	this->BlendFunc = (PROC_glBlendFunc)this->loadMethod(PREFIX "glBlendFunc");
	this->BlendFuncSeparate = (PROC_glBlendFuncSeparate)this->loadMethod(PREFIX "glBlendFuncSeparate");
	this->BlendFuncSeparatei = (PROC_glBlendFuncSeparatei)this->loadMethod(PREFIX "glBlendFuncSeparatei");
	this->BlendFunci = (PROC_glBlendFunci)this->loadMethod(PREFIX "glBlendFunci");
	this->BlitFramebuffer = (PROC_glBlitFramebuffer)this->loadMethod(PREFIX "glBlitFramebuffer");
	this->BlitNamedFramebuffer = (PROC_glBlitNamedFramebuffer)this->loadMethod(PREFIX "glBlitNamedFramebuffer");
	this->BufferData = (PROC_glBufferData)this->loadMethod(PREFIX "glBufferData");
	this->BufferStorage = (PROC_glBufferStorage)this->loadMethod(PREFIX "glBufferStorage");
	this->BufferSubData = (PROC_glBufferSubData)this->loadMethod(PREFIX "glBufferSubData");
	this->CheckFramebufferStatus = (PROC_glCheckFramebufferStatus)this->loadMethod(PREFIX "glCheckFramebufferStatus");
	this->CheckNamedFramebufferStatus = (PROC_glCheckNamedFramebufferStatus)this->loadMethod(PREFIX "glCheckNamedFramebufferStatus");
	this->ClampColor = (PROC_glClampColor)this->loadMethod(PREFIX "glClampColor");
	this->Clear = (PROC_glClear)this->loadMethod(PREFIX "glClear");
	this->ClearBufferData = (PROC_glClearBufferData)this->loadMethod(PREFIX "glClearBufferData");
	this->ClearBufferSubData = (PROC_glClearBufferSubData)this->loadMethod(PREFIX "glClearBufferSubData");
	this->ClearBufferfi = (PROC_glClearBufferfi)this->loadMethod(PREFIX "glClearBufferfi");
	this->ClearBufferfv = (PROC_glClearBufferfv)this->loadMethod(PREFIX "glClearBufferfv");
	this->ClearBufferiv = (PROC_glClearBufferiv)this->loadMethod(PREFIX "glClearBufferiv");
	this->ClearBufferuiv = (PROC_glClearBufferuiv)this->loadMethod(PREFIX "glClearBufferuiv");
	this->ClearColor = (PROC_glClearColor)this->loadMethod(PREFIX "glClearColor");
	this->ClearDepth = (PROC_glClearDepth)this->loadMethod(PREFIX "glClearDepth");
	this->ClearDepthf = (PROC_glClearDepthf)this->loadMethod(PREFIX "glClearDepthf");
	this->ClearNamedBufferData = (PROC_glClearNamedBufferData)this->loadMethod(PREFIX "glClearNamedBufferData");
	this->ClearNamedBufferSubData = (PROC_glClearNamedBufferSubData)this->loadMethod(PREFIX "glClearNamedBufferSubData");
	this->ClearNamedFramebufferfi = (PROC_glClearNamedFramebufferfi)this->loadMethod(PREFIX "glClearNamedFramebufferfi");
	this->ClearNamedFramebufferfv = (PROC_glClearNamedFramebufferfv)this->loadMethod(PREFIX "glClearNamedFramebufferfv");
	this->ClearNamedFramebufferiv = (PROC_glClearNamedFramebufferiv)this->loadMethod(PREFIX "glClearNamedFramebufferiv");
	this->ClearNamedFramebufferuiv = (PROC_glClearNamedFramebufferuiv)this->loadMethod(PREFIX "glClearNamedFramebufferuiv");
	this->ClearStencil = (PROC_glClearStencil)this->loadMethod(PREFIX "glClearStencil");
	this->ClearTexImage = (PROC_glClearTexImage)this->loadMethod(PREFIX "glClearTexImage");
	this->ClearTexSubImage = (PROC_glClearTexSubImage)this->loadMethod(PREFIX "glClearTexSubImage");
	this->ClientWaitSync = (PROC_glClientWaitSync)this->loadMethod(PREFIX "glClientWaitSync");
	this->WaitSync = (PROC_glWaitSync)this->loadMethod(PREFIX "glWaitSync");
	this->ClipControl = (PROC_glClipControl)this->loadMethod(PREFIX "glClipControl");
	this->ColorMask = (PROC_glColorMask)this->loadMethod(PREFIX "glColorMask");
	this->ColorMaski = (PROC_glColorMaski)this->loadMethod(PREFIX "glColorMaski");
	this->CompileShader = (PROC_glCompileShader)this->loadMethod(PREFIX "glCompileShader");
	this->CompressedTexImage1D = (PROC_glCompressedTexImage1D)this->loadMethod(PREFIX "glCompressedTexImage1D");
	this->CompressedTexImage2D = (PROC_glCompressedTexImage2D)this->loadMethod(PREFIX "glCompressedTexImage2D");
	this->CompressedTexImage3D = (PROC_glCompressedTexImage3D)this->loadMethod(PREFIX "glCompressedTexImage3D");
	this->CompressedTexSubImage1D = (PROC_glCompressedTexSubImage1D)this->loadMethod(PREFIX "glCompressedTexSubImage1D");
	this->CompressedTexSubImage2D = (PROC_glCompressedTexSubImage2D)this->loadMethod(PREFIX "glCompressedTexSubImage2D");
	this->CompressedTexSubImage3D = (PROC_glCompressedTexSubImage3D)this->loadMethod(PREFIX "glCompressedTexSubImage3D");
	this->CompressedTextureSubImage1D = (PROC_glCompressedTextureSubImage1D)this->loadMethod(PREFIX "glCompressedTextureSubImage1D");
	this->CompressedTextureSubImage2D = (PROC_glCompressedTextureSubImage2D)this->loadMethod(PREFIX "glCompressedTextureSubImage2D");
	this->CompressedTextureSubImage3D = (PROC_glCompressedTextureSubImage3D)this->loadMethod(PREFIX "glCompressedTextureSubImage3D");
	this->CopyBufferSubData = (PROC_glCopyBufferSubData)this->loadMethod(PREFIX "glCopyBufferSubData");
	this->CopyImageSubData = (PROC_glCopyImageSubData)this->loadMethod(PREFIX "glCopyImageSubData");
	this->CopyNamedBufferSubData = (PROC_glCopyNamedBufferSubData)this->loadMethod(PREFIX "glCopyNamedBufferSubData");
	this->CopyTexImage1D = (PROC_glCopyTexImage1D)this->loadMethod(PREFIX "glCopyTexImage1D");
	this->CopyTexImage2D = (PROC_glCopyTexImage2D)this->loadMethod(PREFIX "glCopyTexImage2D");
	this->CopyTexSubImage1D = (PROC_glCopyTexSubImage1D)this->loadMethod(PREFIX "glCopyTexSubImage1D");
	this->CopyTexSubImage2D = (PROC_glCopyTexSubImage2D)this->loadMethod(PREFIX "glCopyTexSubImage2D");
	this->CopyTexSubImage3D = (PROC_glCopyTexSubImage3D)this->loadMethod(PREFIX "glCopyTexSubImage3D");
	this->CopyTextureSubImage1D = (PROC_glCopyTextureSubImage1D)this->loadMethod(PREFIX "glCopyTextureSubImage1D");
	this->CopyTextureSubImage2D = (PROC_glCopyTextureSubImage2D)this->loadMethod(PREFIX "glCopyTextureSubImage2D");
	this->CopyTextureSubImage3D = (PROC_glCopyTextureSubImage3D)this->loadMethod(PREFIX "glCopyTextureSubImage3D");
	this->CreateBuffers = (PROC_glCreateBuffers)this->loadMethod(PREFIX "glCreateBuffers");
	this->CreateFramebuffers = (PROC_glCreateFramebuffers)this->loadMethod(PREFIX "glCreateFramebuffers");
	this->CreateProgram = (PROC_glCreateProgram)this->loadMethod(PREFIX "glCreateProgram");
	this->CreateProgramPipelines = (PROC_glCreateProgramPipelines)this->loadMethod(PREFIX "glCreateProgramPipelines");
	this->CreateQueries = (PROC_glCreateQueries)this->loadMethod(PREFIX "glCreateQueries");
	this->CreateRenderbuffers = (PROC_glCreateRenderbuffers)this->loadMethod(PREFIX "glCreateRenderbuffers");
	this->CreateSamplers = (PROC_glCreateSamplers)this->loadMethod(PREFIX "glCreateSamplers");
	this->CreateShader = (PROC_glCreateShader)this->loadMethod(PREFIX "glCreateShader");
	this->CreateShaderProgramv = (PROC_glCreateShaderProgramv)this->loadMethod(PREFIX "glCreateShaderProgramv");
	this->CreateTextures = (PROC_glCreateTextures)this->loadMethod(PREFIX "glCreateTextures");
	this->CreateTransformFeedbacks = (PROC_glCreateTransformFeedbacks)this->loadMethod(PREFIX "glCreateTransformFeedbacks");
	this->CreateVertexArrays = (PROC_glCreateVertexArrays)this->loadMethod(PREFIX "glCreateVertexArrays");
	this->CullFace = (PROC_glCullFace)this->loadMethod(PREFIX "glCullFace");
	this->DebugMessageCallback = (PROC_glDebugMessageCallback)this->loadMethod(PREFIX "glDebugMessageCallback");
	this->DebugMessageControl = (PROC_glDebugMessageControl)this->loadMethod(PREFIX "glDebugMessageControl");
	this->DebugMessageInsert = (PROC_glDebugMessageInsert)this->loadMethod(PREFIX "glDebugMessageInsert");
	this->DeleteBuffers = (PROC_glDeleteBuffers)this->loadMethod(PREFIX "glDeleteBuffers");
	this->DeleteFramebuffers = (PROC_glDeleteFramebuffers)this->loadMethod(PREFIX "glDeleteFramebuffers");
	this->DeleteProgram = (PROC_glDeleteProgram)this->loadMethod(PREFIX "glDeleteProgram");
	this->DeleteProgramPipelines = (PROC_glDeleteProgramPipelines)this->loadMethod(PREFIX "glDeleteProgramPipelines");
	this->DeleteQueries = (PROC_glDeleteQueries)this->loadMethod(PREFIX "glDeleteQueries");
	this->DeleteRenderbuffers = (PROC_glDeleteRenderbuffers)this->loadMethod(PREFIX "glDeleteRenderbuffers");
	this->DeleteSamplers = (PROC_glDeleteSamplers)this->loadMethod(PREFIX "glDeleteSamplers");
	this->DeleteShader = (PROC_glDeleteShader)this->loadMethod(PREFIX "glDeleteShader");
	this->DeleteSync = (PROC_glDeleteSync)this->loadMethod(PREFIX "glDeleteSync");
	this->DeleteTextures = (PROC_glDeleteTextures)this->loadMethod(PREFIX "glDeleteTextures");
	this->DeleteTransformFeedbacks = (PROC_glDeleteTransformFeedbacks)this->loadMethod(PREFIX "glDeleteTransformFeedbacks");
	this->DeleteVertexArrays = (PROC_glDeleteVertexArrays)this->loadMethod(PREFIX "glDeleteVertexArrays");
	this->DepthFunc = (PROC_glDepthFunc)this->loadMethod(PREFIX "glDepthFunc");
	this->DepthMask = (PROC_glDepthMask)this->loadMethod(PREFIX "glDepthMask");
	this->DepthRange = (PROC_glDepthRange)this->loadMethod(PREFIX "glDepthRange");
	this->DepthRangeArrayv = (PROC_glDepthRangeArrayv)this->loadMethod(PREFIX "glDepthRangeArrayv");
	this->DepthRangeIndexed = (PROC_glDepthRangeIndexed)this->loadMethod(PREFIX "glDepthRangeIndexed");
	this->DepthRangef = (PROC_glDepthRangef)this->loadMethod(PREFIX "glDepthRangef");
	this->DetachShader = (PROC_glDetachShader)this->loadMethod(PREFIX "glDetachShader");
	this->Disable = (PROC_glDisable)this->loadMethod(PREFIX "glDisable");
	this->DisableVertexArrayAttrib = (PROC_glDisableVertexArrayAttrib)this->loadMethod(PREFIX "glDisableVertexArrayAttrib");
	this->DisableVertexAttribArray = (PROC_glDisableVertexAttribArray)this->loadMethod(PREFIX "glDisableVertexAttribArray");
	this->Disablei = (PROC_glDisablei)this->loadMethod(PREFIX "glDisablei");
	this->DispatchCompute = (PROC_glDispatchCompute)this->loadMethod(PREFIX "glDispatchCompute");
	this->DispatchComputeIndirect = (PROC_glDispatchComputeIndirect)this->loadMethod(PREFIX "glDispatchComputeIndirect");
	this->DrawArrays = (PROC_glDrawArrays)this->loadMethod(PREFIX "glDrawArrays");
	this->DrawArraysIndirect = (PROC_glDrawArraysIndirect)this->loadMethod(PREFIX "glDrawArraysIndirect");
	this->DrawArraysInstanced = (PROC_glDrawArraysInstanced)this->loadMethod(PREFIX "glDrawArraysInstanced");
	this->DrawArraysInstancedBaseInstance = (PROC_glDrawArraysInstancedBaseInstance)this->loadMethod(PREFIX "glDrawArraysInstancedBaseInstance");
	this->DrawBuffer = (PROC_glDrawBuffer)this->loadMethod(PREFIX "glDrawBuffer");
	this->DrawBuffers = (PROC_glDrawBuffers)this->loadMethod(PREFIX "glDrawBuffers");
	this->DrawElements = (PROC_glDrawElements)this->loadMethod(PREFIX "glDrawElements");
	this->DrawElementsBaseVertex = (PROC_glDrawElementsBaseVertex)this->loadMethod(PREFIX "glDrawElementsBaseVertex");
	this->DrawElementsIndirect = (PROC_glDrawElementsIndirect)this->loadMethod(PREFIX "glDrawElementsIndirect");
	this->DrawElementsInstanced = (PROC_glDrawElementsInstanced)this->loadMethod(PREFIX "glDrawElementsInstanced");
	this->DrawElementsInstancedBaseInstance = (PROC_glDrawElementsInstancedBaseInstance)this->loadMethod(PREFIX "glDrawElementsInstancedBaseInstance");
	this->DrawElementsInstancedBaseVertex = (PROC_glDrawElementsInstancedBaseVertex)this->loadMethod(PREFIX "glDrawElementsInstancedBaseVertex");
	this->DrawElementsInstancedBaseVertexBaseInstance = (PROC_glDrawElementsInstancedBaseVertexBaseInstance)this->loadMethod(PREFIX "glDrawElementsInstancedBaseVertexBaseInstance");
	this->DrawRangeElements = (PROC_glDrawRangeElements)this->loadMethod(PREFIX "glDrawRangeElements");
	this->DrawRangeElementsBaseVertex = (PROC_glDrawRangeElementsBaseVertex)this->loadMethod(PREFIX "glDrawRangeElementsBaseVertex");
	this->DrawTransformFeedback = (PROC_glDrawTransformFeedback)this->loadMethod(PREFIX "glDrawTransformFeedback");
	this->DrawTransformFeedbackInstanced = (PROC_glDrawTransformFeedbackInstanced)this->loadMethod(PREFIX "glDrawTransformFeedbackInstanced");
	this->DrawTransformFeedbackStream = (PROC_glDrawTransformFeedbackStream)this->loadMethod(PREFIX "glDrawTransformFeedbackStream");
	this->DrawTransformFeedbackStreamInstanced = (PROC_glDrawTransformFeedbackStreamInstanced)this->loadMethod(PREFIX "glDrawTransformFeedbackStreamInstanced");
	this->Enable = (PROC_glEnable)this->loadMethod(PREFIX "glEnable");
	this->EnableVertexArrayAttrib = (PROC_glEnableVertexArrayAttrib)this->loadMethod(PREFIX "glEnableVertexArrayAttrib");
	this->EnableVertexAttribArray = (PROC_glEnableVertexAttribArray)this->loadMethod(PREFIX "glEnableVertexAttribArray");
	this->Enablei = (PROC_glEnablei)this->loadMethod(PREFIX "glEnablei");
	this->EndConditionalRender = (PROC_glEndConditionalRender)this->loadMethod(PREFIX "glEndConditionalRender");
	this->EndQuery = (PROC_glEndQuery)this->loadMethod(PREFIX "glEndQuery");
	this->EndQueryIndexed = (PROC_glEndQueryIndexed)this->loadMethod(PREFIX "glEndQueryIndexed");
	this->EndTransformFeedback = (PROC_glEndTransformFeedback)this->loadMethod(PREFIX "glEndTransformFeedback");
	this->FenceSync = (PROC_glFenceSync)this->loadMethod(PREFIX "glFenceSync");
	this->Finish = (PROC_glFinish)this->loadMethod(PREFIX "glFinish");
	this->Flush = (PROC_glFlush)this->loadMethod(PREFIX "glFlush");
	this->FlushMappedBufferRange = (PROC_glFlushMappedBufferRange)this->loadMethod(PREFIX "glFlushMappedBufferRange");
	this->FlushMappedNamedBufferRange = (PROC_glFlushMappedNamedBufferRange)this->loadMethod(PREFIX "glFlushMappedNamedBufferRange");
	this->FramebufferParameteri = (PROC_glFramebufferParameteri)this->loadMethod(PREFIX "glFramebufferParameteri");
	this->FramebufferRenderbuffer = (PROC_glFramebufferRenderbuffer)this->loadMethod(PREFIX "glFramebufferRenderbuffer");
	this->FramebufferTexture = (PROC_glFramebufferTexture)this->loadMethod(PREFIX "glFramebufferTexture");
	this->FramebufferTexture1D = (PROC_glFramebufferTexture1D)this->loadMethod(PREFIX "glFramebufferTexture1D");
	this->FramebufferTexture2D = (PROC_glFramebufferTexture2D)this->loadMethod(PREFIX "glFramebufferTexture2D");
	this->FramebufferTexture3D = (PROC_glFramebufferTexture3D)this->loadMethod(PREFIX "glFramebufferTexture3D");
	this->FramebufferTextureLayer = (PROC_glFramebufferTextureLayer)this->loadMethod(PREFIX "glFramebufferTextureLayer");
	this->FrontFace = (PROC_glFrontFace)this->loadMethod(PREFIX "glFrontFace");
	this->GenBuffers = (PROC_glGenBuffers)this->loadMethod(PREFIX "glGenBuffers");
	this->GenFramebuffers = (PROC_glGenFramebuffers)this->loadMethod(PREFIX "glGenFramebuffers");
	this->GenProgramPipelines = (PROC_glGenProgramPipelines)this->loadMethod(PREFIX "glGenProgramPipelines");
	this->GenQueries = (PROC_glGenQueries)this->loadMethod(PREFIX "glGenQueries");
	this->GenRenderbuffers = (PROC_glGenRenderbuffers)this->loadMethod(PREFIX "glGenRenderbuffers");
	this->GenSamplers = (PROC_glGenSamplers)this->loadMethod(PREFIX "glGenSamplers");
	this->GenTextures = (PROC_glGenTextures)this->loadMethod(PREFIX "glGenTextures");
	this->GenTransformFeedbacks = (PROC_glGenTransformFeedbacks)this->loadMethod(PREFIX "glGenTransformFeedbacks");
	this->GenVertexArrays = (PROC_glGenVertexArrays)this->loadMethod(PREFIX "glGenVertexArrays");
	this->GenerateMipmap = (PROC_glGenerateMipmap)this->loadMethod(PREFIX "glGenerateMipmap");
	this->GenerateTextureMipmap = (PROC_glGenerateTextureMipmap)this->loadMethod(PREFIX "glGenerateTextureMipmap");
	this->GetActiveAtomicCounterBufferiv = (PROC_glGetActiveAtomicCounterBufferiv)this->loadMethod(PREFIX "glGetActiveAtomicCounterBufferiv");
	this->GetActiveAttrib = (PROC_glGetActiveAttrib)this->loadMethod(PREFIX "glGetActiveAttrib");
	this->GetActiveSubroutineName = (PROC_glGetActiveSubroutineName)this->loadMethod(PREFIX "glGetActiveSubroutineName");
	this->GetActiveSubroutineUniformName = (PROC_glGetActiveSubroutineUniformName)this->loadMethod(PREFIX "glGetActiveSubroutineUniformName");
	this->GetActiveSubroutineUniformiv = (PROC_glGetActiveSubroutineUniformiv)this->loadMethod(PREFIX "glGetActiveSubroutineUniformiv");
	this->GetActiveUniform = (PROC_glGetActiveUniform)this->loadMethod(PREFIX "glGetActiveUniform");
	this->GetActiveUniformBlockName = (PROC_glGetActiveUniformBlockName)this->loadMethod(PREFIX "glGetActiveUniformBlockName");
	this->GetActiveUniformBlockiv = (PROC_glGetActiveUniformBlockiv)this->loadMethod(PREFIX "glGetActiveUniformBlockiv");
	this->GetActiveUniformName = (PROC_glGetActiveUniformName)this->loadMethod(PREFIX "glGetActiveUniformName");
	this->GetActiveUniformsiv = (PROC_glGetActiveUniformsiv)this->loadMethod(PREFIX "glGetActiveUniformsiv");
	this->GetAttachedShaders = (PROC_glGetAttachedShaders)this->loadMethod(PREFIX "glGetAttachedShaders");
	this->GetAttribLocation = (PROC_glGetAttribLocation)this->loadMethod(PREFIX "glGetAttribLocation");
	this->GetBooleani_v = (PROC_glGetBooleani_v)this->loadMethod(PREFIX "glGetBooleani_v");
	this->GetBooleanv = (PROC_glGetBooleanv)this->loadMethod(PREFIX "glGetBooleanv");
	this->GetBufferParameteri64v = (PROC_glGetBufferParameteri64v)this->loadMethod(PREFIX "glGetBufferParameteri64v");
	this->GetBufferParameteriv = (PROC_glGetBufferParameteriv)this->loadMethod(PREFIX "glGetBufferParameteriv");
	this->GetBufferPointerv = (PROC_glGetBufferPointerv)this->loadMethod(PREFIX "glGetBufferPointerv");
	this->GetBufferSubData = (PROC_glGetBufferSubData)this->loadMethod(PREFIX "glGetBufferSubData");
	this->GetCompressedTexImage = (PROC_glGetCompressedTexImage)this->loadMethod(PREFIX "glGetCompressedTexImage");
	this->GetCompressedTextureImage = (PROC_glGetCompressedTextureImage)this->loadMethod(PREFIX "glGetCompressedTextureImage");
	this->GetCompressedTextureSubImage = (PROC_glGetCompressedTextureSubImage)this->loadMethod(PREFIX "glGetCompressedTextureSubImage");
	this->GetDebugMessageLog = (PROC_glGetDebugMessageLog)this->loadMethod(PREFIX "glGetDebugMessageLog");
	this->GetDoublei_v = (PROC_glGetDoublei_v)this->loadMethod(PREFIX "glGetDoublei_v");
	this->GetDoublev = (PROC_glGetDoublev)this->loadMethod(PREFIX "glGetDoublev");
	this->GetError = (PROC_glGetError)this->loadMethod(PREFIX "glGetError");
	this->GetFloati_v = (PROC_glGetFloati_v)this->loadMethod(PREFIX "glGetFloati_v");
	this->GetFloatv = (PROC_glGetFloatv)this->loadMethod(PREFIX "glGetFloatv");
	this->GetFragDataIndex = (PROC_glGetFragDataIndex)this->loadMethod(PREFIX "glGetFragDataIndex");
	this->GetFragDataLocation = (PROC_glGetFragDataLocation)this->loadMethod(PREFIX "glGetFragDataLocation");
	this->GetFramebufferAttachmentParameteriv = (PROC_glGetFramebufferAttachmentParameteriv)this->loadMethod(PREFIX "glGetFramebufferAttachmentParameteriv");
	this->GetFramebufferParameteriv = (PROC_glGetFramebufferParameteriv)this->loadMethod(PREFIX "glGetFramebufferParameteriv");
	this->GetGraphicsResetStatus = (PROC_glGetGraphicsResetStatus)this->loadMethod(PREFIX "glGetGraphicsResetStatus");
	this->GetInteger64i_v = (PROC_glGetInteger64i_v)this->loadMethod(PREFIX "glGetInteger64i_v");
	this->GetInteger64v = (PROC_glGetInteger64v)this->loadMethod(PREFIX "glGetInteger64v");
	this->GetIntegeri_v = (PROC_glGetIntegeri_v)this->loadMethod(PREFIX "glGetIntegeri_v");
	this->GetIntegerv = (PROC_glGetIntegerv)this->loadMethod(PREFIX "glGetIntegerv");
	this->GetInternalformati64v = (PROC_glGetInternalformati64v)this->loadMethod(PREFIX "glGetInternalformati64v");
	this->GetInternalformativ = (PROC_glGetInternalformativ)this->loadMethod(PREFIX "glGetInternalformativ");
	this->GetMultisamplefv = (PROC_glGetMultisamplefv)this->loadMethod(PREFIX "glGetMultisamplefv");
	this->GetNamedBufferParameteri64v = (PROC_glGetNamedBufferParameteri64v)this->loadMethod(PREFIX "glGetNamedBufferParameteri64v");
	this->GetNamedBufferParameteriv = (PROC_glGetNamedBufferParameteriv)this->loadMethod(PREFIX "glGetNamedBufferParameteriv");
	this->GetNamedBufferPointerv = (PROC_glGetNamedBufferPointerv)this->loadMethod(PREFIX "glGetNamedBufferPointerv");
	this->GetNamedBufferSubData = (PROC_glGetNamedBufferSubData)this->loadMethod(PREFIX "glGetNamedBufferSubData");
	this->GetNamedFramebufferAttachmentParameteriv = (PROC_glGetNamedFramebufferAttachmentParameteriv)this->loadMethod(PREFIX "glGetNamedFramebufferAttachmentParameteriv");
	this->GetNamedFramebufferParameteriv = (PROC_glGetNamedFramebufferParameteriv)this->loadMethod(PREFIX "glGetNamedFramebufferParameteriv");
	this->GetNamedRenderbufferParameteriv = (PROC_glGetNamedRenderbufferParameteriv)this->loadMethod(PREFIX "glGetNamedRenderbufferParameteriv");
	this->GetObjectLabel = (PROC_glGetObjectLabel)this->loadMethod(PREFIX "glGetObjectLabel");
	this->GetObjectPtrLabel = (PROC_glGetObjectPtrLabel)this->loadMethod(PREFIX "glGetObjectPtrLabel");
	this->GetPointerv = (PROC_glGetPointerv)this->loadMethod(PREFIX "glGetPointerv");
	this->GetProgramBinary = (PROC_glGetProgramBinary)this->loadMethod(PREFIX "glGetProgramBinary");
	this->GetProgramInfoLog = (PROC_glGetProgramInfoLog)this->loadMethod(PREFIX "glGetProgramInfoLog");
	this->GetProgramInterfaceiv = (PROC_glGetProgramInterfaceiv)this->loadMethod(PREFIX "glGetProgramInterfaceiv");
	this->GetProgramPipelineInfoLog = (PROC_glGetProgramPipelineInfoLog)this->loadMethod(PREFIX "glGetProgramPipelineInfoLog");
	this->GetProgramPipelineiv = (PROC_glGetProgramPipelineiv)this->loadMethod(PREFIX "glGetProgramPipelineiv");
	this->GetProgramResourceIndex = (PROC_glGetProgramResourceIndex)this->loadMethod(PREFIX "glGetProgramResourceIndex");
	this->GetProgramResourceLocation = (PROC_glGetProgramResourceLocation)this->loadMethod(PREFIX "glGetProgramResourceLocation");
	this->GetProgramResourceLocationIndex = (PROC_glGetProgramResourceLocationIndex)this->loadMethod(PREFIX "glGetProgramResourceLocationIndex");
	this->GetProgramResourceName = (PROC_glGetProgramResourceName)this->loadMethod(PREFIX "glGetProgramResourceName");
	this->GetProgramResourceiv = (PROC_glGetProgramResourceiv)this->loadMethod(PREFIX "glGetProgramResourceiv");
	this->GetProgramStageiv = (PROC_glGetProgramStageiv)this->loadMethod(PREFIX "glGetProgramStageiv");
	this->GetProgramiv = (PROC_glGetProgramiv)this->loadMethod(PREFIX "glGetProgramiv");
	this->GetQueryBufferObjecti64v = (PROC_glGetQueryBufferObjecti64v)this->loadMethod(PREFIX "glGetQueryBufferObjecti64v");
	this->GetQueryBufferObjectiv = (PROC_glGetQueryBufferObjectiv)this->loadMethod(PREFIX "glGetQueryBufferObjectiv");
	this->GetQueryBufferObjectui64v = (PROC_glGetQueryBufferObjectui64v)this->loadMethod(PREFIX "glGetQueryBufferObjectui64v");
	this->GetQueryBufferObjectuiv = (PROC_glGetQueryBufferObjectuiv)this->loadMethod(PREFIX "glGetQueryBufferObjectuiv");
	this->GetQueryIndexediv = (PROC_glGetQueryIndexediv)this->loadMethod(PREFIX "glGetQueryIndexediv");
	this->GetQueryObjecti64v = (PROC_glGetQueryObjecti64v)this->loadMethod(PREFIX "glGetQueryObjecti64v");
	this->GetQueryObjectiv = (PROC_glGetQueryObjectiv)this->loadMethod(PREFIX "glGetQueryObjectiv");
	this->GetQueryObjectui64v = (PROC_glGetQueryObjectui64v)this->loadMethod(PREFIX "glGetQueryObjectui64v");
	this->GetQueryObjectuiv = (PROC_glGetQueryObjectuiv)this->loadMethod(PREFIX "glGetQueryObjectuiv");
	this->GetQueryiv = (PROC_glGetQueryiv)this->loadMethod(PREFIX "glGetQueryiv");
	this->GetRenderbufferParameteriv = (PROC_glGetRenderbufferParameteriv)this->loadMethod(PREFIX "glGetRenderbufferParameteriv");
	this->GetSamplerParameterIiv = (PROC_glGetSamplerParameterIiv)this->loadMethod(PREFIX "glGetSamplerParameterIiv");
	this->GetSamplerParameterIuiv = (PROC_glGetSamplerParameterIuiv)this->loadMethod(PREFIX "glGetSamplerParameterIuiv");
	this->GetSamplerParameterfv = (PROC_glGetSamplerParameterfv)this->loadMethod(PREFIX "glGetSamplerParameterfv");
	this->GetSamplerParameteriv = (PROC_glGetSamplerParameteriv)this->loadMethod(PREFIX "glGetSamplerParameteriv");
	this->GetShaderInfoLog = (PROC_glGetShaderInfoLog)this->loadMethod(PREFIX "glGetShaderInfoLog");
	this->GetShaderPrecisionFormat = (PROC_glGetShaderPrecisionFormat)this->loadMethod(PREFIX "glGetShaderPrecisionFormat");
	this->GetShaderSource = (PROC_glGetShaderSource)this->loadMethod(PREFIX "glGetShaderSource");
	this->GetShaderiv = (PROC_glGetShaderiv)this->loadMethod(PREFIX "glGetShaderiv");
	this->GetString = (PROC_glGetString)this->loadMethod(PREFIX "glGetString");
	this->GetStringi = (PROC_glGetStringi)this->loadMethod(PREFIX "glGetStringi");
	this->GetSubroutineIndex = (PROC_glGetSubroutineIndex)this->loadMethod(PREFIX "glGetSubroutineIndex");
	this->GetSubroutineUniformLocation = (PROC_glGetSubroutineUniformLocation)this->loadMethod(PREFIX "glGetSubroutineUniformLocation");
	this->GetSynciv = (PROC_glGetSynciv)this->loadMethod(PREFIX "glGetSynciv");
	this->GetTexImage = (PROC_glGetTexImage)this->loadMethod(PREFIX "glGetTexImage");
	this->GetTexLevelParameterfv = (PROC_glGetTexLevelParameterfv)this->loadMethod(PREFIX "glGetTexLevelParameterfv");
	this->GetTexLevelParameteriv = (PROC_glGetTexLevelParameteriv)this->loadMethod(PREFIX "glGetTexLevelParameteriv");
	this->GetTexParameterIiv = (PROC_glGetTexParameterIiv)this->loadMethod(PREFIX "glGetTexParameterIiv");
	this->GetTexParameterIuiv = (PROC_glGetTexParameterIuiv)this->loadMethod(PREFIX "glGetTexParameterIuiv");
	this->GetTexParameterfv = (PROC_glGetTexParameterfv)this->loadMethod(PREFIX "glGetTexParameterfv");
	this->GetTexParameteriv = (PROC_glGetTexParameteriv)this->loadMethod(PREFIX "glGetTexParameteriv");
	this->GetTextureImage = (PROC_glGetTextureImage)this->loadMethod(PREFIX "glGetTextureImage");
	this->GetTextureLevelParameterfv = (PROC_glGetTextureLevelParameterfv)this->loadMethod(PREFIX "glGetTextureLevelParameterfv");
	this->GetTextureLevelParameteriv = (PROC_glGetTextureLevelParameteriv)this->loadMethod(PREFIX "glGetTextureLevelParameteriv");
	this->GetTextureParameterIiv = (PROC_glGetTextureParameterIiv)this->loadMethod(PREFIX "glGetTextureParameterIiv");
	this->GetTextureParameterIuiv = (PROC_glGetTextureParameterIuiv)this->loadMethod(PREFIX "glGetTextureParameterIuiv");
	this->GetTextureParameterfv = (PROC_glGetTextureParameterfv)this->loadMethod(PREFIX "glGetTextureParameterfv");
	this->GetTextureParameteriv = (PROC_glGetTextureParameteriv)this->loadMethod(PREFIX "glGetTextureParameteriv");
	this->GetTextureSubImage = (PROC_glGetTextureSubImage)this->loadMethod(PREFIX "glGetTextureSubImage");
	this->GetTransformFeedbackVarying = (PROC_glGetTransformFeedbackVarying)this->loadMethod(PREFIX "glGetTransformFeedbackVarying");
	this->GetTransformFeedbacki64_v = (PROC_glGetTransformFeedbacki64_v)this->loadMethod(PREFIX "glGetTransformFeedbacki64_v");
	this->GetTransformFeedbacki_v = (PROC_glGetTransformFeedbacki_v)this->loadMethod(PREFIX "glGetTransformFeedbacki_v");
	this->GetTransformFeedbackiv = (PROC_glGetTransformFeedbackiv)this->loadMethod(PREFIX "glGetTransformFeedbackiv");
	this->GetUniformBlockIndex = (PROC_glGetUniformBlockIndex)this->loadMethod(PREFIX "glGetUniformBlockIndex");
	this->GetUniformIndices = (PROC_glGetUniformIndices)this->loadMethod(PREFIX "glGetUniformIndices");
	this->GetUniformLocation = (PROC_glGetUniformLocation)this->loadMethod(PREFIX "glGetUniformLocation");
	this->GetUniformSubroutineuiv = (PROC_glGetUniformSubroutineuiv)this->loadMethod(PREFIX "glGetUniformSubroutineuiv");
	this->GetUniformdv = (PROC_glGetUniformdv)this->loadMethod(PREFIX "glGetUniformdv");
	this->GetUniformfv = (PROC_glGetUniformfv)this->loadMethod(PREFIX "glGetUniformfv");
	this->GetUniformiv = (PROC_glGetUniformiv)this->loadMethod(PREFIX "glGetUniformiv");
	this->GetUniformuiv = (PROC_glGetUniformuiv)this->loadMethod(PREFIX "glGetUniformuiv");
	this->GetVertexArrayIndexed64iv = (PROC_glGetVertexArrayIndexed64iv)this->loadMethod(PREFIX "glGetVertexArrayIndexed64iv");
	this->GetVertexArrayIndexediv = (PROC_glGetVertexArrayIndexediv)this->loadMethod(PREFIX "glGetVertexArrayIndexediv");
	this->GetVertexArrayiv = (PROC_glGetVertexArrayiv)this->loadMethod(PREFIX "glGetVertexArrayiv");
	this->GetVertexAttribIiv = (PROC_glGetVertexAttribIiv)this->loadMethod(PREFIX "glGetVertexAttribIiv");
	this->GetVertexAttribIuiv = (PROC_glGetVertexAttribIuiv)this->loadMethod(PREFIX "glGetVertexAttribIuiv");
	this->GetVertexAttribLdv = (PROC_glGetVertexAttribLdv)this->loadMethod(PREFIX "glGetVertexAttribLdv");
	this->GetVertexAttribPointerv = (PROC_glGetVertexAttribPointerv)this->loadMethod(PREFIX "glGetVertexAttribPointerv");
	this->GetVertexAttribdv = (PROC_glGetVertexAttribdv)this->loadMethod(PREFIX "glGetVertexAttribdv");
	this->GetVertexAttribfv = (PROC_glGetVertexAttribfv)this->loadMethod(PREFIX "glGetVertexAttribfv");
	this->GetVertexAttribiv = (PROC_glGetVertexAttribiv)this->loadMethod(PREFIX "glGetVertexAttribiv");
	this->GetnCompressedTexImage = (PROC_glGetnCompressedTexImage)this->loadMethod(PREFIX "glGetnCompressedTexImage");
	this->GetnTexImage = (PROC_glGetnTexImage)this->loadMethod(PREFIX "glGetnTexImage");
	this->GetnUniformdv = (PROC_glGetnUniformdv)this->loadMethod(PREFIX "glGetnUniformdv");
	this->GetnUniformfv = (PROC_glGetnUniformfv)this->loadMethod(PREFIX "glGetnUniformfv");
	this->GetnUniformiv = (PROC_glGetnUniformiv)this->loadMethod(PREFIX "glGetnUniformiv");
	this->GetnUniformuiv = (PROC_glGetnUniformuiv)this->loadMethod(PREFIX "glGetnUniformuiv");
	this->Hint = (PROC_glHint)this->loadMethod(PREFIX "glHint");
	this->InvalidateBufferData = (PROC_glInvalidateBufferData)this->loadMethod(PREFIX "glInvalidateBufferData");
	this->InvalidateBufferSubData = (PROC_glInvalidateBufferSubData)this->loadMethod(PREFIX "glInvalidateBufferSubData");
	this->InvalidateFramebuffer = (PROC_glInvalidateFramebuffer)this->loadMethod(PREFIX "glInvalidateFramebuffer");
	this->InvalidateNamedFramebufferData = (PROC_glInvalidateNamedFramebufferData)this->loadMethod(PREFIX "glInvalidateNamedFramebufferData");
	this->InvalidateNamedFramebufferSubData = (PROC_glInvalidateNamedFramebufferSubData)this->loadMethod(PREFIX "glInvalidateNamedFramebufferSubData");
	this->InvalidateSubFramebuffer = (PROC_glInvalidateSubFramebuffer)this->loadMethod(PREFIX "glInvalidateSubFramebuffer");
	this->InvalidateTexImage = (PROC_glInvalidateTexImage)this->loadMethod(PREFIX "glInvalidateTexImage");
	this->InvalidateTexSubImage = (PROC_glInvalidateTexSubImage)this->loadMethod(PREFIX "glInvalidateTexSubImage");
	this->IsBuffer = (PROC_glIsBuffer)this->loadMethod(PREFIX "glIsBuffer");
	this->IsEnabled = (PROC_glIsEnabled)this->loadMethod(PREFIX "glIsEnabled");
	this->IsEnabledi = (PROC_glIsEnabledi)this->loadMethod(PREFIX "glIsEnabledi");
	this->IsFramebuffer = (PROC_glIsFramebuffer)this->loadMethod(PREFIX "glIsFramebuffer");
	this->IsProgram = (PROC_glIsProgram)this->loadMethod(PREFIX "glIsProgram");
	this->IsProgramPipeline = (PROC_glIsProgramPipeline)this->loadMethod(PREFIX "glIsProgramPipeline");
	this->IsQuery = (PROC_glIsQuery)this->loadMethod(PREFIX "glIsQuery");
	this->IsRenderbuffer = (PROC_glIsRenderbuffer)this->loadMethod(PREFIX "glIsRenderbuffer");
	this->IsSampler = (PROC_glIsSampler)this->loadMethod(PREFIX "glIsSampler");
	this->IsShader = (PROC_glIsShader)this->loadMethod(PREFIX "glIsShader");
	this->IsSync = (PROC_glIsSync)this->loadMethod(PREFIX "glIsSync");
	this->IsTexture = (PROC_glIsTexture)this->loadMethod(PREFIX "glIsTexture");
	this->IsTransformFeedback = (PROC_glIsTransformFeedback)this->loadMethod(PREFIX "glIsTransformFeedback");
	this->IsVertexArray = (PROC_glIsVertexArray)this->loadMethod(PREFIX "glIsVertexArray");
	this->LineWidth = (PROC_glLineWidth)this->loadMethod(PREFIX "glLineWidth");
	this->LinkProgram = (PROC_glLinkProgram)this->loadMethod(PREFIX "glLinkProgram");
	this->LogicOp = (PROC_glLogicOp)this->loadMethod(PREFIX "glLogicOp");
	this->MapBuffer = (PROC_glMapBuffer)this->loadMethod(PREFIX "glMapBuffer");
	this->MapBufferRange = (PROC_glMapBufferRange)this->loadMethod(PREFIX "glMapBufferRange");
	this->MapNamedBuffer = (PROC_glMapNamedBuffer)this->loadMethod(PREFIX "glMapNamedBuffer");
	this->MapNamedBufferRange = (PROC_glMapNamedBufferRange)this->loadMethod(PREFIX "glMapNamedBufferRange");
	this->MemoryBarrier = (PROC_glMemoryBarrier)this->loadMethod(PREFIX "glMemoryBarrier");
	this->MemoryBarrierByRegion = (PROC_glMemoryBarrierByRegion)this->loadMethod(PREFIX "glMemoryBarrierByRegion");
	this->MinSampleShading = (PROC_glMinSampleShading)this->loadMethod(PREFIX "glMinSampleShading");
	this->MultiDrawArrays = (PROC_glMultiDrawArrays)this->loadMethod(PREFIX "glMultiDrawArrays");
	this->MultiDrawArraysIndirect = (PROC_glMultiDrawArraysIndirect)this->loadMethod(PREFIX "glMultiDrawArraysIndirect");
	this->MultiDrawElements = (PROC_glMultiDrawElements)this->loadMethod(PREFIX "glMultiDrawElements");
	this->MultiDrawElementsBaseVertex = (PROC_glMultiDrawElementsBaseVertex)this->loadMethod(PREFIX "glMultiDrawElementsBaseVertex");
	this->MultiDrawElementsIndirect = (PROC_glMultiDrawElementsIndirect)this->loadMethod(PREFIX "glMultiDrawElementsIndirect");
	this->NamedBufferData = (PROC_glNamedBufferData)this->loadMethod(PREFIX "glNamedBufferData");
	this->NamedBufferStorage = (PROC_glNamedBufferStorage)this->loadMethod(PREFIX "glNamedBufferStorage");
	this->NamedBufferSubData = (PROC_glNamedBufferSubData)this->loadMethod(PREFIX "glNamedBufferSubData");
	this->NamedFramebufferDrawBuffer = (PROC_glNamedFramebufferDrawBuffer)this->loadMethod(PREFIX "glNamedFramebufferDrawBuffer");
	this->NamedFramebufferDrawBuffers = (PROC_glNamedFramebufferDrawBuffers)this->loadMethod(PREFIX "glNamedFramebufferDrawBuffers");
	this->NamedFramebufferParameteri = (PROC_glNamedFramebufferParameteri)this->loadMethod(PREFIX "glNamedFramebufferParameteri");
	this->NamedFramebufferReadBuffer = (PROC_glNamedFramebufferReadBuffer)this->loadMethod(PREFIX "glNamedFramebufferReadBuffer");
	this->NamedFramebufferRenderbuffer = (PROC_glNamedFramebufferRenderbuffer)this->loadMethod(PREFIX "glNamedFramebufferRenderbuffer");
	this->NamedFramebufferTexture = (PROC_glNamedFramebufferTexture)this->loadMethod(PREFIX "glNamedFramebufferTexture");
	this->NamedFramebufferTextureLayer = (PROC_glNamedFramebufferTextureLayer)this->loadMethod(PREFIX "glNamedFramebufferTextureLayer");
	this->NamedRenderbufferStorage = (PROC_glNamedRenderbufferStorage)this->loadMethod(PREFIX "glNamedRenderbufferStorage");
	this->NamedRenderbufferStorageMultisample = (PROC_glNamedRenderbufferStorageMultisample)this->loadMethod(PREFIX "glNamedRenderbufferStorageMultisample");
	this->ObjectLabel = (PROC_glObjectLabel)this->loadMethod(PREFIX "glObjectLabel");
	this->ObjectPtrLabel = (PROC_glObjectPtrLabel)this->loadMethod(PREFIX "glObjectPtrLabel");
	this->PatchParameterfv = (PROC_glPatchParameterfv)this->loadMethod(PREFIX "glPatchParameterfv");
	this->PatchParameteri = (PROC_glPatchParameteri)this->loadMethod(PREFIX "glPatchParameteri");
	this->PauseTransformFeedback = (PROC_glPauseTransformFeedback)this->loadMethod(PREFIX "glPauseTransformFeedback");
	this->PixelStoref = (PROC_glPixelStoref)this->loadMethod(PREFIX "glPixelStoref");
	this->PixelStorei = (PROC_glPixelStorei)this->loadMethod(PREFIX "glPixelStorei");
	this->PointParameterf = (PROC_glPointParameterf)this->loadMethod(PREFIX "glPointParameterf");
	this->PointParameterfv = (PROC_glPointParameterfv)this->loadMethod(PREFIX "glPointParameterfv");
	this->PointParameteri = (PROC_glPointParameteri)this->loadMethod(PREFIX "glPointParameteri");
	this->PointParameteriv = (PROC_glPointParameteriv)this->loadMethod(PREFIX "glPointParameteriv");
	this->PointSize = (PROC_glPointSize)this->loadMethod(PREFIX "glPointSize");
	this->PolygonMode = (PROC_glPolygonMode)this->loadMethod(PREFIX "glPolygonMode");
	this->PolygonOffset = (PROC_glPolygonOffset)this->loadMethod(PREFIX "glPolygonOffset");
	this->PopDebugGroup = (PROC_glPopDebugGroup)this->loadMethod(PREFIX "glPopDebugGroup");
	this->PrimitiveRestartIndex = (PROC_glPrimitiveRestartIndex)this->loadMethod(PREFIX "glPrimitiveRestartIndex");
	this->ProgramBinary = (PROC_glProgramBinary)this->loadMethod(PREFIX "glProgramBinary");
	this->ProgramParameteri = (PROC_glProgramParameteri)this->loadMethod(PREFIX "glProgramParameteri");
	this->ProgramUniform1d = (PROC_glProgramUniform1d)this->loadMethod(PREFIX "glProgramUniform1d");
	this->ProgramUniform1dv = (PROC_glProgramUniform1dv)this->loadMethod(PREFIX "glProgramUniform1dv");
	this->ProgramUniform1f = (PROC_glProgramUniform1f)this->loadMethod(PREFIX "glProgramUniform1f");
	this->ProgramUniform1fv = (PROC_glProgramUniform1fv)this->loadMethod(PREFIX "glProgramUniform1fv");
	this->ProgramUniform1i = (PROC_glProgramUniform1i)this->loadMethod(PREFIX "glProgramUniform1i");
	this->ProgramUniform1iv = (PROC_glProgramUniform1iv)this->loadMethod(PREFIX "glProgramUniform1iv");
	this->ProgramUniform1ui = (PROC_glProgramUniform1ui)this->loadMethod(PREFIX "glProgramUniform1ui");
	this->ProgramUniform1uiv = (PROC_glProgramUniform1uiv)this->loadMethod(PREFIX "glProgramUniform1uiv");
	this->ProgramUniform2d = (PROC_glProgramUniform2d)this->loadMethod(PREFIX "glProgramUniform2d");
	this->ProgramUniform2dv = (PROC_glProgramUniform2dv)this->loadMethod(PREFIX "glProgramUniform2dv");
	this->ProgramUniform2f = (PROC_glProgramUniform2f)this->loadMethod(PREFIX "glProgramUniform2f");
	this->ProgramUniform2fv = (PROC_glProgramUniform2fv)this->loadMethod(PREFIX "glProgramUniform2fv");
	this->ProgramUniform2i = (PROC_glProgramUniform2i)this->loadMethod(PREFIX "glProgramUniform2i");
	this->ProgramUniform2iv = (PROC_glProgramUniform2iv)this->loadMethod(PREFIX "glProgramUniform2iv");
	this->ProgramUniform2ui = (PROC_glProgramUniform2ui)this->loadMethod(PREFIX "glProgramUniform2ui");
	this->ProgramUniform2uiv = (PROC_glProgramUniform2uiv)this->loadMethod(PREFIX "glProgramUniform2uiv");
	this->ProgramUniform3d = (PROC_glProgramUniform3d)this->loadMethod(PREFIX "glProgramUniform3d");
	this->ProgramUniform3dv = (PROC_glProgramUniform3dv)this->loadMethod(PREFIX "glProgramUniform3dv");
	this->ProgramUniform3f = (PROC_glProgramUniform3f)this->loadMethod(PREFIX "glProgramUniform3f");
	this->ProgramUniform3fv = (PROC_glProgramUniform3fv)this->loadMethod(PREFIX "glProgramUniform3fv");
	this->ProgramUniform3i = (PROC_glProgramUniform3i)this->loadMethod(PREFIX "glProgramUniform3i");
	this->ProgramUniform3iv = (PROC_glProgramUniform3iv)this->loadMethod(PREFIX "glProgramUniform3iv");
	this->ProgramUniform3ui = (PROC_glProgramUniform3ui)this->loadMethod(PREFIX "glProgramUniform3ui");
	this->ProgramUniform3uiv = (PROC_glProgramUniform3uiv)this->loadMethod(PREFIX "glProgramUniform3uiv");
	this->ProgramUniform4d = (PROC_glProgramUniform4d)this->loadMethod(PREFIX "glProgramUniform4d");
	this->ProgramUniform4dv = (PROC_glProgramUniform4dv)this->loadMethod(PREFIX "glProgramUniform4dv");
	this->ProgramUniform4f = (PROC_glProgramUniform4f)this->loadMethod(PREFIX "glProgramUniform4f");
	this->ProgramUniform4fv = (PROC_glProgramUniform4fv)this->loadMethod(PREFIX "glProgramUniform4fv");
	this->ProgramUniform4i = (PROC_glProgramUniform4i)this->loadMethod(PREFIX "glProgramUniform4i");
	this->ProgramUniform4iv = (PROC_glProgramUniform4iv)this->loadMethod(PREFIX "glProgramUniform4iv");
	this->ProgramUniform4ui = (PROC_glProgramUniform4ui)this->loadMethod(PREFIX "glProgramUniform4ui");
	this->ProgramUniform4uiv = (PROC_glProgramUniform4uiv)this->loadMethod(PREFIX "glProgramUniform4uiv");
	this->ProgramUniformMatrix2dv = (PROC_glProgramUniformMatrix2dv)this->loadMethod(PREFIX "glProgramUniformMatrix2dv");
	this->ProgramUniformMatrix2fv = (PROC_glProgramUniformMatrix2fv)this->loadMethod(PREFIX "glProgramUniformMatrix2fv");
	this->ProgramUniformMatrix2x3dv = (PROC_glProgramUniformMatrix2x3dv)this->loadMethod(PREFIX "glProgramUniformMatrix2x3dv");
	this->ProgramUniformMatrix2x3fv = (PROC_glProgramUniformMatrix2x3fv)this->loadMethod(PREFIX "glProgramUniformMatrix2x3fv");
	this->ProgramUniformMatrix2x4dv = (PROC_glProgramUniformMatrix2x4dv)this->loadMethod(PREFIX "glProgramUniformMatrix2x4dv");
	this->ProgramUniformMatrix2x4fv = (PROC_glProgramUniformMatrix2x4fv)this->loadMethod(PREFIX "glProgramUniformMatrix2x4fv");
	this->ProgramUniformMatrix3dv = (PROC_glProgramUniformMatrix3dv)this->loadMethod(PREFIX "glProgramUniformMatrix3dv");
	this->ProgramUniformMatrix3fv = (PROC_glProgramUniformMatrix3fv)this->loadMethod(PREFIX "glProgramUniformMatrix3fv");
	this->ProgramUniformMatrix3x2dv = (PROC_glProgramUniformMatrix3x2dv)this->loadMethod(PREFIX "glProgramUniformMatrix3x2dv");
	this->ProgramUniformMatrix3x2fv = (PROC_glProgramUniformMatrix3x2fv)this->loadMethod(PREFIX "glProgramUniformMatrix3x2fv");
	this->ProgramUniformMatrix3x4dv = (PROC_glProgramUniformMatrix3x4dv)this->loadMethod(PREFIX "glProgramUniformMatrix3x4dv");
	this->ProgramUniformMatrix3x4fv = (PROC_glProgramUniformMatrix3x4fv)this->loadMethod(PREFIX "glProgramUniformMatrix3x4fv");
	this->ProgramUniformMatrix4dv = (PROC_glProgramUniformMatrix4dv)this->loadMethod(PREFIX "glProgramUniformMatrix4dv");
	this->ProgramUniformMatrix4fv = (PROC_glProgramUniformMatrix4fv)this->loadMethod(PREFIX "glProgramUniformMatrix4fv");
	this->ProgramUniformMatrix4x2dv = (PROC_glProgramUniformMatrix4x2dv)this->loadMethod(PREFIX "glProgramUniformMatrix4x2dv");
	this->ProgramUniformMatrix4x2fv = (PROC_glProgramUniformMatrix4x2fv)this->loadMethod(PREFIX "glProgramUniformMatrix4x2fv");
	this->ProgramUniformMatrix4x3dv = (PROC_glProgramUniformMatrix4x3dv)this->loadMethod(PREFIX "glProgramUniformMatrix4x3dv");
	this->ProgramUniformMatrix4x3fv = (PROC_glProgramUniformMatrix4x3fv)this->loadMethod(PREFIX "glProgramUniformMatrix4x3fv");
	this->ProvokingVertex = (PROC_glProvokingVertex)this->loadMethod(PREFIX "glProvokingVertex");
	this->PushDebugGroup = (PROC_glPushDebugGroup)this->loadMethod(PREFIX "glPushDebugGroup");
	this->QueryCounter = (PROC_glQueryCounter)this->loadMethod(PREFIX "glQueryCounter");
	this->ReadBuffer = (PROC_glReadBuffer)this->loadMethod(PREFIX "glReadBuffer");
	this->ReadPixels = (PROC_glReadPixels)this->loadMethod(PREFIX "glReadPixels");
	this->ReadnPixels = (PROC_glReadnPixels)this->loadMethod(PREFIX "glReadnPixels");
	this->ReleaseShaderCompiler = (PROC_glReleaseShaderCompiler)this->loadMethod(PREFIX "glReleaseShaderCompiler");
	this->RenderbufferStorage = (PROC_glRenderbufferStorage)this->loadMethod(PREFIX "glRenderbufferStorage");
	this->RenderbufferStorageMultisample = (PROC_glRenderbufferStorageMultisample)this->loadMethod(PREFIX "glRenderbufferStorageMultisample");
	this->ResumeTransformFeedback = (PROC_glResumeTransformFeedback)this->loadMethod(PREFIX "glResumeTransformFeedback");
	this->SampleCoverage = (PROC_glSampleCoverage)this->loadMethod(PREFIX "glSampleCoverage");
	this->SampleMaski = (PROC_glSampleMaski)this->loadMethod(PREFIX "glSampleMaski");
	this->SamplerParameterIiv = (PROC_glSamplerParameterIiv)this->loadMethod(PREFIX "glSamplerParameterIiv");
	this->SamplerParameterIuiv = (PROC_glSamplerParameterIuiv)this->loadMethod(PREFIX "glSamplerParameterIuiv");
	this->SamplerParameterf = (PROC_glSamplerParameterf)this->loadMethod(PREFIX "glSamplerParameterf");
	this->SamplerParameterfv = (PROC_glSamplerParameterfv)this->loadMethod(PREFIX "glSamplerParameterfv");
	this->SamplerParameteri = (PROC_glSamplerParameteri)this->loadMethod(PREFIX "glSamplerParameteri");
	this->SamplerParameteriv = (PROC_glSamplerParameteriv)this->loadMethod(PREFIX "glSamplerParameteriv");
	this->Scissor = (PROC_glScissor)this->loadMethod(PREFIX "glScissor");
	this->ScissorArrayv = (PROC_glScissorArrayv)this->loadMethod(PREFIX "glScissorArrayv");
	this->ScissorIndexed = (PROC_glScissorIndexed)this->loadMethod(PREFIX "glScissorIndexed");
	this->ScissorIndexedv = (PROC_glScissorIndexedv)this->loadMethod(PREFIX "glScissorIndexedv");
	this->ShaderBinary = (PROC_glShaderBinary)this->loadMethod(PREFIX "glShaderBinary");
	this->ShaderSource = (PROC_glShaderSource)this->loadMethod(PREFIX "glShaderSource");
	this->ShaderStorageBlockBinding = (PROC_glShaderStorageBlockBinding)this->loadMethod(PREFIX "glShaderStorageBlockBinding");
	this->StencilFunc = (PROC_glStencilFunc)this->loadMethod(PREFIX "glStencilFunc");
	this->StencilFuncSeparate = (PROC_glStencilFuncSeparate)this->loadMethod(PREFIX "glStencilFuncSeparate");
	this->StencilMask = (PROC_glStencilMask)this->loadMethod(PREFIX "glStencilMask");
	this->StencilMaskSeparate = (PROC_glStencilMaskSeparate)this->loadMethod(PREFIX "glStencilMaskSeparate");
	this->StencilOp = (PROC_glStencilOp)this->loadMethod(PREFIX "glStencilOp");
	this->StencilOpSeparate = (PROC_glStencilOpSeparate)this->loadMethod(PREFIX "glStencilOpSeparate");
	this->TexBuffer = (PROC_glTexBuffer)this->loadMethod(PREFIX "glTexBuffer");
	this->TexBufferRange = (PROC_glTexBufferRange)this->loadMethod(PREFIX "glTexBufferRange");
	this->TexImage1D = (PROC_glTexImage1D)this->loadMethod(PREFIX "glTexImage1D");
	this->TexImage2D = (PROC_glTexImage2D)this->loadMethod(PREFIX "glTexImage2D");
	this->TexImage2DMultisample = (PROC_glTexImage2DMultisample)this->loadMethod(PREFIX "glTexImage2DMultisample");
	this->TexImage3D = (PROC_glTexImage3D)this->loadMethod(PREFIX "glTexImage3D");
	this->TexImage3DMultisample = (PROC_glTexImage3DMultisample)this->loadMethod(PREFIX "glTexImage3DMultisample");
	this->TexParameterIiv = (PROC_glTexParameterIiv)this->loadMethod(PREFIX "glTexParameterIiv");
	this->TexParameterIuiv = (PROC_glTexParameterIuiv)this->loadMethod(PREFIX "glTexParameterIuiv");
	this->TexParameterf = (PROC_glTexParameterf)this->loadMethod(PREFIX "glTexParameterf");
	this->TexParameterfv = (PROC_glTexParameterfv)this->loadMethod(PREFIX "glTexParameterfv");
	this->TexParameteri = (PROC_glTexParameteri)this->loadMethod(PREFIX "glTexParameteri");
	this->TexParameteriv = (PROC_glTexParameteriv)this->loadMethod(PREFIX "glTexParameteriv");
	this->TexStorage1D = (PROC_glTexStorage1D)this->loadMethod(PREFIX "glTexStorage1D");
	this->TexStorage2D = (PROC_glTexStorage2D)this->loadMethod(PREFIX "glTexStorage2D");
	this->TexStorage2DMultisample = (PROC_glTexStorage2DMultisample)this->loadMethod(PREFIX "glTexStorage2DMultisample");
	this->TexStorage3D = (PROC_glTexStorage3D)this->loadMethod(PREFIX "glTexStorage3D");
	this->TexStorage3DMultisample = (PROC_glTexStorage3DMultisample)this->loadMethod(PREFIX "glTexStorage3DMultisample");
	this->TexSubImage1D = (PROC_glTexSubImage1D)this->loadMethod(PREFIX "glTexSubImage1D");
	this->TexSubImage2D = (PROC_glTexSubImage2D)this->loadMethod(PREFIX "glTexSubImage2D");
	this->TexSubImage3D = (PROC_glTexSubImage3D)this->loadMethod(PREFIX "glTexSubImage3D");
	this->TextureBarrier = (PROC_glTextureBarrier)this->loadMethod(PREFIX "glTextureBarrier");
	this->TextureBuffer = (PROC_glTextureBuffer)this->loadMethod(PREFIX "glTextureBuffer");
	this->TextureBufferRange = (PROC_glTextureBufferRange)this->loadMethod(PREFIX "glTextureBufferRange");
	this->TextureParameterIiv = (PROC_glTextureParameterIiv)this->loadMethod(PREFIX "glTextureParameterIiv");
	this->TextureParameterIuiv = (PROC_glTextureParameterIuiv)this->loadMethod(PREFIX "glTextureParameterIuiv");
	this->TextureParameterf = (PROC_glTextureParameterf)this->loadMethod(PREFIX "glTextureParameterf");
	this->TextureParameterfv = (PROC_glTextureParameterfv)this->loadMethod(PREFIX "glTextureParameterfv");
	this->TextureParameteri = (PROC_glTextureParameteri)this->loadMethod(PREFIX "glTextureParameteri");
	this->TextureParameteriv = (PROC_glTextureParameteriv)this->loadMethod(PREFIX "glTextureParameteriv");
	this->TextureStorage1D = (PROC_glTextureStorage1D)this->loadMethod(PREFIX "glTextureStorage1D");
	this->TextureStorage2D = (PROC_glTextureStorage2D)this->loadMethod(PREFIX "glTextureStorage2D");
	this->TextureStorage2DMultisample = (PROC_glTextureStorage2DMultisample)this->loadMethod(PREFIX "glTextureStorage2DMultisample");
	this->TextureStorage3D = (PROC_glTextureStorage3D)this->loadMethod(PREFIX "glTextureStorage3D");
	this->TextureStorage3DMultisample = (PROC_glTextureStorage3DMultisample)this->loadMethod(PREFIX "glTextureStorage3DMultisample");
	this->TextureSubImage1D = (PROC_glTextureSubImage1D)this->loadMethod(PREFIX "glTextureSubImage1D");
	this->TextureSubImage2D = (PROC_glTextureSubImage2D)this->loadMethod(PREFIX "glTextureSubImage2D");
	this->TextureSubImage3D = (PROC_glTextureSubImage3D)this->loadMethod(PREFIX "glTextureSubImage3D");
	this->TextureView = (PROC_glTextureView)this->loadMethod(PREFIX "glTextureView");
	this->TransformFeedbackBufferBase = (PROC_glTransformFeedbackBufferBase)this->loadMethod(PREFIX "glTransformFeedbackBufferBase");
	this->TransformFeedbackBufferRange = (PROC_glTransformFeedbackBufferRange)this->loadMethod(PREFIX "glTransformFeedbackBufferRange");
	this->TransformFeedbackVaryings = (PROC_glTransformFeedbackVaryings)this->loadMethod(PREFIX "glTransformFeedbackVaryings");
	this->Uniform1d = (PROC_glUniform1d)this->loadMethod(PREFIX "glUniform1d");
	this->Uniform1dv = (PROC_glUniform1dv)this->loadMethod(PREFIX "glUniform1dv");
	this->Uniform1f = (PROC_glUniform1f)this->loadMethod(PREFIX "glUniform1f");
	this->Uniform1fv = (PROC_glUniform1fv)this->loadMethod(PREFIX "glUniform1fv");
	this->Uniform1i = (PROC_glUniform1i)this->loadMethod(PREFIX "glUniform1i");
	this->Uniform1iv = (PROC_glUniform1iv)this->loadMethod(PREFIX "glUniform1iv");
	this->Uniform1ui = (PROC_glUniform1ui)this->loadMethod(PREFIX "glUniform1ui");
	this->Uniform1uiv = (PROC_glUniform1uiv)this->loadMethod(PREFIX "glUniform1uiv");
	this->Uniform2d = (PROC_glUniform2d)this->loadMethod(PREFIX "glUniform2d");
	this->Uniform2dv = (PROC_glUniform2dv)this->loadMethod(PREFIX "glUniform2dv");
	this->Uniform2f = (PROC_glUniform2f)this->loadMethod(PREFIX "glUniform2f");
	this->Uniform2fv = (PROC_glUniform2fv)this->loadMethod(PREFIX "glUniform2fv");
	this->Uniform2i = (PROC_glUniform2i)this->loadMethod(PREFIX "glUniform2i");
	this->Uniform2iv = (PROC_glUniform2iv)this->loadMethod(PREFIX "glUniform2iv");
	this->Uniform2ui = (PROC_glUniform2ui)this->loadMethod(PREFIX "glUniform2ui");
	this->Uniform2uiv = (PROC_glUniform2uiv)this->loadMethod(PREFIX "glUniform2uiv");
	this->Uniform3d = (PROC_glUniform3d)this->loadMethod(PREFIX "glUniform3d");
	this->Uniform3dv = (PROC_glUniform3dv)this->loadMethod(PREFIX "glUniform3dv");
	this->Uniform3f = (PROC_glUniform3f)this->loadMethod(PREFIX "glUniform3f");
	this->Uniform3fv = (PROC_glUniform3fv)this->loadMethod(PREFIX "glUniform3fv");
	this->Uniform3i = (PROC_glUniform3i)this->loadMethod(PREFIX "glUniform3i");
	this->Uniform3iv = (PROC_glUniform3iv)this->loadMethod(PREFIX "glUniform3iv");
	this->Uniform3ui = (PROC_glUniform3ui)this->loadMethod(PREFIX "glUniform3ui");
	this->Uniform3uiv = (PROC_glUniform3uiv)this->loadMethod(PREFIX "glUniform3uiv");
	this->Uniform4d = (PROC_glUniform4d)this->loadMethod(PREFIX "glUniform4d");
	this->Uniform4dv = (PROC_glUniform4dv)this->loadMethod(PREFIX "glUniform4dv");
	this->Uniform4f = (PROC_glUniform4f)this->loadMethod(PREFIX "glUniform4f");
	this->Uniform4fv = (PROC_glUniform4fv)this->loadMethod(PREFIX "glUniform4fv");
	this->Uniform4i = (PROC_glUniform4i)this->loadMethod(PREFIX "glUniform4i");
	this->Uniform4iv = (PROC_glUniform4iv)this->loadMethod(PREFIX "glUniform4iv");
	this->Uniform4ui = (PROC_glUniform4ui)this->loadMethod(PREFIX "glUniform4ui");
	this->Uniform4uiv = (PROC_glUniform4uiv)this->loadMethod(PREFIX "glUniform4uiv");
	this->UniformBlockBinding = (PROC_glUniformBlockBinding)this->loadMethod(PREFIX "glUniformBlockBinding");
	this->UniformMatrix2dv = (PROC_glUniformMatrix2dv)this->loadMethod(PREFIX "glUniformMatrix2dv");
	this->UniformMatrix2fv = (PROC_glUniformMatrix2fv)this->loadMethod(PREFIX "glUniformMatrix2fv");
	this->UniformMatrix2x3dv = (PROC_glUniformMatrix2x3dv)this->loadMethod(PREFIX "glUniformMatrix2x3dv");
	this->UniformMatrix2x3fv = (PROC_glUniformMatrix2x3fv)this->loadMethod(PREFIX "glUniformMatrix2x3fv");
	this->UniformMatrix2x4dv = (PROC_glUniformMatrix2x4dv)this->loadMethod(PREFIX "glUniformMatrix2x4dv");
	this->UniformMatrix2x4fv = (PROC_glUniformMatrix2x4fv)this->loadMethod(PREFIX "glUniformMatrix2x4fv");
	this->UniformMatrix3dv = (PROC_glUniformMatrix3dv)this->loadMethod(PREFIX "glUniformMatrix3dv");
	this->UniformMatrix3fv = (PROC_glUniformMatrix3fv)this->loadMethod(PREFIX "glUniformMatrix3fv");
	this->UniformMatrix3x2dv = (PROC_glUniformMatrix3x2dv)this->loadMethod(PREFIX "glUniformMatrix3x2dv");
	this->UniformMatrix3x2fv = (PROC_glUniformMatrix3x2fv)this->loadMethod(PREFIX "glUniformMatrix3x2fv");
	this->UniformMatrix3x4dv = (PROC_glUniformMatrix3x4dv)this->loadMethod(PREFIX "glUniformMatrix3x4dv");
	this->UniformMatrix3x4fv = (PROC_glUniformMatrix3x4fv)this->loadMethod(PREFIX "glUniformMatrix3x4fv");
	this->UniformMatrix4dv = (PROC_glUniformMatrix4dv)this->loadMethod(PREFIX "glUniformMatrix4dv");
	this->UniformMatrix4fv = (PROC_glUniformMatrix4fv)this->loadMethod(PREFIX "glUniformMatrix4fv");
	this->UniformMatrix4x2dv = (PROC_glUniformMatrix4x2dv)this->loadMethod(PREFIX "glUniformMatrix4x2dv");
	this->UniformMatrix4x2fv = (PROC_glUniformMatrix4x2fv)this->loadMethod(PREFIX "glUniformMatrix4x2fv");
	this->UniformMatrix4x3dv = (PROC_glUniformMatrix4x3dv)this->loadMethod(PREFIX "glUniformMatrix4x3dv");
	this->UniformMatrix4x3fv = (PROC_glUniformMatrix4x3fv)this->loadMethod(PREFIX "glUniformMatrix4x3fv");
	this->UniformSubroutinesuiv = (PROC_glUniformSubroutinesuiv)this->loadMethod(PREFIX "glUniformSubroutinesuiv");
	this->UnmapBuffer = (PROC_glUnmapBuffer)this->loadMethod(PREFIX "glUnmapBuffer");
	this->UnmapNamedBuffer = (PROC_glUnmapNamedBuffer)this->loadMethod(PREFIX "glUnmapNamedBuffer");
	this->UseProgram = (PROC_glUseProgram)this->loadMethod(PREFIX "glUseProgram");
	this->UseProgramStages = (PROC_glUseProgramStages)this->loadMethod(PREFIX "glUseProgramStages");
	this->ValidateProgram = (PROC_glValidateProgram)this->loadMethod(PREFIX "glValidateProgram");
	this->ValidateProgramPipeline = (PROC_glValidateProgramPipeline)this->loadMethod(PREFIX "glValidateProgramPipeline");
	this->VertexArrayAttribBinding = (PROC_glVertexArrayAttribBinding)this->loadMethod(PREFIX "glVertexArrayAttribBinding");
	this->VertexArrayAttribFormat = (PROC_glVertexArrayAttribFormat)this->loadMethod(PREFIX "glVertexArrayAttribFormat");
	this->VertexArrayAttribIFormat = (PROC_glVertexArrayAttribIFormat)this->loadMethod(PREFIX "glVertexArrayAttribIFormat");
	this->VertexArrayAttribLFormat = (PROC_glVertexArrayAttribLFormat)this->loadMethod(PREFIX "glVertexArrayAttribLFormat");
	this->VertexArrayBindingDivisor = (PROC_glVertexArrayBindingDivisor)this->loadMethod(PREFIX "glVertexArrayBindingDivisor");
	this->VertexArrayElementBuffer = (PROC_glVertexArrayElementBuffer)this->loadMethod(PREFIX "glVertexArrayElementBuffer");
	this->VertexArrayVertexBuffer = (PROC_glVertexArrayVertexBuffer)this->loadMethod(PREFIX "glVertexArrayVertexBuffer");
	this->VertexArrayVertexBuffers = (PROC_glVertexArrayVertexBuffers)this->loadMethod(PREFIX "glVertexArrayVertexBuffers");
	this->VertexAttrib1d = (PROC_glVertexAttrib1d)this->loadMethod(PREFIX "glVertexAttrib1d");
	this->VertexAttrib1dv = (PROC_glVertexAttrib1dv)this->loadMethod(PREFIX "glVertexAttrib1dv");
	this->VertexAttrib1f = (PROC_glVertexAttrib1f)this->loadMethod(PREFIX "glVertexAttrib1f");
	this->VertexAttrib1fv = (PROC_glVertexAttrib1fv)this->loadMethod(PREFIX "glVertexAttrib1fv");
	this->VertexAttrib1s = (PROC_glVertexAttrib1s)this->loadMethod(PREFIX "glVertexAttrib1s");
	this->VertexAttrib1sv = (PROC_glVertexAttrib1sv)this->loadMethod(PREFIX "glVertexAttrib1sv");
	this->VertexAttrib2d = (PROC_glVertexAttrib2d)this->loadMethod(PREFIX "glVertexAttrib2d");
	this->VertexAttrib2dv = (PROC_glVertexAttrib2dv)this->loadMethod(PREFIX "glVertexAttrib2dv");
	this->VertexAttrib2f = (PROC_glVertexAttrib2f)this->loadMethod(PREFIX "glVertexAttrib2f");
	this->VertexAttrib2fv = (PROC_glVertexAttrib2fv)this->loadMethod(PREFIX "glVertexAttrib2fv");
	this->VertexAttrib2s = (PROC_glVertexAttrib2s)this->loadMethod(PREFIX "glVertexAttrib2s");
	this->VertexAttrib2sv = (PROC_glVertexAttrib2sv)this->loadMethod(PREFIX "glVertexAttrib2sv");
	this->VertexAttrib3d = (PROC_glVertexAttrib3d)this->loadMethod(PREFIX "glVertexAttrib3d");
	this->VertexAttrib3dv = (PROC_glVertexAttrib3dv)this->loadMethod(PREFIX "glVertexAttrib3dv");
	this->VertexAttrib3f = (PROC_glVertexAttrib3f)this->loadMethod(PREFIX "glVertexAttrib3f");
	this->VertexAttrib3fv = (PROC_glVertexAttrib3fv)this->loadMethod(PREFIX "glVertexAttrib3fv");
	this->VertexAttrib3s = (PROC_glVertexAttrib3s)this->loadMethod(PREFIX "glVertexAttrib3s");
	this->VertexAttrib3sv = (PROC_glVertexAttrib3sv)this->loadMethod(PREFIX "glVertexAttrib3sv");
	this->VertexAttrib4Nbv = (PROC_glVertexAttrib4Nbv)this->loadMethod(PREFIX "glVertexAttrib4Nbv");
	this->VertexAttrib4Niv = (PROC_glVertexAttrib4Niv)this->loadMethod(PREFIX "glVertexAttrib4Niv");
	this->VertexAttrib4Nsv = (PROC_glVertexAttrib4Nsv)this->loadMethod(PREFIX "glVertexAttrib4Nsv");
	this->VertexAttrib4Nub = (PROC_glVertexAttrib4Nub)this->loadMethod(PREFIX "glVertexAttrib4Nub");
	this->VertexAttrib4Nubv = (PROC_glVertexAttrib4Nubv)this->loadMethod(PREFIX "glVertexAttrib4Nubv");
	this->VertexAttrib4Nuiv = (PROC_glVertexAttrib4Nuiv)this->loadMethod(PREFIX "glVertexAttrib4Nuiv");
	this->VertexAttrib4Nusv = (PROC_glVertexAttrib4Nusv)this->loadMethod(PREFIX "glVertexAttrib4Nusv");
	this->VertexAttrib4bv = (PROC_glVertexAttrib4bv)this->loadMethod(PREFIX "glVertexAttrib4bv");
	this->VertexAttrib4d = (PROC_glVertexAttrib4d)this->loadMethod(PREFIX "glVertexAttrib4d");
	this->VertexAttrib4dv = (PROC_glVertexAttrib4dv)this->loadMethod(PREFIX "glVertexAttrib4dv");
	this->VertexAttrib4f = (PROC_glVertexAttrib4f)this->loadMethod(PREFIX "glVertexAttrib4f");
	this->VertexAttrib4fv = (PROC_glVertexAttrib4fv)this->loadMethod(PREFIX "glVertexAttrib4fv");
	this->VertexAttrib4iv = (PROC_glVertexAttrib4iv)this->loadMethod(PREFIX "glVertexAttrib4iv");
	this->VertexAttrib4s = (PROC_glVertexAttrib4s)this->loadMethod(PREFIX "glVertexAttrib4s");
	this->VertexAttrib4sv = (PROC_glVertexAttrib4sv)this->loadMethod(PREFIX "glVertexAttrib4sv");
	this->VertexAttrib4ubv = (PROC_glVertexAttrib4ubv)this->loadMethod(PREFIX "glVertexAttrib4ubv");
	this->VertexAttrib4uiv = (PROC_glVertexAttrib4uiv)this->loadMethod(PREFIX "glVertexAttrib4uiv");
	this->VertexAttrib4usv = (PROC_glVertexAttrib4usv)this->loadMethod(PREFIX "glVertexAttrib4usv");
	this->VertexAttribBinding = (PROC_glVertexAttribBinding)this->loadMethod(PREFIX "glVertexAttribBinding");
	this->VertexAttribDivisor = (PROC_glVertexAttribDivisor)this->loadMethod(PREFIX "glVertexAttribDivisor");
	this->VertexAttribFormat = (PROC_glVertexAttribFormat)this->loadMethod(PREFIX "glVertexAttribFormat");
	this->VertexAttribI1i = (PROC_glVertexAttribI1i)this->loadMethod(PREFIX "glVertexAttribI1i");
	this->VertexAttribI1iv = (PROC_glVertexAttribI1iv)this->loadMethod(PREFIX "glVertexAttribI1iv");
	this->VertexAttribI1ui = (PROC_glVertexAttribI1ui)this->loadMethod(PREFIX "glVertexAttribI1ui");
	this->VertexAttribI1uiv = (PROC_glVertexAttribI1uiv)this->loadMethod(PREFIX "glVertexAttribI1uiv");
	this->VertexAttribI2i = (PROC_glVertexAttribI2i)this->loadMethod(PREFIX "glVertexAttribI2i");
	this->VertexAttribI2iv = (PROC_glVertexAttribI2iv)this->loadMethod(PREFIX "glVertexAttribI2iv");
	this->VertexAttribI2ui = (PROC_glVertexAttribI2ui)this->loadMethod(PREFIX "glVertexAttribI2ui");
	this->VertexAttribI2uiv = (PROC_glVertexAttribI2uiv)this->loadMethod(PREFIX "glVertexAttribI2uiv");
	this->VertexAttribI3i = (PROC_glVertexAttribI3i)this->loadMethod(PREFIX "glVertexAttribI3i");
	this->VertexAttribI3iv = (PROC_glVertexAttribI3iv)this->loadMethod(PREFIX "glVertexAttribI3iv");
	this->VertexAttribI3ui = (PROC_glVertexAttribI3ui)this->loadMethod(PREFIX "glVertexAttribI3ui");
	this->VertexAttribI3uiv = (PROC_glVertexAttribI3uiv)this->loadMethod(PREFIX "glVertexAttribI3uiv");
	this->VertexAttribI4bv = (PROC_glVertexAttribI4bv)this->loadMethod(PREFIX "glVertexAttribI4bv");
	this->VertexAttribI4i = (PROC_glVertexAttribI4i)this->loadMethod(PREFIX "glVertexAttribI4i");
	this->VertexAttribI4iv = (PROC_glVertexAttribI4iv)this->loadMethod(PREFIX "glVertexAttribI4iv");
	this->VertexAttribI4sv = (PROC_glVertexAttribI4sv)this->loadMethod(PREFIX "glVertexAttribI4sv");
	this->VertexAttribI4ubv = (PROC_glVertexAttribI4ubv)this->loadMethod(PREFIX "glVertexAttribI4ubv");
	this->VertexAttribI4ui = (PROC_glVertexAttribI4ui)this->loadMethod(PREFIX "glVertexAttribI4ui");
	this->VertexAttribI4uiv = (PROC_glVertexAttribI4uiv)this->loadMethod(PREFIX "glVertexAttribI4uiv");
	this->VertexAttribI4usv = (PROC_glVertexAttribI4usv)this->loadMethod(PREFIX "glVertexAttribI4usv");
	this->VertexAttribIFormat = (PROC_glVertexAttribIFormat)this->loadMethod(PREFIX "glVertexAttribIFormat");
	this->VertexAttribIPointer = (PROC_glVertexAttribIPointer)this->loadMethod(PREFIX "glVertexAttribIPointer");
	this->VertexAttribL1d = (PROC_glVertexAttribL1d)this->loadMethod(PREFIX "glVertexAttribL1d");
	this->VertexAttribL1dv = (PROC_glVertexAttribL1dv)this->loadMethod(PREFIX "glVertexAttribL1dv");
	this->VertexAttribL2d = (PROC_glVertexAttribL2d)this->loadMethod(PREFIX "glVertexAttribL2d");
	this->VertexAttribL2dv = (PROC_glVertexAttribL2dv)this->loadMethod(PREFIX "glVertexAttribL2dv");
	this->VertexAttribL3d = (PROC_glVertexAttribL3d)this->loadMethod(PREFIX "glVertexAttribL3d");
	this->VertexAttribL3dv = (PROC_glVertexAttribL3dv)this->loadMethod(PREFIX "glVertexAttribL3dv");
	this->VertexAttribL4d = (PROC_glVertexAttribL4d)this->loadMethod(PREFIX "glVertexAttribL4d");
	this->VertexAttribL4dv = (PROC_glVertexAttribL4dv)this->loadMethod(PREFIX "glVertexAttribL4dv");
	this->VertexAttribLFormat = (PROC_glVertexAttribLFormat)this->loadMethod(PREFIX "glVertexAttribLFormat");
	this->VertexAttribLPointer = (PROC_glVertexAttribLPointer)this->loadMethod(PREFIX "glVertexAttribLPointer");
	this->VertexAttribP1ui = (PROC_glVertexAttribP1ui)this->loadMethod(PREFIX "glVertexAttribP1ui");
	this->VertexAttribP1uiv = (PROC_glVertexAttribP1uiv)this->loadMethod(PREFIX "glVertexAttribP1uiv");
	this->VertexAttribP2ui = (PROC_glVertexAttribP2ui)this->loadMethod(PREFIX "glVertexAttribP2ui");
	this->VertexAttribP2uiv = (PROC_glVertexAttribP2uiv)this->loadMethod(PREFIX "glVertexAttribP2uiv");
	this->VertexAttribP3ui = (PROC_glVertexAttribP3ui)this->loadMethod(PREFIX "glVertexAttribP3ui");
	this->VertexAttribP3uiv = (PROC_glVertexAttribP3uiv)this->loadMethod(PREFIX "glVertexAttribP3uiv");
	this->VertexAttribP4ui = (PROC_glVertexAttribP4ui)this->loadMethod(PREFIX "glVertexAttribP4ui");
	this->VertexAttribP4uiv = (PROC_glVertexAttribP4uiv)this->loadMethod(PREFIX "glVertexAttribP4uiv");
	this->VertexAttribPointer = (PROC_glVertexAttribPointer)this->loadMethod(PREFIX "glVertexAttribPointer");
	this->VertexBindingDivisor = (PROC_glVertexBindingDivisor)this->loadMethod(PREFIX "glVertexBindingDivisor");
	this->Viewport = (PROC_glViewport)this->loadMethod(PREFIX "glViewport");
	this->ViewportArrayv = (PROC_glViewportArrayv)this->loadMethod(PREFIX "glViewportArrayv");
	this->ViewportIndexedf = (PROC_glViewportIndexedf)this->loadMethod(PREFIX "glViewportIndexedf");
	this->ViewportIndexedfv = (PROC_glViewportIndexedfv)this->loadMethod(PREFIX "glViewportIndexedfv");

	// Fake implementation

	if (INVALID_METHOD(this->GetProgramStageiv)) {
		this->GetProgramStageiv = FakeGetProgramStageiv;
	}

	// GetActiveSubroutineName
	// GetActiveSubroutineUniformName
	// GetSubroutineIndex
	// GetSubroutineUniformLocation

	// Must have

	PyObject * loading_log = PyUnicode_FromStringAndSize(0, 0);

	if (INVALID_METHOD(this->ActiveTexture)) {
		AddToLog(&loading_log, "glActiveTexture");
	}

	if (INVALID_METHOD(this->AttachShader)) {
		AddToLog(&loading_log, "glAttachShader");
	}

	if (INVALID_METHOD(this->BeginTransformFeedback)) {
		AddToLog(&loading_log, "glBeginTransformFeedback");
	}

	if (INVALID_METHOD(this->BindBuffer)) {
		AddToLog(&loading_log, "glBindBuffer");
	}

	if (INVALID_METHOD(this->BindBufferBase)) {
		AddToLog(&loading_log, "glBindBufferBase");
	}

	if (INVALID_METHOD(this->BindFramebuffer)) {
		AddToLog(&loading_log, "glBindFramebuffer");
	}

	if (INVALID_METHOD(this->BindRenderbuffer)) {
		AddToLog(&loading_log, "glBindRenderbuffer");
	}

	if (INVALID_METHOD(this->BindTexture)) {
		AddToLog(&loading_log, "glBindTexture");
	}

	if (INVALID_METHOD(this->BindVertexArray)) {
		AddToLog(&loading_log, "glBindVertexArray");
	}

	if (INVALID_METHOD(this->BlendFunc)) {
		AddToLog(&loading_log, "glBlendFunc");
	}

	if (INVALID_METHOD(this->BufferData)) {
		AddToLog(&loading_log, "glBufferData");
	}

	if (INVALID_METHOD(this->BufferSubData)) {
		AddToLog(&loading_log, "glBufferSubData");
	}

	if (INVALID_METHOD(this->CheckFramebufferStatus)) {
		AddToLog(&loading_log, "glCheckFramebufferStatus");
	}

	if (INVALID_METHOD(this->Clear)) {
		AddToLog(&loading_log, "glClear");
	}

	if (INVALID_METHOD(this->ClearColor)) {
		AddToLog(&loading_log, "glClearColor");
	}

	if (INVALID_METHOD(this->CompileShader)) {
		AddToLog(&loading_log, "glCompileShader");
	}

	if (INVALID_METHOD(this->CopyBufferSubData)) {
		AddToLog(&loading_log, "glCopyBufferSubData");
	}

	if (INVALID_METHOD(this->CreateProgram)) {
		AddToLog(&loading_log, "glCreateProgram");
	}

	if (INVALID_METHOD(this->CreateShader)) {
		AddToLog(&loading_log, "glCreateShader");
	}

	if (INVALID_METHOD(this->DeleteBuffers)) {
		AddToLog(&loading_log, "glDeleteBuffers");
	}

	if (INVALID_METHOD(this->DeleteFramebuffers)) {
		AddToLog(&loading_log, "glDeleteFramebuffers");
	}

	if (INVALID_METHOD(this->DeleteProgram)) {
		AddToLog(&loading_log, "glDeleteProgram");
	}

	if (INVALID_METHOD(this->DeleteRenderbuffers)) {
		AddToLog(&loading_log, "glDeleteRenderbuffers");
	}

	if (INVALID_METHOD(this->DeleteShader)) {
		AddToLog(&loading_log, "glDeleteShader");
	}

	if (INVALID_METHOD(this->DeleteTextures)) {
		AddToLog(&loading_log, "glDeleteTextures");
	}

	if (INVALID_METHOD(this->DeleteVertexArrays)) {
		AddToLog(&loading_log, "glDeleteVertexArrays");
	}

	if (INVALID_METHOD(this->Disable)) {
		AddToLog(&loading_log, "glDisable");
	}

	if (INVALID_METHOD(this->DisableVertexAttribArray)) {
		AddToLog(&loading_log, "glDisableVertexAttribArray");
	}

	if (INVALID_METHOD(this->DrawArraysInstanced)) {
		AddToLog(&loading_log, "glDrawArraysInstanced");
	}

	if (INVALID_METHOD(this->DrawElementsInstanced)) {
		AddToLog(&loading_log, "glDrawElementsInstanced");
	}

	if (INVALID_METHOD(this->Enable)) {
		AddToLog(&loading_log, "glEnable");
	}

	if (INVALID_METHOD(this->EnableVertexAttribArray)) {
		AddToLog(&loading_log, "glEnableVertexAttribArray");
	}

	if (INVALID_METHOD(this->EndTransformFeedback)) {
		AddToLog(&loading_log, "glEndTransformFeedback");
	}

	if (INVALID_METHOD(this->Finish)) {
		AddToLog(&loading_log, "glFinish");
	}

	if (INVALID_METHOD(this->Flush)) {
		AddToLog(&loading_log, "glFlush");
	}

	if (INVALID_METHOD(this->FramebufferRenderbuffer)) {
		AddToLog(&loading_log, "glFramebufferRenderbuffer");
	}

	if (INVALID_METHOD(this->FramebufferTexture2D)) {
		AddToLog(&loading_log, "glFramebufferTexture2D");
	}

	if (INVALID_METHOD(this->GenBuffers)) {
		AddToLog(&loading_log, "glGenBuffers");
	}

	if (INVALID_METHOD(this->GenFramebuffers)) {
		AddToLog(&loading_log, "glGenFramebuffers");
	}

	if (INVALID_METHOD(this->GenRenderbuffers)) {
		AddToLog(&loading_log, "glGenRenderbuffers");
	}

	if (INVALID_METHOD(this->GenTextures)) {
		AddToLog(&loading_log, "glGenTextures");
	}

	if (INVALID_METHOD(this->GenVertexArrays)) {
		AddToLog(&loading_log, "glGenVertexArrays");
	}

	if (INVALID_METHOD(this->GetActiveAttrib)) {
		AddToLog(&loading_log, "glGetActiveAttrib");
	}

	if (INVALID_METHOD(this->GetActiveUniform)) {
		AddToLog(&loading_log, "glGetActiveUniform");
	}

	if (INVALID_METHOD(this->GetActiveUniformBlockiv)) {
		AddToLog(&loading_log, "glGetActiveUniformBlockiv");
	}

	if (INVALID_METHOD(this->GetActiveUniformBlockName)) {
		AddToLog(&loading_log, "glGetActiveUniformBlockName");
	}

	if (INVALID_METHOD(this->GetAttribLocation)) {
		AddToLog(&loading_log, "glGetAttribLocation");
	}

	if (INVALID_METHOD(this->GetFloatv)) {
		AddToLog(&loading_log, "glGetFloatv");
	}

	if (INVALID_METHOD(this->GetIntegerv)) {
		AddToLog(&loading_log, "glGetIntegerv");
	}

	if (INVALID_METHOD(this->GetProgramInfoLog)) {
		AddToLog(&loading_log, "glGetProgramInfoLog");
	}

	if (INVALID_METHOD(this->GetProgramiv)) {
		AddToLog(&loading_log, "glGetProgramiv");
	}

	if (INVALID_METHOD(this->GetShaderInfoLog)) {
		AddToLog(&loading_log, "glGetShaderInfoLog");
	}

	if (INVALID_METHOD(this->GetShaderiv)) {
		AddToLog(&loading_log, "glGetShaderiv");
	}

	if (INVALID_METHOD(this->GetString)) {
		AddToLog(&loading_log, "glGetString");
	}

	if (INVALID_METHOD(this->GetTransformFeedbackVarying)) {
		AddToLog(&loading_log, "glGetTransformFeedbackVarying");
	}

	if (INVALID_METHOD(this->GetUniformdv)) {
		AddToLog(&loading_log, "glGetUniformdv");
	}

	if (INVALID_METHOD(this->GetUniformfv)) {
		AddToLog(&loading_log, "glGetUniformfv");
	}

	if (INVALID_METHOD(this->GetUniformiv)) {
		AddToLog(&loading_log, "glGetUniformiv");
	}

	if (INVALID_METHOD(this->GetUniformLocation)) {
		AddToLog(&loading_log, "glGetUniformLocation");
	}

	if (INVALID_METHOD(this->GetUniformuiv)) {
		AddToLog(&loading_log, "glGetUniformuiv");
	}

	if (INVALID_METHOD(this->GetVertexAttribiv)) {
		AddToLog(&loading_log, "glGetVertexAttribiv");
	}

	if (INVALID_METHOD(this->LineWidth)) {
		AddToLog(&loading_log, "glLineWidth");
	}

	if (INVALID_METHOD(this->LinkProgram)) {
		AddToLog(&loading_log, "glLinkProgram");
	}

	if (INVALID_METHOD(this->MapBufferRange)) {
		AddToLog(&loading_log, "glMapBufferRange");
	}

	if (INVALID_METHOD(this->PointSize)) {
		AddToLog(&loading_log, "glPointSize");
	}

	if (INVALID_METHOD(this->PrimitiveRestartIndex)) {
		AddToLog(&loading_log, "glPrimitiveRestartIndex");
	}

	if (INVALID_METHOD(this->ProgramUniform1dv)) {
		AddToLog(&loading_log, "glProgramUniform1dv");
	}

	if (INVALID_METHOD(this->ProgramUniform1fv)) {
		AddToLog(&loading_log, "glProgramUniform1fv");
	}

	if (INVALID_METHOD(this->ProgramUniform1iv)) {
		AddToLog(&loading_log, "glProgramUniform1iv");
	}

	if (INVALID_METHOD(this->ProgramUniform1uiv)) {
		AddToLog(&loading_log, "glProgramUniform1uiv");
	}

	if (INVALID_METHOD(this->ProgramUniform2dv)) {
		AddToLog(&loading_log, "glProgramUniform2dv");
	}

	if (INVALID_METHOD(this->ProgramUniform2fv)) {
		AddToLog(&loading_log, "glProgramUniform2fv");
	}

	if (INVALID_METHOD(this->ProgramUniform2iv)) {
		AddToLog(&loading_log, "glProgramUniform2iv");
	}

	if (INVALID_METHOD(this->ProgramUniform2uiv)) {
		AddToLog(&loading_log, "glProgramUniform2uiv");
	}

	if (INVALID_METHOD(this->ProgramUniform3dv)) {
		AddToLog(&loading_log, "glProgramUniform3dv");
	}

	if (INVALID_METHOD(this->ProgramUniform3fv)) {
		AddToLog(&loading_log, "glProgramUniform3fv");
	}

	if (INVALID_METHOD(this->ProgramUniform3iv)) {
		AddToLog(&loading_log, "glProgramUniform3iv");
	}

	if (INVALID_METHOD(this->ProgramUniform3uiv)) {
		AddToLog(&loading_log, "glProgramUniform3uiv");
	}

	if (INVALID_METHOD(this->ProgramUniform4dv)) {
		AddToLog(&loading_log, "glProgramUniform4dv");
	}

	if (INVALID_METHOD(this->ProgramUniform4fv)) {
		AddToLog(&loading_log, "glProgramUniform4fv");
	}

	if (INVALID_METHOD(this->ProgramUniform4iv)) {
		AddToLog(&loading_log, "glProgramUniform4iv");
	}

	if (INVALID_METHOD(this->ProgramUniform4uiv)) {
		AddToLog(&loading_log, "glProgramUniform4uiv");
	}

	if (INVALID_METHOD(this->ProgramUniformMatrix2dv)) {
		AddToLog(&loading_log, "glProgramUniformMatrix2dv");
	}

	if (INVALID_METHOD(this->ProgramUniformMatrix2fv)) {
		AddToLog(&loading_log, "glProgramUniformMatrix2fv");
	}

	if (INVALID_METHOD(this->ProgramUniformMatrix2x3dv)) {
		AddToLog(&loading_log, "glProgramUniformMatrix2x3dv");
	}

	if (INVALID_METHOD(this->ProgramUniformMatrix2x3fv)) {
		AddToLog(&loading_log, "glProgramUniformMatrix2x3fv");
	}

	if (INVALID_METHOD(this->ProgramUniformMatrix2x4dv)) {
		AddToLog(&loading_log, "glProgramUniformMatrix2x4dv");
	}

	if (INVALID_METHOD(this->ProgramUniformMatrix2x4fv)) {
		AddToLog(&loading_log, "glProgramUniformMatrix2x4fv");
	}

	if (INVALID_METHOD(this->ProgramUniformMatrix3dv)) {
		AddToLog(&loading_log, "glProgramUniformMatrix3dv");
	}

	if (INVALID_METHOD(this->ProgramUniformMatrix3fv)) {
		AddToLog(&loading_log, "glProgramUniformMatrix3fv");
	}

	if (INVALID_METHOD(this->ProgramUniformMatrix3x2dv)) {
		AddToLog(&loading_log, "glProgramUniformMatrix3x2dv");
	}

	if (INVALID_METHOD(this->ProgramUniformMatrix3x2fv)) {
		AddToLog(&loading_log, "glProgramUniformMatrix3x2fv");
	}

	if (INVALID_METHOD(this->ProgramUniformMatrix3x4dv)) {
		AddToLog(&loading_log, "glProgramUniformMatrix3x4dv");
	}

	if (INVALID_METHOD(this->ProgramUniformMatrix3x4fv)) {
		AddToLog(&loading_log, "glProgramUniformMatrix3x4fv");
	}

	if (INVALID_METHOD(this->ProgramUniformMatrix4dv)) {
		AddToLog(&loading_log, "glProgramUniformMatrix4dv");
	}

	if (INVALID_METHOD(this->ProgramUniformMatrix4fv)) {
		AddToLog(&loading_log, "glProgramUniformMatrix4fv");
	}

	if (INVALID_METHOD(this->ProgramUniformMatrix4x2dv)) {
		AddToLog(&loading_log, "glProgramUniformMatrix4x2dv");
	}

	if (INVALID_METHOD(this->ProgramUniformMatrix4x2fv)) {
		AddToLog(&loading_log, "glProgramUniformMatrix4x2fv");
	}

	if (INVALID_METHOD(this->ProgramUniformMatrix4x3dv)) {
		AddToLog(&loading_log, "glProgramUniformMatrix4x3dv");
	}

	if (INVALID_METHOD(this->ProgramUniformMatrix4x3fv)) {
		AddToLog(&loading_log, "glProgramUniformMatrix4x3fv");
	}

	if (INVALID_METHOD(this->ReadPixels)) {
		AddToLog(&loading_log, "glReadPixels");
	}

	if (INVALID_METHOD(this->RenderbufferStorage)) {
		AddToLog(&loading_log, "glRenderbufferStorage");
	}

	if (INVALID_METHOD(this->ShaderSource)) {
		AddToLog(&loading_log, "glShaderSource");
	}

	if (INVALID_METHOD(this->TexImage2D)) {
		AddToLog(&loading_log, "glTexImage2D");
	}

	if (INVALID_METHOD(this->TexParameteri)) {
		AddToLog(&loading_log, "glTexParameteri");
	}

	if (INVALID_METHOD(this->TexSubImage2D)) {
		AddToLog(&loading_log, "glTexSubImage2D");
	}

	if (INVALID_METHOD(this->TransformFeedbackVaryings)) {
		AddToLog(&loading_log, "glTransformFeedbackVaryings");
	}

	if (INVALID_METHOD(this->UnmapBuffer)) {
		AddToLog(&loading_log, "glUnmapBuffer");
	}

	if (INVALID_METHOD(this->UseProgram)) {
		AddToLog(&loading_log, "glUseProgram");
	}

	if (INVALID_METHOD(this->VertexAttribDivisor)) {
		AddToLog(&loading_log, "glVertexAttribDivisor");
	}

	if (INVALID_METHOD(this->VertexAttribIPointer)) {
		AddToLog(&loading_log, "glVertexAttribIPointer");
	}

	// if (INVALID_METHOD(this->VertexAttribLPointer)) {
	// 	AddToLog(&loading_log, "glVertexAttribLPointer");
	// }

	if (INVALID_METHOD(this->VertexAttribPointer)) {
		AddToLog(&loading_log, "glVertexAttribPointer");
	}

	if (INVALID_METHOD(this->Viewport)) {
		AddToLog(&loading_log, "glViewport");
	}

	if (PyUnicode_GET_SIZE(loading_log) != 0) {
		PyObject * message = PyUnicode_FromFormat("Error this->loading some OpenGL functions:\n");
		PyUnicode_Append(&message, loading_log);
		Py_DECREF(loading_log);

		MGLError_Set(message);
		return false;
	}

	Py_DECREF(loading_log);
	return true;
}
