## Window Tutorials

This group of tutorials will show you how to create and manipulate a window for
rendering an OpenGL application.

Steps for having a window are the same for all examples:

- Create a window class derivating from `GLWindow`. let name it `MyFirstWindow`
  for this document.

- In `MyFirstWindow` class, you shall override methods: `onSetup()` and
  `onPaint()`. Tag them private since other classes shall not have to call these
  methods. Indeed this methods are directly called by `GLWindow`.

- `onSetup()` and `onPaint()` call `onSetupFailed(std::string const& reason)`
  and `onPaintFailed(std::string const& reason)` when something odd
  happened. Tag them private since other classes shall not have to call these
  methods.

- Depending on your project goals, you can enable window events such as mouse,
  keyboard through the method `makeReactOn(Events)` and implement callback
  methods such as `onMouseScrolled()` ... Tag them private since other classes
  have to call these methods. You can overide `onWindowResized()` to react when
  the window has been resized. Call template method `width<T>()` and
  `height<T>()` to know the new size.

- `GLWindow` needs to be created and managed by a `GLApplication` to allow to
  run. Thanks `GLApplication` you can create simply several windows. The minimal
  code in your `main()` function will be something like:

```
GLApplication app;
app.create<MyFirstWindow>(800u, 600u, "my title 1");
app.create<MySecondWindow>(800u, 600u, "my title 2");
app.start()
```

Example 00:
===========

This example creates a dummy OpenGL window, nothing more! This example shows
you the minmal base code.

Example 01:
===========

Following the previous example, this example creates a dummy OpenGL window
reacting to events such as keyboard, pressed/released event, mouse button click,
mouse movement, mouse scrolling, window resize event.

This example uses the delta time to change the background color.

Example 02:
===========

This example shows how to initialize [Dear ImGui](https://github.com/ocornut/imgui)
context to create widgets and buttons inside your OpenGL window. In this example, a
simple button is added for changing the background color.
