# RayTracingInOneWeekend_BVHplus
本项目主要参考https://raytracing.github.io/books/RayTracingInOneWeekend.html#outputanimage中的教程编写完成一个基础的RayTracing。
项目主要涵盖了以下几个方面的内容：

  （1）如何使用简单的光线追踪在屏幕中显示球体。
  
  （2）如何通过在像素点周围采样来提高图像渲染质量并进行Anti-Alising（反走样）。
  
  （3）如何构建物体obj基类（项目中的hittable.h）,并编写sphere.h的球体派生类，进而实现世界坐标系中光线与添加的物体的相交检测等功能。注：本项目使用方式为由相机发射光线进行渲染，因此不涉及model、view、projection等矩阵变换。
  
  （4）如何通过递归实现光线在不同物体之间的弹射。（包括反射reflection和折射refrection）
  
  （5）实现不同材质的光线交互接口，包括lambertain的漫反射diffuse、metal的全反射（存在fuzz系数决定全反射与漫反射的比例以实现毛金属的效果）和 dielectric（电介质，主要是玻璃）的折射效果。
  
  （6）实现可移动的相机类，其中需要实现散焦模糊效果进一步凸显景深的影响。
  
  （7）最终渲染，这是一个在cpu上运行的渲染程序，为了得到一张高质量的渲染图片，渲染了非常久，但整体效果非常好。
  
![result](https://user-images.githubusercontent.com/104212697/164740699-62270ea2-483b-4c15-a20a-c00709117eb5.jpg)

  addition：
  
  （8）本人尝试对程序进行优化加速，尝试通过编写代码来实现BVH加速结构，最终确实能够实现，但是由于场景中的可交互对象仅有400+个球体，BVH树的加速效果不明显，反而是每一次的包围盒求交拖慢了整体程序的运行速度。而BVH对于MeshTriangle（海量三角形组成的模型）的光线求交的加速效果是非常优秀的（仍有改进空间），毕竟构建了树之后，每次光线求交 从O（n）变为了O（log n）次查找光线与基础单元的求交。
