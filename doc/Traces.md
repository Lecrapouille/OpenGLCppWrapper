# How to debug OpenGL ?

I'm using [apitrace](https://github.com/apitrace/apitrace) for debuging the correct OpenGL calls in my API.

General way to call apitrace:
```sh
apitrace trace --api gl path/to/your/binary
qapitrace binary.trace &
```

Where `path/to/your/binary` has to be changed by the path of your application using OpenGL and `binary.trace` has to be replaced by the name of your application.

For OpenGLCppWrapper:
```sh
cd OpenGLCppWrapper/examples
apitrace trace --api gl build/OpenGLExamples 10
qapitrace OpenGLExamples.trace
```

You probably have to remove the previous trace `OpenGLExamples.trace` else a `OpenGLExamples.1.trace` will be created.
