// Demo: https://threejs.org/examples/webgl_camera.html
// Code: https://github.com/mrdoob/three.js/blob/master/examples/webgl_camera.html
void aa()
{
  scene = Node3D::create(); // Scene3d::create() ?
  viewPers = CameraHelper::create(PERSPECTIVE);
  veiwOrth = CameraHelper::create(ORTHO);
  scene->attach(viewPers).attach(veiwOrth);

  cameraRig = Node3D::create();
  cameraPers = PerspectiveCamera::create();
  cameraOrth = OrthographicCamera::create();
  cameraRig->attach(cameraPers).attach(cameraOrth);
  scene->attach(cameraRig);

  sphere1 = Sphere::create();
  sphere2 = Sphere::create();
  scene->attach(sphere1).attach(sphere2);

  sphere3 = Sphere::create();
  cameraRig->attach(sphere3);

  // stars = 
}
