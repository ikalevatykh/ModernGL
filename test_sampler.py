import ctypes
import moderngl_debugger
import moderngl as mgl

ctx = mgl.create_standalone_context()
sampler = ctx.sampler()
print(sampler.foo())
print(sampler.bar)
