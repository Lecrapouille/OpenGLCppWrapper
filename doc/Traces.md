# Traces OpenGL calls

I'm using [apitrace](https://github.com/apitrace/apitrace) for debuging OpenGL calls in my API. Here how to do that:

```sh
apitrace trace --api gl path/to/your/binary
qapitrace OpenGLExamples.trace &
```
